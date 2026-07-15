#include <gtest/gtest.h>
#include "ReportManager.hpp"

TEST(ReportManagerTest, StartsEmpty)
{
    ReportManager manager;
    EXPECT_EQ(manager.getAllReports().size(), 0);
}

TEST(ReportManagerTest, AddLostItemIncreasesCount)
{
    ReportManager manager;
    manager.addLostItem("Backpack", "Personal", "Library", "2026-07-01");
    EXPECT_EQ(manager.getAllReports().size(), 1);
    EXPECT_EQ(manager.getAllReports()[0].itemName, "Backpack");
}

TEST(ReportManagerTest, SearchFindsMatchingKeyword)
{
    ReportManager manager;
    manager.addLostItem("Blue Backpack", "Personal", "Library", "2026-07-01");
    manager.addLostItem("Laptop Charger", "Electronic", "Cafeteria", "2026-07-02");

    auto results = manager.searchReports("backpack");
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].itemName, "Blue Backpack");
}

TEST(ReportManagerTest, SearchWithNoMatchesReturnsEmpty)
{
    ReportManager manager;
    manager.addLostItem("Backpack", "Personal", "Library", "2026-07-01");

    auto results = manager.searchReports("umbrella");
    EXPECT_TRUE(results.empty());
}

TEST(ReportManagerTest, UpdateStatusChangesValue)
{
    ReportManager manager;
    manager.addLostItem("Backpack", "Personal", "Library", "2026-07-01");

    bool success = manager.updateStatus(0, "Found");
    EXPECT_TRUE(success);
    EXPECT_EQ(manager.getAllReports()[0].status, "Found");
}

TEST(ReportManagerTest, UpdateStatusWithInvalidIndexFails)
{
    ReportManager manager;
    bool success = manager.updateStatus(5, "Found");
    EXPECT_FALSE(success);
}

TEST(ReportManagerTest, FilterByCategoryReturnsOnlyMatching)
{
    ReportManager manager;
    manager.addLostItem("Backpack", "Personal", "Library", "2026-07-01");
    manager.addLostItem("Charger", "Electronic", "Cafeteria", "2026-07-02");

    auto results = manager.filterByCategory("Electronic");
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].itemName, "Charger");
}

TEST(ReportManagerTest, FilterByLocationReturnsOnlyMatching)
{
    ReportManager manager;
    manager.addLostItem("Backpack", "Personal", "Library", "2026-07-01");
    manager.addLostItem("Umbrella", "Personal", "Gate 4", "2026-07-02");

    auto results = manager.filterByLocation("Gate 4");
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].itemName, "Umbrella");
}
