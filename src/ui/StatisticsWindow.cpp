#include "StatisticsWindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QTableWidgetItem>
#include <QMap>

StatisticsWindow::StatisticsWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Lost & Found Statistics");
    resize(650, 550);

    QWidget*     central = new QWidget(this);
    QVBoxLayout* layout  = new QVBoxLayout();

    layout->addWidget(new QLabel("Reports Overview"));
    layout->addSpacing(8);

    totalLabel    = new QLabel("Total reports: 0");
    lostLabel     = new QLabel("Lost: 0");
    foundLabel    = new QLabel("Found: 0");
    claimedLabel  = new QLabel("Claimed: 0");
    returnedLabel = new QLabel("Returned: 0");

    layout->addWidget(totalLabel);
    layout->addWidget(lostLabel);
    layout->addWidget(foundLabel);
    layout->addWidget(claimedLabel);
    layout->addWidget(returnedLabel);
    layout->addSpacing(10);

    layout->addWidget(new QLabel("Reports by Category"));

    categoryTable = new QTableWidget();
    categoryTable->setColumnCount(2);
    categoryTable->setHorizontalHeaderLabels({"Category", "Count"});
    layout->addWidget(categoryTable);
    layout->addSpacing(10);

    QHBoxLayout* buttonRow = new QHBoxLayout();
    demoDataButton = new QPushButton("Load Demo Data");
    refreshButton  = new QPushButton("Refresh");
    buttonRow->addWidget(demoDataButton);
    buttonRow->addWidget(refreshButton);
    layout->addLayout(buttonRow);

    central->setLayout(layout);
    setCentralWidget(central);

    refreshStats(manager.getAllReports());

    connect(demoDataButton, &QPushButton::clicked, this, [this]()
    {
        manager.addLostItem("Backpack", "Personal", "Library", "2026-07-01");
        manager.addLostItem("Laptop Charger", "Electronic", "Cafeteria", "2026-07-02");
        manager.addLostItem("Student ID Card", "Personal", "Gate 4", "2026-07-03");
        manager.addLostItem("Umbrella", "Personal", "Science Building", "2026-07-03");
        manager.addLostItem("Headphones", "Electronic", "Library", "2026-07-04");

        refreshStats(manager.getAllReports());
    });

    connect(refreshButton, &QPushButton::clicked, this, [this]()
    {
        refreshStats(manager.getAllReports());
    });
}

void StatisticsWindow::loadReports(const QVector<ItemReport>& reports)
{
    refreshStats(reports);
}

void StatisticsWindow::refreshStats(const QVector<ItemReport>& reports)
{
    int lostCount     = 0;
    int foundCount    = 0;
    int claimedCount  = 0;
    int returnedCount = 0;

    QMap<QString, int> categoryCounts;

    for (const ItemReport& report : reports)
    {
        QString status = report.status.toLower();

        if (status == "lost")
            lostCount++;
        else if (status == "found")
            foundCount++;
        else if (status == "claimed")
            claimedCount++;
        else if (status == "returned")
            returnedCount++;

        categoryCounts[report.category] = categoryCounts.value(report.category, 0) + 1;
    }

    totalLabel->setText("Total reports: " + QString::number(reports.size()));
    lostLabel->setText("Lost: " + QString::number(lostCount));
    foundLabel->setText("Found: " + QString::number(foundCount));
    claimedLabel->setText("Claimed: " + QString::number(claimedCount));
    returnedLabel->setText("Returned: " + QString::number(returnedCount));

    categoryTable->setRowCount(categoryCounts.size());
    int row = 0;
    for (auto it = categoryCounts.constBegin(); it != categoryCounts.constEnd(); ++it)
    {
        categoryTable->setItem(row, 0, new QTableWidgetItem(it.key()));
        categoryTable->setItem(row, 1, new QTableWidgetItem(QString::number(it.value())));
        row++;
    }
}
