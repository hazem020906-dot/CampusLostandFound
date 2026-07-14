#include <QApplication>

#include "ui/LoginWindow.hpp"
#include "client/NetworkClient.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    NetworkClient networkClient;

    LoginWindow loginWindow(&networkClient);
    loginWindow.show();

    networkClient.connectToServer(
        "127.0.0.1",
        5555
    );

    return app.exec();
}
