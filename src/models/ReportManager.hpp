#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H
#include <QVector>
#include <QString>
#include "ItemReport.hpp"

class ReportManager
{
private:
    QVector<ItemReport> reports;

public:
    void addLostItem(const QString& name,
                     const QString& category,
                     const QString& location,
                     const QString& date);

    QVector<ItemReport> getAllReports() const;

    QVector<ItemReport> searchReports(const QString& keyword) const; //broader serch function

    bool updateStatus(int index, const QString& newStatus);

    QVector<ItemReport> filterByCategory(const QString& category) const;
    QVector<ItemReport> filterByDate(const QString& date) const;       // ll 4 of these functions are search by filtering
    QVector<ItemReport> filterByLocation(const QString& location) const;
    QVector<ItemReport> filterByStatus(const QString& status) const;

};

#endif
