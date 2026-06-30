#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include "../models/ReportManager.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    ReportManager manager;

    QLineEdit *nameInput;
    QLineEdit *categoryInput;
    QLineEdit *locationInput;
    QLineEdit *dateInput;
    QLineEdit *searchInput;

    QPushButton *addButton;
    QPushButton *searchButton;
    QPushButton *showAllButton;
    QPushButton *updateStatusButton;

    QComboBox *statusBox;

    QTableWidget *table;

    void refreshTable(const QVector<ItemReport>& reports);
};

#endif
