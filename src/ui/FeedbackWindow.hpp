#ifndef FEEDBACKWINDOW_H
#define FEEDBACKWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include "../models/FeedbackManager.hpp"
#include "../models/User.hpp"

class FeedbackWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FeedbackWindow(const User& user, QWidget* parent = nullptr);

private:
    FeedbackManager manager;
    User            currentUser;

    QComboBox*   ratingBox;
    QTextEdit*   commentsInput;
    QPushButton* submitButton;

    QLabel*       averageLabel;
    QTableWidget* table;

    void refreshTable(const QVector<Feedback>& feedbackList);
    void updateAverageLabel();
};

#endif
