#ifndef MATCHINGENGINE_HPP
#define MATCHINGENGINE_HPP
#include <QVector>
#include "ItemReport.hpp"

class MatchingEngine
{
public:
static QVector<ItemReport> findMatches(const ItemReport& target,
                                            const QVector<ItemReport>& allReports);

private:
    static bool isPotentialMatch(const ItemReport& a, const ItemReport& b);
    static bool namesAreSimilar(const QString& nameA, const QString& nameB);
};

#endif
