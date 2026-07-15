#ifndef NETWORKCLIENT_HPP
#define NETWORKCLIENT_HPP

#include <QObject>
#include <QString>

#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

#include <atomic>
#include <deque>
#include <string>
#include <thread>

class NetworkClient : public QObject
{
    Q_OBJECT

public:
    explicit NetworkClient(QObject* parent = nullptr);
    ~NetworkClient();

    void connectToServer(
        const QString& host = "127.0.0.1",
        unsigned short port = 5555
    );

    void disconnectFromServer();

    void login(
        const QString& username,
        const QString& password
    );

    void registerUser(
        const QString& username,
        const QString& password,
        const QString& role
    );

    void submitReport(
        const QString& itemName,
        const QString& category,
        const QString& location,
        const QString& date,
        const QString& reportType,
        const QString& imageData = ""
    );

    void updateStatus(
        int reportIndex,
        const QString& newStatus
    );

    void requestAllReports();

    bool isConnected() const;

signals:
    void connected();
    void disconnected();

    void connectionError(const QString& message);
    void serverError(const QString& message);

    void loginResult(
        bool success,
        const QString& username,
        const QString& role
    );

    void registerResult(
        bool success,
        const QString& message
    );

    void reportsSnapshot(const QString& reportsJson);
    void notificationReceived(const QString& message);

private:
    using tcp = boost::asio::ip::tcp;

    void startReading();
    void handleMessage(const std::string& jsonLine);

    void sendJson(const nlohmann::json& message);
    void startWriting();

    void closeConnection();

    boost::asio::io_context ioContext;
    tcp::resolver resolver;
    tcp::socket socket;
    boost::asio::streambuf incomingBuffer;

    std::thread networkThread;
    std::deque<std::string> outgoingMessages;

    std::atomic<bool> connectedState;
};

#endif
