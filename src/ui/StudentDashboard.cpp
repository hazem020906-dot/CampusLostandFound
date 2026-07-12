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

StudentDashboard::StudentDashboard(const User& user, QWidget* parent)
    : QMainWindow(parent), currentUser(user)//constructor
{
    setWindowTitle("Student Dashboard - " + user.username);
    resize(800, 600);

    QWidget*     central = new QWidget(this);
    QVBoxLayout* layout  = new QVBoxLayout();

    //  Header
    layout->addWidget(new QLabel("Welcome, " + user.username + "  Student Dashboard"));
    //  Feature navigation
    feedbackButton = new QPushButton("Feedback");
    messagesButton = new QPushButton("Messages");
    statsButton    = new QPushButton("Statistics");
    rewardsButton  = new QPushButton("Rewards");

    QHBoxLayout* navRow = new QHBoxLayout();
    navRow->addWidget(feedbackButton);
    navRow->addWidget(messagesButton);
    navRow->addWidget(statsButton);
    navRow->addWidget(rewardsButton);
    layout->addLayout(navRow);

    connect(feedbackButton, &QPushButton::clicked, this, [this]() {
        (new FeedbackWindow(currentUser))->show();
    });
    connect(messagesButton, &QPushButton::clicked, this, [this]() {
        (new MessagingWindow(currentUser))->show();
    });
    connect(statsButton, &QPushButton::clicked, this, [this]() {
        (new StatisticsWindow())->show();
    });
    connect(rewardsButton, &QPushButton::clicked, this, [this]() {
        (new RewardWindow(currentUser))->show();
    });
    layout->addSpacing(8);

    // ── Report submission form layout code
    layout->addWidget(new QLabel("Submit a Lost or Found Item Report"));

    nameInput = new QLineEdit();
    nameInput->setPlaceholderText("Item name");

    categoryInput = new QLineEdit();
    categoryInput->setPlaceholderText("Category");

    locationInput = new QLineEdit();
    locationInput->setPlaceholderText("Location where you found the item");

    dateInput = new QLineEdit();
    dateInput->setPlaceholderText("Date");

    reportTypeBox = new QComboBox();
    reportTypeBox->addItems({"Lost", "Found"});

    submitButton = new QPushButton("Submit Report");

    layout->addWidget(nameInput);
    layout->addWidget(categoryInput);
    layout->addWidget(locationInput);
    layout->addWidget(dateInput);
    layout->addWidget(new QLabel("Report type:"));//header thing for reporttypebox
    layout->addWidget(reportTypeBox);
    layout->addWidget(submitButton);
    layout->addSpacing(10);

    // ── Filter controls
    layout->addWidget(new QLabel("Filter Reports"));

    QHBoxLayout* filterRow = new QHBoxLayout();

    filterTypeBox = new QComboBox();
    filterTypeBox->addItems({"Category", "Date", "Location", "Status"});

    filterValueInput = new QLineEdit();
    filterValueInput->setPlaceholderText("Filter value");//user types here filtering by value

    // Status box only shown when filtering by status
    filterStatusBox = new QComboBox();
    filterStatusBox->addItems({"Lost", "Found", "Claimed", "Returned"});

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

    // ── Table to show all repports on the app the UI
    layout->addWidget(new QLabel("All Reports"));

    table = new QTableWidget();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"Name", "Category", "Location", "Date", "Status"});
    layout->addWidget(table);

    central->setLayout(layout);
    setCentralWidget(central);

    refreshTable(manager.getAllReports());

    // ── Connections signakls and slots

    connect(submitButton, &QPushButton::clicked, this, [this]()
    {
        if (nameInput->text().isEmpty()     ||
            categoryInput->text().isEmpty() ||
            locationInput->text().isEmpty() ||
            dateInput->text().isEmpty())
        {
            QMessageBox::warning(this, "Error", "Please fill in all fields.");
            return;
        }

        // addLostItem works for both lost and found
        manager.addLostItem(
            nameInput->text(),
            categoryInput->text(),
            locationInput->text(),
            dateInput->text()
        );

        QVector<ItemReport> all = manager.getAllReports();
        if (!all.isEmpty())
        {
            // Update the last added report's status to match the selected type
            manager.updateStatus(all.size() - 1, reportTypeBox->currentText());
        }

        nameInput->clear();
        categoryInput->clear();
        locationInput->clear();
        dateInput->clear();

        refreshTable(manager.getAllReports());
        QMessageBox::information(this, "Success", "Report submitted successfully.");
    });

    connect(applyFilterButton, &QPushButton::clicked, this, [this]()
    {
        QString filterType = filterTypeBox->currentText();

        if (filterType == "Category")
        {
            if (filterValueInput->text().isEmpty())
            {
                QMessageBox::warning(this, "Error", "Enter a category to filter by.");
                return;
            }
            refreshTable(manager.filterByCategory(filterValueInput->text()));
        }
        else if (filterType == "Date")
        {
            if (filterValueInput->text().isEmpty())
            {
                QMessageBox::warning(this, "Error", "Enter a date to filter by.");
                return;
            }
            refreshTable(manager.filterByDate(filterValueInput->text()));
        }
        else if (filterType == "Location")
        {
            if (filterValueInput->text().isEmpty())
            {
                QMessageBox::warning(this, "Error", "Enter a location to filter by.");
                return;
            }
            refreshTable(manager.filterByLocation(filterValueInput->text()));
        }
        else if (filterType == "Status")
        {
            refreshTable(manager.filterByStatus(filterStatusBox->currentText()));
        }
    });

    connect(clearFilterButton, &QPushButton::clicked, this, [this]()
    {
        filterValueInput->clear();
        refreshTable(manager.getAllReports());
    });
}

void StudentDashboard::refreshTable(const QVector<ItemReport>& reports)
{
    table->setRowCount(reports.size());
    for (int i = 0; i < reports.size(); i++)
    {
        table->setItem(i, 0, new QTableWidgetItem(reports[i].itemName));
        table->setItem(i, 1, new QTableWidgetItem(reports[i].category));
        table->setItem(i, 2, new QTableWidgetItem(reports[i].location));
        table->setItem(i, 3, new QTableWidgetItem(reports[i].date));
        table->setItem(i, 4, new QTableWidgetItem(reports[i].status));
    }
}
