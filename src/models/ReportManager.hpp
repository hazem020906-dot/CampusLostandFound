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

    QVector<ItemReport> searchReports(const QString& keyword) const;

    bool updateStatus(int index, const QString& newStatus);
};

#endif
