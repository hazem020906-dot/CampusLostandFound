#include "LoginWindow.hpp"
#include "StudentDashboard.hpp"
#include "Gate4Dashoboard.hpp"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QWidget>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("AUC Lost & Found - Login");
    resize(400, 300);

    QWidget*     central = new QWidget(this);
    QVBoxLayout* layout  = new QVBoxLayout();

    layout->addWidget(new QLabel("AUC Campus Lost & Found"));
    layout->addSpacing(10);

    usernameInput = new QLineEdit();
    usernameInput->setPlaceholderText("Username");

    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Password");
    passwordInput->setEchoMode(QLineEdit::Password);

    roleBox = new QComboBox();
    roleBox->addItems({"Student", "Security"});

    loginButton    = new QPushButton("Login");
    registerButton = new QPushButton("Register New Account");

    feedbackLabel = new QLabel("");
    feedbackLabel->setWordWrap(true);

    layout->addWidget(new QLabel("Username:"));
    layout->addWidget(usernameInput);
    layout->addWidget(new QLabel("Password:"));
    layout->addWidget(passwordInput);
    layout->addWidget(new QLabel("Role (for registration only):"));
    layout->addWidget(roleBox);
    layout->addWidget(loginButton);
    layout->addWidget(registerButton);
    layout->addWidget(feedbackLabel);

    central->setLayout(layout);
    setCentralWidget(central);

    // Login
    connect(loginButton, &QPushButton::clicked, this, [this]()
    {
        if (usernameInput->text().isEmpty() || passwordInput->text().isEmpty())
        {
            feedbackLabel->setText("Please enter both username and password.");
            return;
        }

        User loggedInUser;
        bool success = userManager.login(
            usernameInput->text(),
            passwordInput->text(),
            loggedInUser
        );

        if (!success)
        {
            feedbackLabel->setText("Invalid username or password. Please try again.");
            return;
        }

        openDashboard(loggedInUser);
    });

    // Register
    connect(registerButton, &QPushButton::clicked, this, [this]()
    {
        if (usernameInput->text().isEmpty() || passwordInput->text().isEmpty())
        {
            feedbackLabel->setText("Please fill in username and password to register.");
            return;
        }

        bool success = userManager.registerUser(
            usernameInput->text(),
            passwordInput->text(),
            roleBox->currentText()
        );

        if (!success)
        {
            feedbackLabel->setText("Username already taken. Please choose a different one.");
            return;
        }

        feedbackLabel->setText("Account created successfully. You can now log in.");
        usernameInput->clear();
        passwordInput->clear();
    });
}

void LoginWindow::openDashboard(const User& user)
{
    if (user.role == "Security")
    {
        Gate4Dashboard* dashboard = new Gate4Dashboard(user);
        dashboard->show();
    }
    else
    {
        StudentDashboard* dashboard = new StudentDashboard(user);
        dashboard->show();
    }

    this->hide(); // hide the login window, not close, in case they log out later
}
