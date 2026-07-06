#ifndef FEEDBACKMANAGER_HPP
#define FEEDBACKMANAGER_HPP
#include <QVector>
#include <QString>
#include "Feedback.hpp"

class FeedbackManager
{
private:
    QVector<Feedback> feedbackList;

public:
    bool submitFeedback(const QString& username,
                         int rating,
                         const QString& comments,
                         const QString& date);

    QVector<Feedback> getAllFeedback() const;

    double getAverageRating() const;
};

#endif
