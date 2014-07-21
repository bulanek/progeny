#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "datahandle.h"
#include "tools.h"
#include "sqlconnection.h"
#include "concentrations.h"

#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static QString TITLE;
    void setMainData();
    void getMainData();



private slots:
    void on_actionOpen_triggered();
    void on_commandLinkButton_2_clicked();
    void about();

    void on_actionOpen_database_triggered();

    void on_commandLinkButton_4_clicked();

    void on_commandLinkButton_5_clicked();

private:
    Ui::MainWindow *ui;
    DataHandle* _dataHandle;
};

#endif // MAINWINDOW_H
