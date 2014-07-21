#ifndef DATAHANDLE_H
#define DATAHANDLE_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <string>

#include <sstream>
#include <cmath>


#include <QString>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QtXml>


//#include "tools.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlin.h>

#include "progenyMatrix.hh"
#include "tools.h"


using namespace std;

struct MainWindowData{
    string name;
    string dbName;
    double lambda[3];
    double concentrations[3];
    double filt_time;
    double eff_filter;
    double volume;
};

struct TimeDependenceWindowData{
    double efficiencyDetection[3];
    double activityTime;
    double detectedParticlesTimeRange[2];
    double activities[3];
    double detectedParticles[3];
    double plotXRange[2];
};



class DataHandle
{
public:
    static DataHandle* getInstance();
    static ProgenyMatrix* getProgeny();
    //const vector<vector<int> >& getDataPoints();
    const inline vector<Data>& getData(){return _dataVec;}
    int createDataFromTxt(string name);
    int openDb(string name);

    int getConfigurationData(const string& confDataName);
    int chiSquareComputeGSL(double* initialParameters);
    int createChiSquareInputData();
    double computeACCorrection();



    inline DataHandle& setDataVec(const vector<Data> dataVec){_dataVec=dataVec;_inputData.aData=dataVec;_inputData.progenyMatrix=progeny; return *this;} /* mad syntax..will be corrected*/
    inline const gsl_vector* getResults(){return _results;}
    inline const gsl_matrix* getCovMat(){return _covMat;}
    inline const QSqlDatabase& getDb(){return _db;}
    inline void setDatabasePath(const string& databasePath){_databasePath=databasePath;}
    inline const string& getDatabasePath() const{return _databasePath;}
    inline void setDatabaseName(const string& databaseName){_databaseName=databaseName;}
    inline const string& getDatabaseName() const{return _databaseName;}
    inline const MainWindowData& getMainWindowData() {return _mainWindowData;}
    inline const TimeDependenceWindowData& getTimeDependenceWindowData() {return _timeDependenceWindowData;}

    DataHandle* setMainWindowData(MainWindowData& data);
    DataHandle* setTimeDependenceWindowData(TimeDependenceWindowData& data);



    friend ostream& operator<<(ostream& stream,const DataHandle& dataHandle);
    friend ostream& operator<<(ostream& stream,const DataHandle* dataHandle);
    static bool IS_NEW;
    static double TIME_NOT_A;

private:
    DataHandle(){};
    static DataHandle* instance;
    static ProgenyMatrix* progeny;
    QSqlDatabase _db;
    QDomDocument _confDom;
    string _databaseName;
    string _databasePath;
    static double AC_CORRECTION;

    MainWindowData _mainWindowData;
    TimeDependenceWindowData _timeDependenceWindowData;
    vector<Data> _dataVec;
    InputData _inputData;
    static gsl_vector* _results;
    static gsl_matrix* _covMat;


};

#endif // DATAHANDLE_H
