#include "FeedbackManager.hpp"

bool FeedbackManager::submitFeedback(const QString& username,
                                      int rating,
                                      const QString& comments,
                                      const QString& date)
{
    if (username.isEmpty())
        return false;

    if (rating < 1 || rating > 5)
        return false;

    Feedback fb;
    fb.username = username;
    fb.rating   = rating;
    fb.comments = comments;
    fb.date     = date;

    feedbackList.push_back(fb);
    return true;
}

QVector<Feedback> FeedbackManager::getAllFeedback() const
{
    return feedbackList;
}

double FeedbackManager::getAverageRating() const
{
    if (feedbackList.isEmpty())
        return 0.0;

    int total = 0;
    for (const Feedback& fb : feedbackList)
    {
        total += fb.rating;
    }

    return static_cast<double>(total) / feedbackList.size();
}
