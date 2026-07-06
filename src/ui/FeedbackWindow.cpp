#include "FeedbackWindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QDate>

FeedbackWindow::FeedbackWindow(const User& user, QWidget* parent)
    : QMainWindow(parent), currentUser(user)
{
    setWindowTitle("Feedback & Ratings - " + user.username);
    resize(700, 550);

    QWidget*     central = new QWidget(this);
    QVBoxLayout* layout  = new QVBoxLayout();

    layout->addWidget(new QLabel("Rate Your Lost & Found Experience"));
    layout->addSpacing(8);

    ratingBox = new QComboBox();
    ratingBox->addItems({"5 - Excellent", "4 - Good", "3 - Average", "2 - Poor", "1 - Very Poor"});

    commentsInput = new QTextEdit();
    commentsInput->setPlaceholderText("Tell us about your experience (optional)");
    commentsInput->setMaximumHeight(90);

    submitButton = new QPushButton("Submit Feedback");

    layout->addWidget(new QLabel("Rating:"));
    layout->addWidget(ratingBox);
    layout->addWidget(new QLabel("Comments:"));
    layout->addWidget(commentsInput);
    layout->addWidget(submitButton);
    layout->addSpacing(10);

    averageLabel = new QLabel("Average rating: N/A");
    layout->addWidget(averageLabel);
    layout->addSpacing(6);

    layout->addWidget(new QLabel("All Feedback"));

    table = new QTableWidget();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Username", "Rating", "Comments", "Date"});
    layout->addWidget(table);

    central->setLayout(layout);
    setCentralWidget(central);

    refreshTable(manager.getAllFeedback());
    updateAverageLabel();

    connect(submitButton, &QPushButton::clicked, this, [this]()
    {
        int rating = ratingBox->currentText().left(1).toInt();

        bool success = manager.submitFeedback(
            currentUser.username,
            rating,
            commentsInput->toPlainText(),
            QDate::currentDate().toString("yyyy-MM-dd")
        );

        if (!success)
        {
            QMessageBox::warning(this, "Error", "Could not submit feedback.");
            return;
        }

        commentsInput->clear();
        refreshTable(manager.getAllFeedback());
        updateAverageLabel();
        QMessageBox::information(this, "Thank You", "Feedback submitted successfully.");
    });
}

void FeedbackWindow::refreshTable(const QVector<Feedback>& feedbackList)
{
    table->setRowCount(feedbackList.size());
    for (int i = 0; i < feedbackList.size(); i++)
    {
        table->setItem(i, 0, new QTableWidgetItem(feedbackList[i].username));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(feedbackList[i].rating)));
        table->setItem(i, 2, new QTableWidgetItem(feedbackList[i].comments));
        table->setItem(i, 3, new QTableWidgetItem(feedbackList[i].date));
    }
}

void FeedbackWindow::updateAverageLabel()
{
    double avg = manager.getAverageRating();
    if (manager.getAllFeedback().isEmpty())
    {
        averageLabel->setText("Average rating: N/A");
    }
    else
    {
        averageLabel->setText("Average rating: " + QString::number(avg, 'f', 1) + " / 5");
    }
}
