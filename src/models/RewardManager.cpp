#include "RewardManager.hpp"

bool RewardManager::addReward(const QString& itemName,
                               const QString& message,
                               const QString& contactInfo,
                               const QString& offeredBy,
                               const QString& date)
{
    if (itemName.isEmpty() || message.isEmpty())
        return false;

    RewardOffer reward;
    reward.itemName    = itemName;
    reward.message     = message;
    reward.contactInfo = contactInfo;
    reward.offeredBy   = offeredBy;
    reward.date        = date;

    rewards.push_back(reward);
    return true;
}

QVector<RewardOffer> RewardManager::getAllRewards() const
{
    return rewards;
}

QVector<RewardOffer> RewardManager::searchByItemName(const QString& itemName) const
{
    QVector<RewardOffer> results;
    QString lowerName = itemName.toLower();

    for (const RewardOffer& reward : rewards)
    {
        if (reward.itemName.toLower().contains(lowerName))
            results.push_back(reward);
    }
    return results;
}

bool RewardManager::removeReward(int index)
{
    if (index < 0 || index >= rewards.size())
        return false;

    rewards.remove(index);
    return true;
}
