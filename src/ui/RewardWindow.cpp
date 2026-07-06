#include "RewardWindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QDate>

RewardWindow::RewardWindow(const User& user, QWidget* parent)
    : QMainWindow(parent), currentUser(user)
{
    setWindowTitle("Thank You Notes - " + user.username);
    resize(800, 600);

    QWidget*     central = new QWidget(this);
    QVBoxLayout* layout  = new QVBoxLayout();

    layout->addWidget(new QLabel("Leave a Thank You Note for a Returned Item"));
    layout->addSpacing(8);

    itemNameInput = new QLineEdit();
    itemNameInput->setPlaceholderText("Item name");

    messageInput = new QTextEdit();
    messageInput->setPlaceholderText("Write your thank-you message here...");
    messageInput->setFixedHeight(80);

    contactInput = new QLineEdit();
    contactInput->setPlaceholderText("Contact info (optional)");

    addButton = new QPushButton("Send Thank You");

    layout->addWidget(itemNameInput);
    layout->addWidget(messageInput);
    layout->addWidget(contactInput);
    layout->addWidget(addButton);
    layout->addSpacing(10);

    layout->addWidget(new QLabel("Search Thank You Notes"));

    QHBoxLayout* searchRow = new QHBoxLayout();
    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Search by item name");
    searchButton  = new QPushButton("Search");
    showAllButton = new QPushButton("Show All");

    searchRow->addWidget(searchInput);
    searchRow->addWidget(searchButton);
    searchRow->addWidget(showAllButton);
    layout->addLayout(searchRow);
    layout->addSpacing(6);

    layout->addWidget(new QLabel("All Thank You Notes"));

    table = new QTableWidget();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"Item", "Message", "Contact", "From", "Date"});
    layout->addWidget(table);

    removeButton = new QPushButton("Remove Selected Note");
    layout->addWidget(removeButton);

    central->setLayout(layout);
    setCentralWidget(central);

    refreshTable(manager.getAllRewards());

    connect(addButton, &QPushButton::clicked, this, [this]()
    {
        if (itemNameInput->text().isEmpty() || messageInput->toPlainText().isEmpty())
        {
            QMessageBox::warning(this, "Error", "Please fill in the item name and message.");
            return;
        }

        bool success = manager.addReward(
            itemNameInput->text(),
            messageInput->toPlainText(),
            contactInput->text(),
            currentUser.username,
            QDate::currentDate().toString("yyyy-MM-dd")
        );

        if (!success)
        {
            QMessageBox::warning(this, "Error", "Could not add thank you note.");
            return;
        }

        itemNameInput->clear();
        messageInput->clear();
        contactInput->clear();

        refreshTable(manager.getAllRewards());
        QMessageBox::information(this, "Success", "Thank you note sent successfully.");
    });

    connect(searchButton, &QPushButton::clicked, this, [this]()
    {
        refreshTable(manager.searchByItemName(searchInput->text()));
    });

    connect(showAllButton, &QPushButton::clicked, this, [this]()
    {
        searchInput->clear();
        refreshTable(manager.getAllRewards());
    });

    connect(removeButton, &QPushButton::clicked, this, [this]()
    {
        int row = table->currentRow();

        if (row < 0)
        {
            QMessageBox::warning(this, "No selection", "Please select a note to remove.");
            return;
        }

        if (manager.removeReward(row))
        {
            refreshTable(manager.getAllRewards());
        }
        else
        {
            QMessageBox::warning(this, "Error", "Could not remove note.");
        }
    });
}

void RewardWindow::refreshTable(const QVector<RewardOffer>& rewards)
{
    table->setRowCount(rewards.size());
    for (int i = 0; i < rewards.size(); i++)
    {
        table->setItem(i, 0, new QTableWidgetItem(rewards[i].itemName));
        table->setItem(i, 1, new QTableWidgetItem(rewards[i].message));
        table->setItem(i, 2, new QTableWidgetItem(rewards[i].contactInfo));
        table->setItem(i, 3, new QTableWidgetItem(rewards[i].offeredBy));
        table->setItem(i, 4, new QTableWidgetItem(rewards[i].date));
    }
}
