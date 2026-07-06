#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include <QString>

struct Message {
    QString fromUser;
    QString toUser;
    QString itemName;
    QString content;
    QString date;
};

#endif
