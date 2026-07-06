#ifndef MESSAGINGWINDOW_H
#define MESSAGINGWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTabWidget>
#include <QLabel>
#include "../models/MessageManager.hpp"
#include "../models/User.hpp"

class MessagingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MessagingWindow(const User& user, QWidget* parent = nullptr);

private:
    MessageManager manager;
    User           currentUser;

    QLineEdit*   toInput;
    QLineEdit*   itemNameInput;
    QTextEdit*   contentInput;
    QPushButton* sendButton;

    QTabWidget*   tabs;
    QTableWidget* inboxTable;
    QTableWidget* outboxTable;
    QPushButton*  refreshButton;

    void refreshTables();
    void fillTable(QTableWidget* targetTable, const QVector<Message>& messageList);
};

#endif
