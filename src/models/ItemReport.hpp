#ifndef ITEMREPORT_H
#define ITEMREPORT_H
#include <QString>

struct ItemReport {
    QString itemName;
    QString category;//electronic or personal etc
    QString status;//lost or found
    QString location;
    QString date;
};

#endif
//model view architecture this is the model part
