#include "ReportManager.hpp"

void ReportManager::addLostItem(const QString& name,
                                const QString& category,
                                const QString& location,
                                const QString& date)
{
    ItemReport report;
    report.itemName = name;
    report.category = category;
    report.location = location;
    report.date = date;
    report.status = "Lost";

    reports.push_back(report);
}

QVector<ItemReport> ReportManager::getAllReports() const
{
    return reports;
}

QVector<ItemReport> ReportManager::searchReports(const QString& keyword) const
{
    QVector<ItemReport> results;

    QString lowerKeyword = keyword.toLower();

    for (const ItemReport& report : reports)
    {
        if (report.itemName.toLower().contains(lowerKeyword) ||
            report.category.toLower().contains(lowerKeyword) ||
            report.location.toLower().contains(lowerKeyword) ||
            report.date.toLower().contains(lowerKeyword) ||
            report.status.toLower().contains(lowerKeyword))
        {
            results.push_back(report);
        }
    }

    return results;
}

bool ReportManager::updateStatus(int index, const QString& newStatus)
{
    if (index < 0 || index >= reports.size())
    {
        return false;
    }

    reports[index].status = newStatus;
    return true;
}
QVector<ItemReport> ReportManager::filterByCategory(const QString& category) const
{
    QVector<ItemReport> results;
    for (const ItemReport& report : reports)
    {
        if (report.category.toLower() == category.toLower())
            results.push_back(report);
    }
    return results;
}

QVector<ItemReport> ReportManager::filterByDate(const QString& date) const
{
    QVector<ItemReport> results;
    for (const ItemReport& report : reports)
    {
        if (report.date == date)
            results.push_back(report);
    }
    return results;
}

QVector<ItemReport> ReportManager::filterByLocation(const QString& location) const
{
    QVector<ItemReport> results;
    for (const ItemReport& report : reports)
    {
        if (report.location.toLower().contains(location.toLower()))
            results.push_back(report);
    }
    return results;
}

QVector<ItemReport> ReportManager::filterByStatus(const QString& status) const
{
    QVector<ItemReport> results;
    for (const ItemReport& report : reports)
    {
        if (report.status.toLower() == status.toLower())
            results.push_back(report);
    }
    return results;
}
