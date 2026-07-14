#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

#include "../models/User.hpp"
#include "../client/NetworkClient.hpp"

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(
        NetworkClient* networkClient,
        QWidget* parent = nullptr
    );

private:
    NetworkClient* networkClient;

    QLineEdit* usernameInput;
    QLineEdit* passwordInput;
    QComboBox* roleBox;
    QPushButton* loginButton;
    QPushButton* registerButton;
    QLabel* feedbackLabel;

    void openDashboard(const User& user);
};

#endif
