#ifndef MEASUREMENTDATA_H
#define MEASUREMENTDATA_H

#include <QDialog>

namespace Ui {
class MeasurementData;
}

class MeasurementData : public QDialog
{
    Q_OBJECT

public:
    explicit MeasurementData(QWidget *parent = 0);
    ~MeasurementData();

private:
    Ui::MeasurementData *ui;
};

#endif // MEASUREMENTDATA_H
