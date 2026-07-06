#include "MessagingWindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QDate>

MessagingWindow::MessagingWindow(const User& user, QWidget* parent)
    : QMainWindow(parent), currentUser(user)
{
    setWindowTitle("Messages - " + user.username);
    resize(750, 600);

    QWidget*     central = new QWidget(this);
    QVBoxLayout* layout  = new QVBoxLayout();

    layout->addWidget(new QLabel("Contact the Finder or Owner of an Item"));
    layout->addSpacing(8);

    toInput = new QLineEdit();
    toInput->setPlaceholderText("Recipient username");

    itemNameInput = new QLineEdit();
    itemNameInput->setPlaceholderText("Item this message is about");

    contentInput = new QTextEdit();
    contentInput->setPlaceholderText("Write your message here");
    contentInput->setMaximumHeight(90);

    sendButton = new QPushButton("Send Message");

    layout->addWidget(new QLabel("To:"));
    layout->addWidget(toInput);
    layout->addWidget(new QLabel("Item:"));
    layout->addWidget(itemNameInput);
    layout->addWidget(new QLabel("Message:"));
    layout->addWidget(contentInput);
    layout->addWidget(sendButton);
    layout->addSpacing(10);

    tabs = new QTabWidget();

    inboxTable = new QTableWidget();
    inboxTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    inboxTable->setColumnCount(4);
    inboxTable->setHorizontalHeaderLabels({"From", "Item", "Message", "Date"});

    outboxTable = new QTableWidget();
    outboxTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    outboxTable->setColumnCount(4);
    outboxTable->setHorizontalHeaderLabels({"To", "Item", "Message", "Date"});

    tabs->addTab(inboxTable, "Inbox");
    tabs->addTab(outboxTable, "Sent");

    refreshButton = new QPushButton("Refresh");

    layout->addWidget(tabs);
    layout->addWidget(refreshButton);

    central->setLayout(layout);
    setCentralWidget(central);

    refreshTables();

    connect(sendButton, &QPushButton::clicked, this, [this]()
    {
        if (toInput->text().isEmpty() || contentInput->toPlainText().isEmpty())
        {
            QMessageBox::warning(this, "Error", "Please enter a recipient and a message.");
            return;
        }

        bool success = manager.sendMessage(
            currentUser.username,
            toInput->text(),
            itemNameInput->text(),
            contentInput->toPlainText(),
            QDate::currentDate().toString("yyyy-MM-dd")
        );

        if (!success)
        {
            QMessageBox::warning(this, "Error", "Could not send message.");
            return;
        }

        toInput->clear();
        itemNameInput->clear();
        contentInput->clear();

        refreshTables();
        QMessageBox::information(this, "Sent", "Message sent successfully.");
    });

    connect(refreshButton, &QPushButton::clicked, this, [this]()
    {
        refreshTables();
    });
}

void MessagingWindow::refreshTables()
{
    fillTable(inboxTable, manager.getInbox(currentUser.username));
    fillTable(outboxTable, manager.getOutbox(currentUser.username));
}

void MessagingWindow::fillTable(QTableWidget* targetTable, const QVector<Message>& messageList)
{
    targetTable->setRowCount(messageList.size());
    for (int i = 0; i < messageList.size(); i++)
    {
        QString otherParty = (targetTable == inboxTable) ? messageList[i].fromUser
                                                           : messageList[i].toUser;

        targetTable->setItem(i, 0, new QTableWidgetItem(otherParty));
        targetTable->setItem(i, 1, new QTableWidgetItem(messageList[i].itemName));
        targetTable->setItem(i, 2, new QTableWidgetItem(messageList[i].content));
        targetTable->setItem(i, 3, new QTableWidgetItem(messageList[i].date));
    }
}
