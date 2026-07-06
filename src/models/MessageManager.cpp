#include "MessageManager.hpp"

bool MessageManager::sendMessage(const QString& fromUser,
                                  const QString& toUser,
                                  const QString& itemName,
                                  const QString& content,
                                  const QString& date)
{
    if (fromUser.isEmpty() || toUser.isEmpty() || content.isEmpty())
        return false;

    Message msg;
    msg.fromUser = fromUser;
    msg.toUser   = toUser;
    msg.itemName = itemName;
    msg.content  = content;
    msg.date     = date;

    messages.push_back(msg);
    return true;
}

QVector<Message> MessageManager::getAllMessages() const
{
    return messages;
}

QVector<Message> MessageManager::getInbox(const QString& username) const
{
    QVector<Message> results;
    for (const Message& msg : messages)
    {
        if (msg.toUser == username)
            results.push_back(msg);
    }
    return results;
}

QVector<Message> MessageManager::getOutbox(const QString& username) const
{
    QVector<Message> results;
    for (const Message& msg : messages)
    {
        if (msg.fromUser == username)
            results.push_back(msg);
    }
    return results;
}
