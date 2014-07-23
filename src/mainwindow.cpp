#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "progeny_time_dependence.h"



QString MainWindow::TITLE="Progeny estimation";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(TITLE);
    _dataHandle=DataHandle::getInstance();
    this->setMainData();

    ui->commandLinkButton_2->setEnabled(0);
    ui->commandLinkButton_4->setEnabled(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_triggered() {
    QString fname;
    try{
        fname=QFileDialog::getOpenFileName(this,"Open file","");
        if (fname=="") return;
        this->setWindowTitle(TITLE+"; File: "+fname);
        if(_dataHandle->createDataFromTxt(fname.toStdString())==1){
            throw 1;
        }

    }catch(...){
        string text= string("Cannot open txt file ")+fname.toStdString()+"!";
        cerr<<text<<endl;
        QMessageBox box;
        box.setText(text.c_str());
        box.exec();
        return;
    }
    ui->commandLinkButton_2->setEnabled(1);
    ui->commandLinkButton_4->setEnabled(1);

}

void MainWindow::on_commandLinkButton_2_clicked()
{
    this->setMainData();
    double initialParameters[]={ui->lineEdit_4->text().toDouble(),
                                ui->lineEdit_5->text().toDouble(),
                                ui->lineEdit_6->text().toDouble()};

    _dataHandle->chiSquareComputeGSL(initialParameters);
    const gsl_vector* results=_dataHandle->getResults();

    ui->lineEdit_4->setText(TOOLS::convertToString(gsl_vector_get(results,0)).c_str());
    ui->lineEdit_5->setText(TOOLS::convertToString(gsl_vector_get(results,1)).c_str());
    ui->lineEdit_6->setText(TOOLS::convertToString(gsl_vector_get(results,2)).c_str());

    Concentrations concWindow;
    concWindow.exec();
}

void MainWindow::setMainData(){
    MainWindowData mainData;
    mainData.lambda[0]=ui->lineEdit->text().toDouble();
    mainData.lambda[1]=ui->lineEdit_2->text().toDouble();
    mainData.lambda[2]=ui->lineEdit_3->text().toDouble();
    mainData.filt_time=ui->lineEdit_7->text().toDouble();
    mainData.volume=ui->lineEdit_8->text().toDouble();
    mainData.eff_filter=ui->lineEdit_9->text().toDouble();
    mainData.concentrations[0]=ui->lineEdit_4->text().toDouble();
    mainData.concentrations[1]=ui->lineEdit_5->text().toDouble();
    mainData.concentrations[2]=ui->lineEdit_6->text().toDouble();
    _dataHandle->setMainWindowData(mainData);
}

void MainWindow::getMainData(){
    const MainWindowData mainData=_dataHandle->getMainWindowData();
    ui->lineEdit_7->setText(TOOLS::convertToString(mainData.filt_time).c_str());
    ui->lineEdit_8->setText(TOOLS::convertToString(mainData.volume).c_str());
    ui->lineEdit_9->setText(TOOLS::convertToString(mainData.eff_filter).c_str());
    ui->lineEdit->setText(TOOLS::convertToString(mainData.lambda[0]).c_str());
    ui->lineEdit_2->setText(TOOLS::convertToString(mainData.lambda[1]).c_str());
    ui->lineEdit_3->setText(TOOLS::convertToString(mainData.lambda[2]).c_str());
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About"), tr("Program for radon progenies concentration estimation."
                                             "You can find a detail information in doc\\html\\index.html "
                                             "or doc\\latex\\refman.pdf or send me a "
                                             "<a href=mailto:boris.bulanek@suro.cz>mail</a>."));
}

void MainWindow::on_actionOpen_database_triggered(){

    DataHandle::IS_NEW=0;
    QString fname=QFileDialog::getOpenFileName(this,tr("Directory"),_dataHandle->getDatabasePath().c_str());
    _dataHandle->openDb(fname.toStdString());
    if (!_dataHandle->getDb().isOpen()) return;


    ui->commandLinkButton_2->setEnabled(1);

    SqlConnection sql;
    sql.onlyForOpen();
    if (sql.exec()==QDialog::Accepted){
        //ui->pushButton->setEnabled(1);
        ui->commandLinkButton_2->setEnabled(1);
        ui->commandLinkButton_4->setEnabled(1);

        this->getMainData();
        const vector<Data> dataVec=_dataHandle->getData();



        this->setWindowTitle(TITLE+"; Database entry name: "+QString(fname));
    }
}

void MainWindow::on_commandLinkButton_4_clicked()
{
    const vector<Data> aData=_dataHandle->getData();

    QDialog dialog;
    dialog.setWindowTitle("Signal data table");
    QTableWidget tableWidget(&dialog);
    tableWidget.setColumnCount(5);
    tableWidget.setRowCount(aData.size());
    QStringList horizontalList;
    horizontalList<<"signal"<<"startTime"<<"timeDelta"<<"det_eff"<<"type";
    tableWidget.setHorizontalHeaderLabels(horizontalList);
    for (unsigned i = 0; i < aData.size(); ++i) {
        tableWidget.setItem(i,0,new QTableWidgetItem(TOOLS::convertToString(aData[i]._signal).c_str()));
        tableWidget.setItem(i,1,new QTableWidgetItem(TOOLS::convertToString(aData[i]._aTime).c_str()));
        tableWidget.setItem(i,2,new QTableWidgetItem(TOOLS::convertToString(aData[i]._timeDelta).c_str()));
        tableWidget.setItem(i,3,new QTableWidgetItem(TOOLS::convertToString(aData[i]._efficiency).c_str()));
        tableWidget.setItem(i,4,new QTableWidgetItem(TOOLS::convertToString(aData[i]._type).c_str()));
    }
    dialog.setMinimumWidth(tableWidget.horizontalHeader()->length()+60);
    tableWidget.setMinimumWidth(tableWidget.horizontalHeader()->length()+60);//tableWidget.verticalHeader()->length()+100);
    tableWidget.setMaximumHeight(800);
    dialog.setMaximumHeight(500);
    QGridLayout layout(&dialog);
    layout.addWidget(&tableWidget,2,1);
    dialog.exec();
}

void MainWindow::on_commandLinkButton_5_clicked()
{
    this->setMainData();
    Progeny_time_dependence aProgenyTimeDep;
    aProgenyTimeDep.exec();
}
