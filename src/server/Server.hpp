#ifndef SERVER_HPP
#define SERVER_HPP
#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include "../models/ReportManager.hpp"
#include "../models/UserManager.hpp"

using boost::asio::ip::tcp;

class Server; // declared below, ConnectedClient needs to know it exists

// ConnectedClient object represents one running client app connected to us.
class ConnectedClient : public std::enable_shared_from_this<ConnectedClient>
{
public:
    ConnectedClient(tcp::socket socket, Server& server);

    void start();
    void send(const std::string& jsonLine);

private:
    void listenForMessages();
    tcp::socket clientSocket;
    Server& parentServer;
    boost::asio::streambuf incomingData;
};

class Server
{
public:
    Server(boost::asio::io_context& io, unsigned short port);

    void addClient(std::shared_ptr<ConnectedClient> client);
    void removeClient(std::shared_ptr<ConnectedClient> client);
    void handleMessage(std::shared_ptr<ConnectedClient> client, const std::string& line);

private:
    void waitForNextConnection();
    void sendReportsToEveryone();

    tcp::acceptor connectionListener;
    std::vector<std::shared_ptr<ConnectedClient>> connectedClients;

    ReportManager reportManager;
    UserManager   userManager;
};

#endif
