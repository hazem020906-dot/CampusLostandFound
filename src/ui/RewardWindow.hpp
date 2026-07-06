#ifndef REWARDWINDOW_H
#define REWARDWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include "../models/RewardManager.hpp"
#include "../models/User.hpp"

class RewardWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RewardWindow(const User& user, QWidget* parent = nullptr);

private:
    RewardManager manager;
    User          currentUser;

    QLineEdit*   itemNameInput;
    QTextEdit*   messageInput;
    QLineEdit*   contactInput;
    QPushButton* addButton;

    QLineEdit*   searchInput;
    QPushButton* searchButton;
    QPushButton* showAllButton;
    QPushButton* removeButton;

    QTableWidget* table;

    void refreshTable(const QVector<RewardOffer>& rewards);
};

#endif
