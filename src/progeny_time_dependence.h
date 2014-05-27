#ifndef PROGENY_TIME_DEPENDENCE_H
#define PROGENY_TIME_DEPENDENCE_H
#include <QDialog>
#include "qcustomplot.h"
#include "datahandle.h"
#include "qcustomplot.h"
#include "tools.h"


namespace Ui {
class Progeny_time_dependence;
}

class Progeny_time_dependence : public QDialog
{
    Q_OBJECT

public:
    explicit Progeny_time_dependence(QWidget *parent = 0);
    ~Progeny_time_dependence();
    void showPlot();
    void saveWindowData();


private slots:
    void on_commandLinkButton_3_clicked();

    void on_commandLinkButton_clicked();

    void on_commandLinkButton_2_clicked();

private:
    Ui::Progeny_time_dependence *ui;
    DataHandle* _dataHandle;
};

#endif // PROGENY_TIME_DEPENDENCE_H
