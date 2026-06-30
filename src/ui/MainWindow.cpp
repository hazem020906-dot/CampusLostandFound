#include "MainWindow.h"

#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QLabel>

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

    layout->addWidget(nameInput);
    layout->addWidget(categoryInput);
    layout->addWidget(locationInput);
    layout->addWidget(dateInput);
    layout->addWidget(addButton);

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

        QMessageBox::information(this, "Success", "Lost item submitted successfully.");
    });
}

void MainWindow::refreshTable(const QVector<ItemReport>& reports)
{
    Q_UNUSED(reports);
}
