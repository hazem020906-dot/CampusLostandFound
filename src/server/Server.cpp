#include "Server.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>


ConnectedClient::ConnectedClient(tcp::socket socket, Server& server)
    : clientSocket(std::move(socket)), parentServer(server)
{
}

void ConnectedClient::start()
{
    parentServer.addClient(shared_from_this());
    listenForMessages();
}

void ConnectedClient::send(const std::string& jsonLine)
{
    auto self = shared_from_this();
    auto messageToSend = std::make_shared<std::string>(jsonLine + "\n");
    boost::asio::async_write(clientSocket, boost::asio::buffer(*messageToSend),
        [self, messageToSend](boost::system::error_code, std::size_t) {});
}

void ConnectedClient::listenForMessages()
{
    auto self = shared_from_this();
    boost::asio::async_read_until(clientSocket, incomingData, '\n',
        [this, self](boost::system::error_code error, std::size_t)
        {
            if (error)
            {
                parentServer.removeClient(self);
                return;
            }

            std::istream messageStream(&incomingData);
            std::string oneLine;
            std::getline(messageStream, oneLine);

            try
            {
                parentServer.handleMessage(self, oneLine);
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error handling message: " << e.what() << std::endl;
            }

            listenForMessages();
        });
}

//  Server
Server::Server(boost::asio::io_context& io, unsigned short port)
    : connectionListener(io, tcp::endpoint(tcp::v4(), port))
{
    std::cout << "Server listening " << port << std::endl;
    waitForNextConnection();
}

void Server::waitForNextConnection()
{
    connectionListener.async_accept(
        [this](boost::system::error_code error, tcp::socket socket)
        {
            if (!error)
            {
                auto newClient = std::make_shared<ConnectedClient>(std::move(socket), *this);
                newClient->start();
            }
            waitForNextConnection();
        });
}

void Server::addClient(std::shared_ptr<ConnectedClient> client)
{
    connectedClients.push_back(client);
    std::cout << "Client connected. Total: " << connectedClients.size() << std::endl;
    sendReportsToEveryone();
}

void Server::removeClient(std::shared_ptr<ConnectedClient> client)
{
    for (size_t i = 0; i < connectedClients.size(); i++)
    {
        if (connectedClients[i] == client)
        {
            connectedClients.erase(connectedClients.begin() + i);
            break;
        }
    }
    std::cout << "Client disconnected. Total: " << connectedClients.size() << std::endl;
}

void Server::sendReportsToEveryone()
{
    QJsonObject message;
    message["type"] = "reports_snapshot";

    QJsonArray reportList;
    for (const ItemReport& report : reportManager.getAllReports())
    {
        QJsonObject oneReport;
        oneReport["itemName"] = report.itemName;
        oneReport["category"] = report.category;
        oneReport["location"] = report.location;
        oneReport["date"]     = report.date;
        oneReport["status"]   = report.status;
        reportList.append(oneReport);
    }
    message["reports"] = reportList;

    std::string line = QJsonDocument(message).toJson(QJsonDocument::Compact).toStdString();
    for (auto& client : connectedClients)
    {
        client->send(line);
    }
}

void Server::handleMessage(std::shared_ptr<ConnectedClient> client, const std::string& line)
{
    QJsonDocument document = QJsonDocument::fromJson(QByteArray::fromStdString(line));
    if (!document.isObject())
    {
        return;
    }

    QJsonObject message = document.object();
    QString messageType = message["type"].toString();

    if (messageType == "login")
    {
        User loggedInUser;
        bool success = userManager.login(message["username"].toString(), message["password"].toString(), loggedInUser);

        QJsonObject response;
        response["type"] = "login_result";
        response["success"] = success;
        response["username"] = loggedInUser.username;
        response["role"] = loggedInUser.role;
        client->send(QJsonDocument(response).toJson(QJsonDocument::Compact).toStdString());
    }
    else if (messageType == "register")
    {
        bool success = userManager.registerUser(message["username"].toString(), message["password"].toString(), message["role"].toString());

        QJsonObject response;
        response["type"] = "register_result";
        response["success"] = success;
        client->send(QJsonDocument(response).toJson(QJsonDocument::Compact).toStdString());
    }
    else if (messageType == "submit_report")
    {
        reportManager.addLostItem(message["itemName"].toString(), message["category"].toString(),
                                   message["location"].toString(), message["date"].toString());

        QVector<ItemReport> allReports = reportManager.getAllReports();
        if (!allReports.isEmpty())
        {
            reportManager.updateStatus(allReports.size() - 1, message["reportType"].toString());
        }
        sendReportsToEveryone();
    }
    else if (messageType == "update_status")
    {
        reportManager.updateStatus(message["reportIndex"].toInt(), message["newStatus"].toString());
        sendReportsToEveryone();
    }
    else if (messageType == "get_all_reports")
    {
        sendReportsToEveryone();
    }
}
