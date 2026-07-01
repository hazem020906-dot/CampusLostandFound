#include "MainWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QLabel>
#include <QTableWidgetItem>
#include <QAbstractItemView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(new QLabel("Submit Lost Item Report"));

    nameInput = new QLineEdit();
    nameInput->setPlaceholderText("Item name");

    categoryInput = new QLineEdit();
    categoryInput->setPlaceholderText("Category");

    locationInput = new QLineEdit();
    locationInput->setPlaceholderText("Location");

    dateInput = new QLineEdit();
    dateInput->setPlaceholderText("Date");

    addButton = new QPushButton("Submit Lost Item");

    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Search reports");

    searchButton = new QPushButton("Search");
    showAllButton = new QPushButton("Show All");

    statusBox = new QComboBox();
    statusBox->addItems({"Lost", "Found", "Claimed", "Returned"});

    updateStatusButton = new QPushButton("Update Status");

    table = new QTableWidget();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels(
        {"Name","Category","Location","Date","Status"});

    layout->addWidget(nameInput);
    layout->addWidget(categoryInput);
    layout->addWidget(locationInput);
    layout->addWidget(dateInput);
    layout->addWidget(addButton);

    layout->addWidget(new QLabel("Search Reports"));
    layout->addWidget(searchInput);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(showAllButton);
    layout->addLayout(searchLayout);

    layout->addWidget(new QLabel("All Active Reports"));
    layout->addWidget(table);

    QHBoxLayout *statusLayout = new QHBoxLayout();
    statusLayout->addWidget(statusBox);
    statusLayout->addWidget(updateStatusButton);
    layout->addLayout(statusLayout);

    central->setLayout(layout);
    setCentralWidget(central);

    // Submit Lost Item
    connect(addButton, &QPushButton::clicked, this, [this]() {

        if(nameInput->text().isEmpty() ||
           categoryInput->text().isEmpty() ||
           locationInput->text().isEmpty() ||
           dateInput->text().isEmpty())
        {
            QMessageBox::warning(this,
                                 "Error",
                                 "Please fill in all fields.");
            return;
        }

        manager.addLostItem(
            nameInput->text(),
            categoryInput->text(),
            locationInput->text(),
            dateInput->text());

        nameInput->clear();
        categoryInput->clear();
        locationInput->clear();
        dateInput->clear();

        refreshTable(manager.getAllReports());

        QMessageBox::information(this,
                                 "Success",
                                 "Lost item submitted successfully.");
    });

    // Search
    connect(searchButton, &QPushButton::clicked, this, [this]() {

        refreshTable(manager.searchReports(searchInput->text()));

    });

    // Show All
    connect(showAllButton, &QPushButton::clicked, this, [this]() {

        searchInput->clear();
        refreshTable(manager.getAllReports());

    });

    // Update Status
    connect(updateStatusButton, &QPushButton::clicked, this, [this]() {

        int row = table->currentRow();

        if(row < 0)
        {
            QMessageBox::warning(this,
                                 "Error",
                                 "Please select a report.");
            return;
        }

        if(manager.updateStatus(row, statusBox->currentText()))
        {
            refreshTable(manager.getAllReports());

            QMessageBox::information(this,
                                     "Success",
                                     "Status updated.");
        }
        else
        {
            QMessageBox::warning(this,
                                 "Error",
                                 "Unable to update status.");
        }

    });

    refreshTable(manager.getAllReports());
}

void MainWindow::refreshTable(const QVector<ItemReport>& reports)
{
    table->setRowCount(reports.size());

    for(int i = 0; i < reports.size(); i++)
    {
        table->setItem(i,0,new QTableWidgetItem(reports[i].itemName));
        table->setItem(i,1,new QTableWidgetItem(reports[i].category));
        table->setItem(i,2,new QTableWidgetItem(reports[i].location));
        table->setItem(i,3,new QTableWidgetItem(reports[i].date));
        table->setItem(i,4,new QTableWidgetItem(reports[i].status));
    }
}
