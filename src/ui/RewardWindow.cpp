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
    setWindowTitle("Reward Offers - " + user.username);
    resize(800, 600);

    QWidget*     central = new QWidget(this);
    QVBoxLayout* layout  = new QVBoxLayout();

    layout->addWidget(new QLabel("Offer a Reward for a Lost Item"));
    layout->addSpacing(8);

    itemNameInput = new QLineEdit();
    itemNameInput->setPlaceholderText("Item name");

    amountInput = new QDoubleSpinBox();
    amountInput->setRange(1, 100000);
    amountInput->setPrefix("EGP ");
    amountInput->setValue(100);

    contactInput = new QLineEdit();
    contactInput->setPlaceholderText("Contact info (phone or email)");

    descriptionInput = new QLineEdit();
    descriptionInput->setPlaceholderText("Description (optional)");

    addButton = new QPushButton("Offer Reward");

    layout->addWidget(itemNameInput);
    layout->addWidget(new QLabel("Reward amount:"));
    layout->addWidget(amountInput);
    layout->addWidget(contactInput);
    layout->addWidget(descriptionInput);
    layout->addWidget(addButton);
    layout->addSpacing(10);

    layout->addWidget(new QLabel("Search Reward Offers"));

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

    layout->addWidget(new QLabel("All Reward Offers"));

    table = new QTableWidget();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"Item", "Amount", "Contact", "Description", "Offered By", "Date"});
    layout->addWidget(table);

    removeButton = new QPushButton("Remove Selected Offer");
    layout->addWidget(removeButton);

    central->setLayout(layout);
    setCentralWidget(central);

    refreshTable(manager.getAllRewards());

    connect(addButton, &QPushButton::clicked, this, [this]()
    {
        if (itemNameInput->text().isEmpty() || contactInput->text().isEmpty())
        {
            QMessageBox::warning(this, "Error", "Please fill in the item name and contact info.");
            return;
        }

        bool success = manager.addReward(
            itemNameInput->text(),
            amountInput->value(),
            contactInput->text(),
            descriptionInput->text(),
            currentUser.username,
            QDate::currentDate().toString("yyyy-MM-dd")
        );

        if (!success)
        {
            QMessageBox::warning(this, "Error", "Could not add reward offer.");
            return;
        }

        itemNameInput->clear();
        contactInput->clear();
        descriptionInput->clear();
        amountInput->setValue(100);

        refreshTable(manager.getAllRewards());
        QMessageBox::information(this, "Success", "Reward offer added successfully.");
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
            QMessageBox::warning(this, "No selection", "Please select a reward offer to remove.");
            return;
        }

        if (manager.removeReward(row))
        {
            refreshTable(manager.getAllRewards());
        }
        else
        {
            QMessageBox::warning(this, "Error", "Could not remove reward offer.");
        }
    });
}

void RewardWindow::refreshTable(const QVector<RewardOffer>& rewards)
{
    table->setRowCount(rewards.size());
    for (int i = 0; i < rewards.size(); i++)
    {
        table->setItem(i, 0, new QTableWidgetItem(rewards[i].itemName));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(rewards[i].amount, 'f', 2)));
        table->setItem(i, 2, new QTableWidgetItem(rewards[i].contactInfo));
        table->setItem(i, 3, new QTableWidgetItem(rewards[i].description));
        table->setItem(i, 4, new QTableWidgetItem(rewards[i].offeredBy));
        table->setItem(i, 5, new QTableWidgetItem(rewards[i].date));
    }
}
