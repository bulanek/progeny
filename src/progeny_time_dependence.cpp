#include "progeny_time_dependence.h"
#include "ui_progeny_time_dependence.h"

#include <QtGui>
#include <QMainWindow>
#include <QCheckBox>


Progeny_time_dependence::Progeny_time_dependence(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Progeny_time_dependence)
{
    ui->setupUi(this);

    /* graph interactions*/
    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->widget->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->widget->legend->setFont(legendFont);
    ui->widget->legend->setSelectedFont(legendFont);
    ui->widget->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items
    //connect(ui->widget, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
    // connect slot that ties some axis selections together (especially opposite axes):
    //connect(ui->widget, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    //connect(ui->widget, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));

    _dataHandle=DataHandle::getInstance();

    ui->widget->xAxis->setLabel("Time [s]");
    ui->widget->yAxis->setLabel("Activity on filter [Bq]");
    QCPAxis::ScaleType scale=QCPAxis::stLogarithmic;
    ui->widget->yAxis->setScaleType(scale );
    for (int i = 0; i < 3; ++i) {
        ui->widget->addGraph();
    }

    this->showPlot();
}

Progeny_time_dependence::~Progeny_time_dependence()
{
    delete ui;
}

void Progeny_time_dependence::showPlot(){
    TimeDependenceWindowData windowData=_dataHandle->getTimeDependenceWindowData() ;
    windowData.plotXRange[0]=ui->lineEdit_4->text().toDouble();
    windowData.plotXRange[1]=ui->lineEdit_5->text().toDouble();

    int NUM_POINTS=1000;
    MainWindowData mainWindowData=_dataHandle->getMainWindowData();

    ProgenyMatrix* progeny=DataHandle::getProgeny();
    QCustomPlot* plotWidget=ui->widget;

    string names[]={"RnA","RnB","RnC"};
    QVector<double> x(NUM_POINTS);
    QVector<double> y(NUM_POINTS);

    double aTime=0;
    for(int p=0;p<3;++p){
        plotWidget->graph(p)->setName(names[p].c_str());
        QPen pen;
        pen.setColor(QColor((Qt::GlobalColor)(7+p)));
        plotWidget->graph(p)->setPen(pen);
        for (int i = 0; i < x.size(); ++i) {
            aTime=windowData.plotXRange[0]+(windowData.plotXRange[1]-windowData.plotXRange[0])*i/double(NUM_POINTS);
            y[i]=progeny->getActivityFilter(p,mainWindowData.concentrations,aTime,mainWindowData.volume,mainWindowData.filt_time);
            x[i]=aTime;
        }
        plotWidget->graph(p)->setData(x,y);
    }
    plotWidget->rescaleAxes();

    QCPAxisRect axis(plotWidget);
    axis.setRangeDrag(Qt::Horizontal | Qt::Vertical);
    axis.setRangeZoom(Qt::Horizontal | Qt::Vertical);

    plotWidget->replot();
}

void Progeny_time_dependence::saveWindowData(){
    TimeDependenceWindowData windowData;
    windowData.efficiencyDetection[0]=ui->lineEdit->text().toDouble();
    windowData.efficiencyDetection[1]=ui->lineEdit_2->text().toDouble();
    windowData.efficiencyDetection[2]=ui->lineEdit_3->text().toDouble();
    windowData.activityTime=ui->lineEdit_7->text().toDouble();
    windowData.activities[0]=ui->lineEdit_8->text().toDouble();
    windowData.activities[1]=ui->lineEdit_9->text().toDouble();
    windowData.activities[2]=ui->lineEdit_10->text().toDouble();
    windowData.detectedParticlesTimeRange[0]=ui->lineEdit_11->text().toDouble();
    windowData.detectedParticlesTimeRange[1]=ui->lineEdit_12->text().toDouble();
    windowData.detectedParticles[0]=ui->lineEdit_13->text().toDouble();
    windowData.detectedParticles[1]=ui->lineEdit_14->text().toDouble();
    windowData.detectedParticles[2]=ui->lineEdit_15->text().toDouble();
    windowData.plotXRange[0]=ui->lineEdit_4->text().toDouble();
    windowData.plotXRange[1]=ui->lineEdit_5->text().toDouble();


    _dataHandle->setTimeDependenceWindowData(windowData);
}


void Progeny_time_dependence::on_commandLinkButton_3_clicked()
{
    this->showPlot();
}

void Progeny_time_dependence::on_commandLinkButton_clicked()
{
    ProgenyMatrix* progeny=DataHandle::getProgeny();
    TimeDependenceWindowData windowData=_dataHandle->getTimeDependenceWindowData() ;
    windowData.activityTime=ui->lineEdit_7->text().toDouble();
    MainWindowData mainWindowData=_dataHandle->getMainWindowData();
    double activities[3];
    for (int i = 0; i < 3; ++i) {
        activities[i]=
progeny->getActivityFilter(i,mainWindowData.concentrations,windowData.activityTime,mainWindowData.volume,mainWindowData.filt_time)*mainWindowData.eff_filter; }
    ui->lineEdit_8->setText(TOOLS::convertToString(activities[0]).c_str());
    ui->lineEdit_9->setText(TOOLS::convertToString(activities[1]).c_str());
    ui->lineEdit_10->setText(TOOLS::convertToString(activities[2]).c_str());
}

void Progeny_time_dependence::on_commandLinkButton_2_clicked()
{
    this->saveWindowData();
    ProgenyMatrix* progeny=DataHandle::getProgeny();
    TimeDependenceWindowData windowData=_dataHandle->getTimeDependenceWindowData() ;

    MainWindowData mainWindowData=_dataHandle->getMainWindowData();
    double numParticles[3];
    for (int i = 0; i < 3; ++i) {
        numParticles[i]= progeny->getNumParticles(i,mainWindowData.concentrations, windowData.detectedParticlesTimeRange[0],(windowData.detectedParticlesTimeRange[1]-windowData.detectedParticlesTimeRange[0]),mainWindowData.volume,mainWindowData.filt_time)*mainWindowData.eff_filter*windowData.efficiencyDetection[i];
    }
    ui->lineEdit_13->setText(TOOLS::convertToString(numParticles[0]).c_str());
    ui->lineEdit_14->setText(TOOLS::convertToString(numParticles[1]).c_str());
    ui->lineEdit_15->setText(TOOLS::convertToString(numParticles[2]).c_str());
}
