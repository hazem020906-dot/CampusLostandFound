#ifndef GATE4DASHBOARD_H
#define GATE4DASHBOARD_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include "../models/ReportManager.hpp"
#include "../models/User.hpp"

class Gate4Dashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gate4Dashboard(const User& user, QWidget* parent = nullptr);

private:
    ReportManager manager;
    User          currentUser;

    QTableWidget* table;
    QComboBox*    statusBox;
    QPushButton*  verifyButton;   // marks selected item as Returned after ownership check
    QPushButton*  refreshButton;
    QLabel*       feedbackLabel;

    void refreshTable(const QVector<ItemReport>& reports);
};

#endif
