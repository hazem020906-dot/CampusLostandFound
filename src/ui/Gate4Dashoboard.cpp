#include "Gate4Dashoboard.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QAbstractItemView>

Gate4Dashboard::Gate4Dashboard(const User& user, QWidget* parent)
    : QMainWindow(parent), currentUser(user)
{
    setWindowTitle("Gate 4 Lost & Found Office Security Dashboard");
    resize(850, 600);

    QWidget*     central = new QWidget(this);
    QVBoxLayout* layout  = new QVBoxLayout();

    layout->addWidget(new QLabel("Gate 4 Security Dashboard" + user.username));
    layout->addWidget(new QLabel(
        "Review all submitted reports. Select a row and use the controls below "
        "to manage item status and verify ownership before returning items."
    ));
    layout->addSpacing(10);

    refreshButton = new QPushButton("Refresh Reports");
    layout->addWidget(refreshButton);

    table = new QTableWidget();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"Name", "Category", "Location", "Date", "Status"});
    layout->addWidget(table);

    layout->addSpacing(8);
    layout->addWidget(new QLabel("Manage selected report:"));

    QHBoxLayout* controlRow = new QHBoxLayout();

    statusBox = new QComboBox();
    statusBox->addItems({"Lost", "Found", "Claimed", "Returned"});

    verifyButton = new QPushButton("Verify Ownership & Update Status");

    controlRow->addWidget(new QLabel("Set status to:"));
    controlRow->addWidget(statusBox);
    controlRow->addWidget(verifyButton);
    layout->addLayout(controlRow);

    feedbackLabel = new QLabel("");
    feedbackLabel->setWordWrap(true);
    layout->addWidget(feedbackLabel);

    central->setLayout(layout);
    setCentralWidget(central);

    refreshTable(manager.getAllReports());

    // ── Connections ──────────────────────────────────────────────────────────

    connect(refreshButton, &QPushButton::clicked, this, [this]()
    {
        refreshTable(manager.getAllReports());
        feedbackLabel->setText("Reports refreshed.");
    });

    connect(verifyButton, &QPushButton::clicked, this, [this]()
    {
        int row = table->currentRow();

        if (row < 0)
        {
            QMessageBox::warning(this, "No selection",
                "Please select a report from the table first.");
            return;
        }

        // Ownership confirmation step before marking as returned
        auto reply = QMessageBox::question(
            this,
            "Confirm Ownership Verification",
            "Have you verified that this  is the rightful owner of this item?\n\n"
            "This will update the item status to: " + statusBox->currentText(),
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply != QMessageBox::Yes)
            return;

        if (manager.updateStatus(row, statusBox->currentText()))
        {
            refreshTable(manager.getAllReports());
            feedbackLabel->setText("Status updated to '" + statusBox->currentText() +
                                   "' after ownership verification.");
        }
        else
        {
            feedbackLabel->setText("Error: could not update status. Report not found.");
        }
    });
}

void Gate4Dashboard::refreshTable(const QVector<ItemReport>& reports)
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
