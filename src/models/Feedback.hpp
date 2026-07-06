#ifndef FEEDBACK_HPP
#define FEEDBACK_HPP
#include <QString>

struct Feedback {
    QString username;
    int     rating; // 1 to 5 stars
    QString comments;
    QString date;
};
#endif
