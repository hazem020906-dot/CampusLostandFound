#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include "../models/UserManager.hpp"

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
  explicit  LoginWindow(QWidget* parent = nullptr);//constructor building the login screen parent is null to be able to create standalone widget as window

private:
    UserManager userManager;

    QLineEdit*  usernameInput;
    QLineEdit*  passwordInput;
    QComboBox*  roleBox;        // used only during registration
    QPushButton* loginButton;
    QPushButton* registerButton;
    QLabel*     feedbackLabel;  // shows success or error messages to the user like username taken or password is wrong etc

    void openDashboard(const User& user);// declaration to open the correct dashboard for students/security respectively
};

#endif
