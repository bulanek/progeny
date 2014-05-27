#include "measurementdata.h"
#include "ui_measurementdata.h"

MeasurementData::MeasurementData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MeasurementData)
{
    ui->setupUi(this);
}

MeasurementData::~MeasurementData()
{
    delete ui;
}
