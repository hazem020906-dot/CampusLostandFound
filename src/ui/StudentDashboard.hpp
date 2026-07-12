#ifndef STUDENTDASHBOARD_H
#define STUDENTDASHBOARD_H
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>
#include <QLabel>
#include "../models/ReportManager.hpp"
#include "../models/User.hpp"

class StudentDashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentDashboard(const User& user, QWidget* parent = nullptr);//constructor makes window with student name

private:
    ReportManager manager;
    User          currentUser;//a copy of currentuser info to be able to do currentuser.username etc

    QLineEdit*   nameInput;//itemname for user to write
    QLineEdit*   categoryInput;//category of item to display
    QLineEdit*   locationInput;
    QLineEdit*   dateInput;
    QComboBox*   reportTypeBox; // Lost or Found
    QPushButton* submitButton;
    QPushButton* feedbackButton;
    QPushButton* messagesButton;
    QPushButton* statsButton;
    QPushButton* rewardsButton;

    // Filter controls
    QComboBox*   filterTypeBox;    // dropdown to pick which filter to apply: Category, Date etc
    QLineEdit*   filterValueInput; // the value to filter by like filter by location but by writimng
    QComboBox*   filterStatusBox;  // only used when filtering by status
    QPushButton* applyFilterButton;
    QPushButton* clearFilterButton;

    // Table
    QTableWidget* table;

    void refreshTable(const QVector<ItemReport>& reports);
};

#endif
