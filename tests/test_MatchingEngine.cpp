#include <gtest/gtest.h>
#include "MatchingEngine.hpp"

ItemReport makeReport(const QString& name, const QString& category,
                       const QString& status, const QString& location, const QString& date)
{
    ItemReport report;
    report.itemName = name;
    report.category = category;
    report.status   = status;
    report.location = location;
    report.date     = date;
    return report;
}

TEST(MatchingEngineTest, FindsMatchWithSimilarNameOppositeStatus)
{
    ItemReport lostItem  = makeReport("Black Backpack", "Personal", "Lost", "Library", "2026-07-01");
    ItemReport foundItem = makeReport("Backpack", "Personal", "Found", "Cafeteria", "2026-07-02");

    QVector<ItemReport> allReports = {lostItem, foundItem};

    auto matches = MatchingEngine::findMatches(lostItem, allReports);
    EXPECT_EQ(matches.size(), 1);
    EXPECT_EQ(matches[0].itemName, "Backpack");
}

TEST(MatchingEngineTest, NoMatchWhenCategoryDiffers)
{
    ItemReport lostItem  = makeReport("Backpack", "Personal", "Lost", "Library", "2026-07-01");
    ItemReport foundItem = makeReport("Backpack", "Electronic", "Found", "Cafeteria", "2026-07-02");

    QVector<ItemReport> allReports = {lostItem, foundItem};

    auto matches = MatchingEngine::findMatches(lostItem, allReports);
    EXPECT_TRUE(matches.empty());
}

TEST(MatchingEngineTest, NoMatchWhenBothAreLost)
{
    ItemReport lostItemA = makeReport("Backpack", "Personal", "Lost", "Library", "2026-07-01");
    ItemReport lostItemB = makeReport("Backpack", "Personal", "Lost", "Cafeteria", "2026-07-02");

    QVector<ItemReport> allReports = {lostItemA, lostItemB};

    auto matches = MatchingEngine::findMatches(lostItemA, allReports);
    EXPECT_TRUE(matches.empty());
}

TEST(MatchingEngineTest, NoMatchWhenNamesAreUnrelated)
{
    ItemReport lostItem  = makeReport("Backpack", "Personal", "Lost", "Library", "2026-07-01");
    ItemReport foundItem = makeReport("Umbrella", "Personal", "Found", "Cafeteria", "2026-07-02");

    QVector<ItemReport> allReports = {lostItem, foundItem};

    auto matches = MatchingEngine::findMatches(lostItem, allReports);
    EXPECT_TRUE(matches.empty());
}

TEST(MatchingEngineTest, DoesNotMatchIdenticalCopyOfItself)
{
    ItemReport lostItem = makeReport("Backpack", "Personal", "Lost", "Library", "2026-07-01");
    QVector<ItemReport> allReports = {lostItem};

    auto matches = MatchingEngine::findMatches(lostItem, allReports);
    EXPECT_TRUE(matches.empty());
}

TEST(MatchingEngineTest, ReturnsMultipleMatchesWhenSeveralQualify)
{
    ItemReport lostItem   = makeReport("Backpack", "Personal", "Lost", "Library", "2026-07-01");
    ItemReport foundItemA = makeReport("Black Backpack", "Personal", "Found", "Cafeteria", "2026-07-02");
    ItemReport foundItemB = makeReport("Backpack", "Personal", "Found", "Gate 4", "2026-07-03");

    QVector<ItemReport> allReports = {lostItem, foundItemA, foundItemB};

    auto matches = MatchingEngine::findMatches(lostItem, allReports);
    EXPECT_EQ(matches.size(), 2);
}
