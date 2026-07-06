#ifndef MESSAGEMANAGER_HPP
#define MESSAGEMANAGER_HPP
#include <QVector>
#include <QString>
#include "Message.hpp"

class MessageManager
{
private:
    QVector<Message> messages;

public:
    bool sendMessage(const QString& fromUser,
                      const QString& toUser,
                      const QString& itemName,
                      const QString& content,
                      const QString& date);

    QVector<Message> getAllMessages() const;

    QVector<Message> getInbox(const QString& username) const;
    QVector<Message> getOutbox(const QString& username) const;
};

#endif
