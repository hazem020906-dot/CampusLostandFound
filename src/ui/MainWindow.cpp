#include "MainWindow.h"

#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QLabel>
#include <QTableWidgetItem>

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

    table = new QTableWidget();
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"Name", "Category", "Location", "Date", "Status"});

    layout->addWidget(nameInput);
    layout->addWidget(categoryInput);
    layout->addWidget(locationInput);
    layout->addWidget(dateInput);
    layout->addWidget(addButton);

    layout->addWidget(new QLabel("All Active Reports"));
    layout->addWidget(table);

    central->setLayout(layout);
    setCentralWidget(central);

    connect(addButton, &QPushButton::clicked, this, [this]() {
        if (nameInput->text().isEmpty() ||
            categoryInput->text().isEmpty() ||
            locationInput->text().isEmpty() ||
            dateInput->text().isEmpty())
        {
            QMessageBox::warning(this, "Error", "Please fill in all fields.");
            return;
        }

        manager.addLostItem(
            nameInput->text(),
            categoryInput->text(),
            locationInput->text(),
            dateInput->text()
        );

        nameInput->clear();
        categoryInput->clear();
        locationInput->clear();
        dateInput->clear();

        refreshTable(manager.getAllReports());

        QMessageBox::information(this, "Success", "Lost item submitted successfully.");
    });

    refreshTable(manager.getAllReports());
}

void MainWindow::refreshTable(const QVector<ItemReport>& reports)
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
