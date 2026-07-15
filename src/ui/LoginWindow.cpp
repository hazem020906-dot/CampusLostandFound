#include "LoginWindow.hpp"
#include "StudentDashboard.hpp"
#include "Gate4Dashoboard.hpp"

#include <QVBoxLayout>
#include <QWidget>

LoginWindow::LoginWindow(
    NetworkClient* client,
    QWidget* parent
)
    : QMainWindow(parent),
      networkClient(client)
{
    setWindowTitle("AUC Lost & Found - Login");
    resize(400, 300);

    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(new QLabel("AUC Campus Lost & Found"));
    layout->addSpacing(10);

    usernameInput = new QLineEdit();
    usernameInput->setPlaceholderText("Username");

    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Password");
    passwordInput->setEchoMode(QLineEdit::Password);

    roleBox = new QComboBox();
    roleBox->addItems({"Student", "Security"});

    loginButton = new QPushButton("Login");
    registerButton = new QPushButton("Register New Account");

    feedbackLabel = new QLabel("Connecting to server...");
    feedbackLabel->setWordWrap(true);

    layout->addWidget(new QLabel("Username:"));
    layout->addWidget(usernameInput);

    layout->addWidget(new QLabel("Password:"));
    layout->addWidget(passwordInput);

    layout->addWidget(
        new QLabel("Role (for registration only):")
    );
    layout->addWidget(roleBox);

    layout->addWidget(loginButton);
    layout->addWidget(registerButton);
    layout->addWidget(feedbackLabel);

    central->setLayout(layout);
    setCentralWidget(central);

    loginButton->setEnabled(false);
    registerButton->setEnabled(false);

    connect(
        networkClient,
        &NetworkClient::connected,
        this,
        [this]()
        {
            feedbackLabel->setText("Connected to server.");
            loginButton->setEnabled(true);
            registerButton->setEnabled(true);
        }
    );

    connect(
        networkClient,
        &NetworkClient::disconnected,
        this,
        [this]()
        {
            feedbackLabel->setText("Disconnected from server.");
            loginButton->setEnabled(false);
            registerButton->setEnabled(false);
        }
    );

    connect(
        networkClient,
        &NetworkClient::connectionError,
        this,
        [this](const QString& message)
        {
            feedbackLabel->setText(message);
            loginButton->setEnabled(false);
            registerButton->setEnabled(false);
        }
    );

    connect(
        networkClient,
        &NetworkClient::serverError,
        this,
        [this](const QString& message)
        {
            feedbackLabel->setText(message);
        }
    );

    connect(
        loginButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            if (
                usernameInput->text().isEmpty() ||
                passwordInput->text().isEmpty()
            )
            {
                feedbackLabel->setText(
                    "Please enter both username and password."
                );
                return;
            }

            feedbackLabel->setText("Logging in...");

            this->networkClient->login(
                usernameInput->text(),
                passwordInput->text()
            );
        }
    );

    connect(
        networkClient,
        &NetworkClient::loginResult,
        this,
        [this](
            bool success,
            const QString& username,
            const QString& role
        )
        {
            if (!success)
            {
                feedbackLabel->setText(
                    "Invalid username or password."
                );
                return;
            }

            User loggedInUser;
            loggedInUser.username = username;
            loggedInUser.role = role;

            feedbackLabel->setText("Login successful.");
            openDashboard(loggedInUser);
        }
    );

    connect(
        registerButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            if (
                usernameInput->text().isEmpty() ||
                passwordInput->text().isEmpty()
            )
            {
                feedbackLabel->setText(
                    "Please fill in username and password."
                );
                return;
            }

            feedbackLabel->setText("Creating account...");

            this->networkClient->registerUser(
                usernameInput->text(),
                passwordInput->text(),
                roleBox->currentText()
            );
        }
    );

    connect(
        networkClient,
        &NetworkClient::registerResult,
        this,
        [this](bool success, const QString& message)
        {
            feedbackLabel->setText(message);

            if (success)
            {
                usernameInput->clear();
                passwordInput->clear();
            }
        }
    );
}

void LoginWindow::openDashboard(const User& user)
{
    if (user.role == "Security")
    {
        Gate4Dashboard* dashboard =
            new Gate4Dashboard(user, networkClient);
        dashboard->show();
    }
    else
    {
        StudentDashboard* dashboard =
            new StudentDashboard(user, networkClient);

        dashboard->show();
    }

    hide();
}
