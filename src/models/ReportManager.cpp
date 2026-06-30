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
