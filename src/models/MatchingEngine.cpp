#include "MatchingEngine.hpp"

QVector<ItemReport> MatchingEngine::findMatches(const ItemReport& target,
                                                 const QVector<ItemReport>& allReports)
{
 QVector<ItemReport> matches;
for (const ItemReport& candidate : allReports)
{
if (candidate.itemName == target.itemName &&
     candidate.date == target.date &&
candidate.location == target.location &&
candidate.status == target.status)
        {
            continue;
        }

    if (isPotentialMatch(target, candidate))
       {
            matches.push_back(candidate);
  }
    }

    return matches;
}

bool MatchingEngine::isPotentialMatch(const ItemReport& a, const ItemReport& b)
{
    bool oppositeStatus = (a.status.toLower() == "lost" && b.status.toLower() == "found") ||
                          (a.status.toLower() == "found" && b.status.toLower() == "lost");

    if (!oppositeStatus)
        return false;

    if (a.category.toLower() != b.category.toLower())
        return false;

    return namesAreSimilar(a.itemName, b.itemName);
}

bool MatchingEngine::namesAreSimilar(const QString& nameA, const QString& nameB)
{
    QString lowerA = nameA.toLower().trimmed();
    QString lowerB = nameB.toLower().trimmed();

    if (lowerA.isEmpty() || lowerB.isEmpty())
        return false;

    return lowerA.contains(lowerB) || lowerB.contains(lowerA);
}
