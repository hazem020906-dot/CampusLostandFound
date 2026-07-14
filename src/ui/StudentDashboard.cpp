#include "StudentDashboard.hpp"
#include "FeedbackWindow.hpp"
#include "MessagingWindow.hpp"
#include "StatisticsWindow.hpp"
#include "RewardWindow.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QAbstractItemView>

#include <nlohmann/json.hpp>

StudentDashboard::StudentDashboard(
    const User& user,
    NetworkClient* client,
    QWidget* parent
)
    : QMainWindow(parent),
      networkClient(client),
      currentUser(user)
{
    setWindowTitle("Student Dashboard - " + user.username);
    resize(800, 600);

    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout();

    // Header
    layout->addWidget(
        new QLabel(
            "Welcome, " + user.username + "  Student Dashboard"
        )
    );

    // Feature navigation
    feedbackButton = new QPushButton("Feedback");
    messagesButton = new QPushButton("Messages");
    statsButton = new QPushButton("Statistics");
    rewardsButton = new QPushButton("Rewards");

    QHBoxLayout* navRow = new QHBoxLayout();
    navRow->addWidget(feedbackButton);
    navRow->addWidget(messagesButton);
    navRow->addWidget(statsButton);
    navRow->addWidget(rewardsButton);
    layout->addLayout(navRow);

    connect(
        feedbackButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            FeedbackWindow* window =
                new FeedbackWindow(currentUser, this);

            window->setAttribute(Qt::WA_DeleteOnClose);
            window->show();
        }
    );

    connect(
        messagesButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            MessagingWindow* window =
                new MessagingWindow(currentUser, this);

            window->setAttribute(Qt::WA_DeleteOnClose);
            window->show();
        }
    );

    connect(
        statsButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            StatisticsWindow* window =
                new StatisticsWindow(this);

            window->setAttribute(Qt::WA_DeleteOnClose);
            window->show();
        }
    );

    connect(
        rewardsButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            RewardWindow* window =
                new RewardWindow(currentUser, this);

            window->setAttribute(Qt::WA_DeleteOnClose);
            window->show();
        }
    );

    layout->addSpacing(8);

    // Report submission form
    layout->addWidget(
        new QLabel("Submit a Lost or Found Item Report")
    );

    nameInput = new QLineEdit();
    nameInput->setPlaceholderText("Item name");

    categoryInput = new QLineEdit();
    categoryInput->setPlaceholderText("Category");

    locationInput = new QLineEdit();
    locationInput->setPlaceholderText("Location");

    dateInput = new QLineEdit();
    dateInput->setPlaceholderText("Date");

    reportTypeBox = new QComboBox();
    reportTypeBox->addItems({"Lost", "Found"});

    submitButton = new QPushButton("Submit Report");

    layout->addWidget(nameInput);
    layout->addWidget(categoryInput);
    layout->addWidget(locationInput);
    layout->addWidget(dateInput);
    layout->addWidget(new QLabel("Report type:"));
    layout->addWidget(reportTypeBox);
    layout->addWidget(submitButton);
    layout->addSpacing(10);

    // Filter controls
    layout->addWidget(new QLabel("Filter Reports"));

    QHBoxLayout* filterRow = new QHBoxLayout();

    filterTypeBox = new QComboBox();
    filterTypeBox->addItems({
        "Category",
        "Date",
        "Location",
        "Status"
    });

    filterValueInput = new QLineEdit();
    filterValueInput->setPlaceholderText("Filter value");

    filterStatusBox = new QComboBox();
    filterStatusBox->addItems({
        "Lost",
        "Found",
        "Claimed",
        "Returned"
    });

    applyFilterButton = new QPushButton("Apply Filter");
    clearFilterButton = new QPushButton("Clear Filter");

    filterRow->addWidget(new QLabel("Filter by:"));
    filterRow->addWidget(filterTypeBox);
    filterRow->addWidget(filterValueInput);
    filterRow->addWidget(new QLabel("or Status:"));
    filterRow->addWidget(filterStatusBox);
    filterRow->addWidget(applyFilterButton);
    filterRow->addWidget(clearFilterButton);

    layout->addLayout(filterRow);
    layout->addSpacing(6);

    // Reports table
    layout->addWidget(new QLabel("All Reports"));

    table = new QTableWidget();
    table->setSelectionBehavior(
        QAbstractItemView::SelectRows
    );
    table->setSelectionMode(
        QAbstractItemView::SingleSelection
    );
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({
        "Name",
        "Category",
        "Location",
        "Date",
        "Status"
    });

    layout->addWidget(table);

    central->setLayout(layout);
    setCentralWidget(central);

    // Submit report through the server
    connect(
        submitButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            if (
                nameInput->text().isEmpty() ||
                categoryInput->text().isEmpty() ||
                locationInput->text().isEmpty() ||
                dateInput->text().isEmpty()
            )
            {
                QMessageBox::warning(
                    this,
                    "Error",
                    "Please fill in all fields."
                );
                return;
            }

            networkClient->submitReport(
                nameInput->text(),
                categoryInput->text(),
                locationInput->text(),
                dateInput->text(),
                reportTypeBox->currentText()
            );

            nameInput->clear();
            categoryInput->clear();
            locationInput->clear();
            dateInput->clear();
        }
    );

    // Receive report snapshots from the server
    connect(
        networkClient,
        &NetworkClient::reportsSnapshot,
        this,
        [this](const QString& reportsJson)
        {
            try
            {
                nlohmann::json reports =
                    nlohmann::json::parse(
                        reportsJson.toStdString()
                    );

                QVector<ItemReport> receivedReports;

                for (const auto& reportJson : reports)
                {
                    ItemReport report;

                    report.itemName =
                        QString::fromStdString(
                            reportJson.value(
                                "itemName",
                                ""
                            )
                        );

                    report.category =
                        QString::fromStdString(
                            reportJson.value(
                                "category",
                                ""
                            )
                        );

                    report.location =
                        QString::fromStdString(
                            reportJson.value(
                                "location",
                                ""
                            )
                        );

                    report.date =
                        QString::fromStdString(
                            reportJson.value(
                                "date",
                                ""
                            )
                        );

                    report.status =
                        QString::fromStdString(
                            reportJson.value(
                                "status",
                                ""
                            )
                        );

                    receivedReports.push_back(report);
                }

                currentReports = receivedReports;
                refreshTable(currentReports);
            }
            catch (const nlohmann::json::exception&)
            {
                QMessageBox::warning(
                    this,
                    "Error",
                    "The report data received from the server was invalid."
                );
            }
        }
    );

    // Matching notification
    connect(
        networkClient,
        &NetworkClient::notificationReceived,
        this,
        [this](const QString& message)
        {
            QMessageBox::information(
                this,
                "Potential Match",
                message
            );
        }
    );

    // Server error
    connect(
        networkClient,
        &NetworkClient::serverError,
        this,
        [this](const QString& message)
        {
            QMessageBox::warning(
                this,
                "Server Error",
                message
            );
        }
    );

    // Disconnection feedback
    connect(
        networkClient,
        &NetworkClient::disconnected,
        this,
        [this]()
        {
            QMessageBox::warning(
                this,
                "Connection Lost",
                "The connection to the server was lost."
            );

            submitButton->setEnabled(false);
        }
    );

    // Apply local filter to the latest server snapshot
    connect(
        applyFilterButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            QString filterType =
                filterTypeBox->currentText();

            QString filterValue =
                filterValueInput->text();

            QVector<ItemReport> filteredReports;

            if (
                filterType != "Status" &&
                filterValue.isEmpty()
            )
            {
                QMessageBox::warning(
                    this,
                    "Error",
                    "Enter a value to filter by."
                );
                return;
            }

            for (const ItemReport& report : currentReports)
            {
                bool matches = false;

                if (filterType == "Category")
                {
                    matches =
                        report.category.contains(
                            filterValue,
                            Qt::CaseInsensitive
                        );
                }
                else if (filterType == "Date")
                {
                    matches =
                        report.date.contains(
                            filterValue,
                            Qt::CaseInsensitive
                        );
                }
                else if (filterType == "Location")
                {
                    matches =
                        report.location.contains(
                            filterValue,
                            Qt::CaseInsensitive
                        );
                }
                else if (filterType == "Status")
                {
                    matches =
                        report.status.compare(
                            filterStatusBox->currentText(),
                            Qt::CaseInsensitive
                        ) == 0;
                }

                if (matches)
                {
                    filteredReports.push_back(report);
                }
            }

            refreshTable(filteredReports);
        }
    );

    connect(
        clearFilterButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            filterValueInput->clear();
            refreshTable(currentReports);
        }
    );

    // Ask the server for the current report list
    networkClient->requestAllReports();
}

void StudentDashboard::refreshTable(
    const QVector<ItemReport>& reports
)
{
    table->setRowCount(reports.size());

    for (int i = 0; i < reports.size(); i++)
    {
        table->setItem(
            i,
            0,
            new QTableWidgetItem(
                reports[i].itemName
            )
        );

        table->setItem(
            i,
            1,
            new QTableWidgetItem(
                reports[i].category
            )
        );

        table->setItem(
            i,
            2,
            new QTableWidgetItem(
                reports[i].location
            )
        );

        table->setItem(
            i,
            3,
            new QTableWidgetItem(
                reports[i].date
            )
        );

        table->setItem(
            i,
            4,
            new QTableWidgetItem(
                reports[i].status
            )
        );
    }
}
