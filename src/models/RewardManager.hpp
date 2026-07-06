#ifndef REWARDMANAGER_HPP
#define REWARDMANAGER_HPP
#include <QVector>
#include <QString>
#include "RewardOffer.hpp"

class RewardManager
{
private:
    QVector<RewardOffer> rewards;

public:
    bool addReward(const QString& itemName,
                    const QString& message,
                    const QString& contactInfo,
                    const QString& offeredBy,
                    const QString& date);

    QVector<RewardOffer> getAllRewards() const;

    QVector<RewardOffer> searchByItemName(const QString& itemName) const;

    bool removeReward(int index);
};

#endif
