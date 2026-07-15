#include "NetworkClient.hpp"

#include <QMetaObject>

NetworkClient::NetworkClient(QObject* parent)
    : QObject(parent),
      resolver(ioContext),
      socket(ioContext),
      connectedState(false)
{
}

NetworkClient::~NetworkClient()
{
    disconnectFromServer();
}

void NetworkClient::connectToServer(const QString& host, unsigned short port)
{
    try
    {
        auto endpoints = resolver.resolve(
            host.toStdString(),
            std::to_string(port)
        );

        boost::asio::async_connect(
            socket,
            endpoints,
            [this](const boost::system::error_code& error,
                   const tcp::endpoint&)
            {
                if (error)
                {
                    QMetaObject::invokeMethod(
                        this,
                        [this]()
                        {
                            emit connectionError(
                                "Could not connect to the server."
                            );
                        },
                        Qt::QueuedConnection
                    );

                    return;
                }

                connectedState = true;

                QMetaObject::invokeMethod(
                    this,
                    [this]()
                    {
                        emit connected();
                    },
                    Qt::QueuedConnection
                );

                startReading();
            }
        );

        networkThread = std::thread(
            [this]()
            {
                try
                {
                    ioContext.run();
                }
                catch (const std::exception&)
                {
                    QMetaObject::invokeMethod(
                        this,
                        [this]()
                        {
                            emit connectionError(
                                "A network error occurred."
                            );
                        },
                        Qt::QueuedConnection
                    );
                }
            }
        );
    }
    catch (const std::exception&)
    {
        emit connectionError("Unable to start the network connection.");
    }
}

void NetworkClient::disconnectFromServer()
{
    if (!connectedState && !networkThread.joinable())
    {
        return;
    }

    boost::asio::post(
        ioContext,
        [this]()
        {
            closeConnection();
        }
    );

    ioContext.stop();

    if (networkThread.joinable())
    {
        networkThread.join();
    }
}

void NetworkClient::login(
    const QString& username,
    const QString& password)
{
    sendJson({
        {"type", "login"},
        {"username", username.toStdString()},
        {"password", password.toStdString()}
    });
}

void NetworkClient::registerUser(
    const QString& username,
    const QString& password,
    const QString& role)
{
    sendJson({
        {"type", "register"},
        {"username", username.toStdString()},
        {"password", password.toStdString()},
        {"role", role.toStdString()}
    });
}

void NetworkClient::submitReport(
    const QString& itemName,
    const QString& category,
    const QString& location,
    const QString& date,
    const QString& reportType,
    const QString& imageData)
{
    sendJson({
        {"type", "submit_report"},
        {"itemName", itemName.toStdString()},
        {"category", category.toStdString()},
        {"location", location.toStdString()},
        {"date", date.toStdString()},
        {"reportType", reportType.toStdString()},
        {"imageData", imageData.toStdString()}
    });
}

void NetworkClient::updateStatus(
    int reportIndex,
    const QString& newStatus)
{
    sendJson({
        {"type", "update_status"},
        {"reportIndex", reportIndex},
        {"newStatus", newStatus.toStdString()}
    });
}

void NetworkClient::requestAllReports()
{
    sendJson({
        {"type", "get_all_reports"}
    });
}

bool NetworkClient::isConnected() const
{
    return connectedState;
}

void NetworkClient::startReading()
{
    boost::asio::async_read_until(
        socket,
        incomingBuffer,
        '\n',
        [this](
            const boost::system::error_code& error,
            std::size_t)
        {
            if (error)
            {
                closeConnection();

                QMetaObject::invokeMethod(
                    this,
                    [this]()
                    {
                        emit disconnected();
                    },
                    Qt::QueuedConnection
                );

                return;
            }

            std::istream input(&incomingBuffer);
            std::string line;
            std::getline(input, line);

            handleMessage(line);
            startReading();
        }
    );
}

void NetworkClient::handleMessage(const std::string& jsonLine)
{
    try
    {
        nlohmann::json message =
            nlohmann::json::parse(jsonLine);

        std::string type =
            message.value("type", "");

        if (type == "login_result")
        {
            bool success =
                message.value("success", false);

            QString username =
                QString::fromStdString(
                    message.value("username", "")
                );

            QString role =
                QString::fromStdString(
                    message.value("role", "")
                );

            QMetaObject::invokeMethod(
                this,
                [this, success, username, role]()
                {
                    emit loginResult(
                        success,
                        username,
                        role
                    );
                },
                Qt::QueuedConnection
            );
        }
        else if (type == "register_result")
        {
            bool success =
                message.value("success", false);

            QString text =
                QString::fromStdString(
                    message.value("message", "")
                );

            QMetaObject::invokeMethod(
                this,
                [this, success, text]()
                {
                    emit registerResult(
                        success,
                        text
                    );
                },
                Qt::QueuedConnection
            );
        }
        else if (type == "reports_snapshot")
        {
            QString reportsJson =
                QString::fromStdString(
                    message["reports"].dump()
                );

            QMetaObject::invokeMethod(
                this,
                [this, reportsJson]()
                {
                    emit reportsSnapshot(reportsJson);
                },
                Qt::QueuedConnection
            );
        }
        else if (type == "notification")
        {
            QString text =
                QString::fromStdString(
                    message.value("message", "")
                );

            QMetaObject::invokeMethod(
                this,
                [this, text]()
                {
                    emit notificationReceived(text);
                },
                Qt::QueuedConnection
            );
        }
        else if (type == "error")
        {
            QString text =
                QString::fromStdString(
                    message.value(
                        "message",
                        "The server reported an error."
                    )
                );

            QMetaObject::invokeMethod(
                this,
                [this, text]()
                {
                    emit serverError(text);
                },
                Qt::QueuedConnection
            );
        }
        else
        {
            QMetaObject::invokeMethod(
                this,
                [this]()
                {
                    emit serverError(
                        "The server sent an unknown response."
                    );
                },
                Qt::QueuedConnection
            );
        }
    }
    catch (const nlohmann::json::exception&)
    {
        QMetaObject::invokeMethod(
            this,
            [this]()
            {
                emit serverError(
                    "The server sent an invalid response."
                );
            },
            Qt::QueuedConnection
        );
    }
}

void NetworkClient::sendJson(
    const nlohmann::json& message)
{
    if (!connectedState)
    {
        emit connectionError(
            "The application is not connected to the server."
        );
        return;
    }

    std::string text = message.dump() + "\n";

    boost::asio::post(
        ioContext,
        [this, text]()
        {
            bool writeInProgress =
                !outgoingMessages.empty();

            outgoingMessages.push_back(text);

            if (!writeInProgress)
            {
                startWriting();
            }
        }
    );
}

void NetworkClient::startWriting()
{
    boost::asio::async_write(
        socket,
        boost::asio::buffer(
            outgoingMessages.front()
        ),
        [this](
            const boost::system::error_code& error,
            std::size_t)
        {
            if (error)
            {
                closeConnection();

                QMetaObject::invokeMethod(
                    this,
                    [this]()
                    {
                        emit disconnected();
                    },
                    Qt::QueuedConnection
                );

                return;
            }

            outgoingMessages.pop_front();

            if (!outgoingMessages.empty())
            {
                startWriting();
            }
        }
    );
}

void NetworkClient::closeConnection()
{
    if (!connectedState)
    {
        return;
    }

    connectedState = false;

    boost::system::error_code error;

    socket.shutdown(
        tcp::socket::shutdown_both,
        error
    );

    socket.close(error);
}
