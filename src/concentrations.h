#ifndef CONCENTRATIONS_H
#define CONCENTRATIONS_H

#include <QDialog>
#include "datahandle.h"

namespace Ui {
class Concentrations;
}

class Concentrations : public QDialog
{
    Q_OBJECT

public:
    explicit Concentrations(QWidget *parent = 0);
    ~Concentrations();

    Concentrations& showTable();
    Concentrations& showConcentrations();

private:
    Ui::Concentrations *ui;
    DataHandle* _dataHandle;
};

#endif // CONCENTRATIONS_H
