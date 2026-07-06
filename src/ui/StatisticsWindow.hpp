#ifndef STATISTICSWINDOW_H
#define STATISTICSWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include "../models/ReportManager.hpp"

class StatisticsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StatisticsWindow(QWidget* parent = nullptr);

    void loadReports(const QVector<ItemReport>& reports);

private:
    ReportManager manager;

    QLabel* totalLabel;
    QLabel* lostLabel;
    QLabel* foundLabel;
    QLabel* claimedLabel;
    QLabel* returnedLabel;

    QTableWidget* categoryTable;

    QPushButton* demoDataButton;
    QPushButton* refreshButton;

    void refreshStats(const QVector<ItemReport>& reports);
};

#endif
