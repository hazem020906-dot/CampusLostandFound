#include "Gate4Dashoboard.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QAbstractItemView>

#include <nlohmann/json.hpp>

Gate4Dashboard::Gate4Dashboard(
    const User& user,
    NetworkClient* client,
    QWidget* parent
)
    : QMainWindow(parent),
      networkClient(client),
      currentUser(user)
{
    setWindowTitle("Gate 4 Lost & Found Office Security Dashboard");
    resize(850, 600);

    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(
        new QLabel("Gate 4 Security Dashboard - " + user.username)
    );

    layout->addWidget(
        new QLabel(
            "Review submitted reports. Select a row, verify ownership, "
            "then update the item status."
        )
    );

    layout->addSpacing(10);

    refreshButton = new QPushButton("Refresh Reports");
    layout->addWidget(refreshButton);

    table = new QTableWidget();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({
        "Name",
        "Category",
        "Location",
        "Date",
        "Status"
    });

    layout->addWidget(table);
    layout->addSpacing(8);
    layout->addWidget(new QLabel("Manage selected report:"));

    QHBoxLayout* controlRow = new QHBoxLayout();

    statusBox = new QComboBox();
    statusBox->addItems({
        "Lost",
        "Found",
        "Claimed",
        "Returned"
    });

    verifyButton =
        new QPushButton("Verify Ownership & Update Status");

    controlRow->addWidget(new QLabel("Set status to:"));
    controlRow->addWidget(statusBox);
    controlRow->addWidget(verifyButton);

    layout->addLayout(controlRow);

    feedbackLabel = new QLabel("");
    feedbackLabel->setWordWrap(true);
    layout->addWidget(feedbackLabel);

    central->setLayout(layout);
    setCentralWidget(central);

    // Receive updated reports from the server
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
                            reportJson.value("itemName", "")
                        );

                    report.category =
                        QString::fromStdString(
                            reportJson.value("category", "")
                        );

                    report.location =
                        QString::fromStdString(
                            reportJson.value("location", "")
                        );

                    report.date =
                        QString::fromStdString(
                            reportJson.value("date", "")
                        );

                    report.status =
                        QString::fromStdString(
                            reportJson.value("status", "")
                        );

                    receivedReports.push_back(report);
                }

                currentReports = receivedReports;
                refreshTable(currentReports);

                feedbackLabel->setText(
                    "Reports updated from the server."
                );
            }
            catch (const nlohmann::json::exception&)
            {
                QMessageBox::warning(
                    this,
                    "Error",
                    "The server sent invalid report data."
                );
            }
        }
    );

    // Refresh button asks the server for all reports
    connect(
        refreshButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            networkClient->requestAllReports();
            feedbackLabel->setText("Requesting reports...");
        }
    );

    // Verify ownership and send status update to server
    connect(
        verifyButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            int row = table->currentRow();

            if (row < 0)
            {
                QMessageBox::warning(
                    this,
                    "No Selection",
                    "Please select a report first."
                );
                return;
            }

            QMessageBox::StandardButton reply =
                QMessageBox::question(
                    this,
                    "Confirm Ownership Verification",
                    "Have you verified that this person is the "
                    "rightful owner?\n\nThe status will be changed to: " +
                        statusBox->currentText(),
                    QMessageBox::Yes | QMessageBox::No
                );

            if (reply != QMessageBox::Yes)
            {
                return;
            }

            networkClient->updateStatus(
                row,
                statusBox->currentText()
            );

            feedbackLabel->setText(
                "Status update sent to the server."
            );
        }
    );

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

            refreshButton->setEnabled(false);
            verifyButton->setEnabled(false);
        }
    );

    networkClient->requestAllReports();
}

void Gate4Dashboard::refreshTable(
    const QVector<ItemReport>& reports
)
{
    table->setRowCount(reports.size());

    for (int i = 0; i < reports.size(); i++)
    {
        table->setItem(
            i,
            0,
            new QTableWidgetItem(reports[i].itemName)
        );

        table->setItem(
            i,
            1,
            new QTableWidgetItem(reports[i].category)
        );

        table->setItem(
            i,
            2,
            new QTableWidgetItem(reports[i].location)
        );

        table->setItem(
            i,
            3,
            new QTableWidgetItem(reports[i].date)
        );

        table->setItem(
            i,
            4,
            new QTableWidgetItem(reports[i].status)
        );
    }
}
