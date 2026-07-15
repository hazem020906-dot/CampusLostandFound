#ifndef STUDENTDASHBOARD_H
#define STUDENTDASHBOARD_H
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>
#include <QLabel>
#include "../client/NetworkClient.hpp"
#include "../models/ItemReport.hpp"
#include "../models/User.hpp"

class StudentDashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentDashboard(
    const User& user,
    NetworkClient* networkClient,
    QWidget* parent = nullptr
);

private:
    NetworkClient* networkClient;
QVector<ItemReport> currentReports;
    User  currentUser;

    QLineEdit*   nameInput;
    QLineEdit*   categoryInput;
    QLineEdit*   locationInput;
    QLineEdit*   dateInput;
    QComboBox*   reportTypeBox; 
    QPushButton* submitButton;
    QPushButton* feedbackButton;
    QPushButton* messagesButton;
    QPushButton* statsButton;
    QPushButton* rewardsButton;

    
    QComboBox*   filterTypeBox;    
    QLineEdit*   filterValueInput; 
    QComboBox*   filterStatusBox;  
    QPushButton* applyFilterButton;
    QPushButton* clearFilterButton;

    
    QTableWidget* table;

    void refreshTable(const QVector<ItemReport>& reports);
};

#endif
