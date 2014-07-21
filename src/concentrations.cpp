#include "concentrations.h"
#include "ui_concentrations.h"

Concentrations::Concentrations(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Concentrations)
{
    ui->setupUi(this);

    _dataHandle=DataHandle::getInstance();

    this->showTable().showConcentrations();
}

Concentrations::~Concentrations()
{
    delete ui;
}


Concentrations& Concentrations::showTable(){

    const gsl_matrix* covMat=_dataHandle->getCovMat();


    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
           ui->tableWidget->setItem(i,j,new QTableWidgetItem(TOOLS::convertToString(gsl_matrix_get(covMat,i,j)).c_str()));
           ui->tableWidget_2->setItem(i,j,new QTableWidgetItem(TOOLS::convertToString(gsl_matrix_get(covMat,i,j)/(sqrt(gsl_matrix_get(covMat,i,i))*sqrt(gsl_matrix_get(covMat,j,j)))).c_str()));
        }
    }

    return *this;
}

Concentrations& Concentrations::showConcentrations(){
    const gsl_vector* results=_dataHandle->getResults();
    const gsl_matrix* covMat=_dataHandle->getCovMat();

    ui->lineEdit->setText(TOOLS::convertToString(gsl_vector_get(results,0)).c_str());
    ui->lineEdit_3->setText(TOOLS::convertToString(gsl_vector_get(results,1)).c_str());
    ui->lineEdit_5->setText(TOOLS::convertToString(gsl_vector_get(results,2)).c_str());

    ui->lineEdit_2->setText(TOOLS::convertToString(sqrt(gsl_matrix_get(covMat,0,0))).c_str());
    ui->lineEdit_4->setText(TOOLS::convertToString(sqrt(gsl_matrix_get(covMat,1,1))).c_str());
    ui->lineEdit_6->setText(TOOLS::convertToString(sqrt(gsl_matrix_get(covMat,2,2))).c_str());


    return *this;
}
