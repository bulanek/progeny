#include "datahandle.h"
#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <climits>

#include	"boost/program_options/variables_map.hpp"
#include	"boost/program_options/parsers.hpp"
#include	"boost/algorithm/string.hpp"
#include	"boost/algorithm/string/split.hpp"

using namespace std;

DataHandle* DataHandle::instance=0;
ProgenyMatrix* DataHandle::progeny=0;
bool DataHandle::IS_NEW=0;
double DataHandle::TIME_NOT_A=1000;
double DataHandle::AC_CORRECTION=0;



gsl_vector* DataHandle::_results=gsl_vector_alloc(3);
gsl_matrix* DataHandle::_covMat=gsl_matrix_alloc(3,3);


DataHandle* DataHandle::getInstance()
{
    if(instance==0){
        instance=new DataHandle();
        progeny=new ProgenyMatrix();
    }
    return instance;
}
ProgenyMatrix* DataHandle::getProgeny(){
    if(progeny==0){
        progeny=new ProgenyMatrix();
    }
    return progeny;
}


/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
int DataHandle::getConfigurationData(const string& confDataName){
    QDomDocument doc("xml");
    QFile file(confDataName.c_str());

    if(!file.open(QFile::ReadOnly | QFile::Text)) return 0;

    if(!doc.setContent(&file)) {
        file.close();
        return 0;
    }
    file.close();
    _confDom=doc;
    QDomElement root=doc.documentElement();
    QDomNode n = root.firstChild();
    string databasePath,binPath;
    while( !n.isNull() )
    {
        QDomElement e = n.toElement();
        if( !e.isNull() )
        {
            string tagName=e.tagName().toStdString();
            string text=e.text().toStdString();
            if (tagName=="DatabasePath"){
                _databasePath=text;
            }else if(tagName=="DatabaseName"){
                _databaseName=text;
            }
        }
        n = n.nextSibling();
    }
    cout<<endl;
    cout<<"Obtained configuration file data from: "<<confDataName<<endl;
    cout<<"\t Database name: "<<_databaseName<<endl;
    return 1;
}

/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
int DataHandle::createDataFromTxt(string name){

    ifstream inFile(name.c_str());
    if(inFile.is_open()==0) return 1;
    string line;
    vector<string> lineArray;
    Data aData;
    int isotopes;
    getline(inFile,line);
    while(inFile>>aData._signal>>aData._aTime>>aData._timeDelta>>aData._efficiency>>aData._type){
        aData._eSignal=sqrt(abs(aData._signal));
        switch(isotopes){
            case 0:
                aData._isotopes.insert(0),aData._isotopes.insert(2);
                break;
            case 1:
                aData._isotopes.insert(0);
                break;
            case 2:
                aData._isotopes.insert(2);
                break;
            case 3:
                aData._isotopes.insert(1),aData._isotopes.insert(2);
                break;
        }
        _dataVec.push_back(aData);
    }
    _inputData.aData=_dataVec;
    _inputData.progenyMatrix=progeny;
    return 0;
}

/*-----------------------------------------------------------------------------
 *  
 *-----------------------------------------------------------------------------*/
double DataHandle::computeACCorrection(){

    double acCorrection=0;
    double weight=0;
    double weightAll=0;
    for ( unsigned i=0;i<_dataVec.size() ;++i ) {
        if(_dataVec[i]._signalA!=0 && _dataVec[i]._signalC!=0 &&_dataVec[i]._aTime>TIME_NOT_A){
            weight=pow(_dataVec[i]._signalA/_dataVec[i]._signalC,2)*(1/sqrt(_dataVec[i]._signalA)+1/sqrt(_dataVec[i]._signalC));
            acCorrection+=_dataVec[i]._signalA*weight;
            weightAll+=weight;
        }
    }
    if(weightAll!=0) acCorrection/=weightAll;
    AC_CORRECTION=acCorrection;
}

/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
int DataHandle::createChiSquareInputData(){
    for (int i = 0; i < _dataVec.size(); ++i) {
        _dataVec[i]._eff_filter=_mainWindowData.eff_filter;
        _dataVec[i]._timeFiltration=_mainWindowData.filt_time;
        _dataVec[i]._volumeFiltered=_mainWindowData.volume;
    }
    _inputData.aData=_dataVec;
    _inputData.progenyMatrix=progeny;
    return 1;
}


/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
int DataHandle::openDb(string dbName){

    if(!_db.open()){
        _db=QSqlDatabase::addDatabase("QSQLITE");
        _db.setDatabaseName(QString(dbName.c_str()));
    }else{
        _db.setDatabaseName(QString(dbName.c_str()));
    }
    bool ok=_db.open();
    cout<<"Connecting to db..."<<endl;

    if(ok){
        cout<<"In database: "<<dbName<<endl;
    }else{
        cout<<"Cannot open database "<<dbName<<endl;
        return 0;
    }
}

/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
DataHandle* DataHandle::setMainWindowData(MainWindowData& mainWindowData){
    _mainWindowData=mainWindowData;
    progeny->setCoeficients(mainWindowData.lambda);
    return this;
}
/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
DataHandle* DataHandle::setTimeDependenceWindowData(TimeDependenceWindowData& timeData){
    _timeDependenceWindowData=timeData;
    return this;
}

/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
int DataHandle::chiSquareComputeGSL(double* initialParameters){
    this->createChiSquareInputData();

    const gsl_multifit_fdfsolver_type *T;
    gsl_multifit_fdfsolver *s;
    int status;
    size_t iter=0;

    int(*usedFuncResidual)(const gsl_vector*,void*,gsl_vector*) =NULL;
    int(*usedFuncDer)(const gsl_vector*,void*,gsl_matrix*) =NULL;
    int(*usedFunc_fdf)(const gsl_vector*,void*,gsl_vector*,gsl_matrix*) =NULL;
    size_t p=3;

    usedFuncResidual=&TOOLS::chiSquare;
    usedFuncDer=&TOOLS::chiSquareDer;

    usedFunc_fdf=&TOOLS::chiSquare_fdf;
    const size_t n=_inputData.aData.size();

    for (int i = 0; i < _inputData.aData.size(); ++i) {
        cout<<_inputData.aData[i]<<endl;
    }


    //    gsl_matrix* covar=gsl_matrix_alloc(p,p);
    gsl_multifit_function_fdf fun;

    gsl_vector_view x=gsl_vector_view_array(initialParameters,p);

    fun.f=usedFuncResidual;
    fun.fdf=usedFunc_fdf;
    fun.df=usedFuncDer;
    fun.n=_inputData.aData.size();
    fun.p=p;
    fun.params=&_inputData;

    if( n<p ){
        throw 1;
    }

    T=gsl_multifit_fdfsolver_lmder;
    s=gsl_multifit_fdfsolver_alloc(T,n,p);
    gsl_multifit_fdfsolver_set(s,&fun,&x.vector);

    TOOLS::print_state(iter,s);

    do
    {
        iter++;
        status = gsl_multifit_fdfsolver_iterate(s);
        TOOLS::print_state (iter, s);
        printf ("status = %s\n", gsl_strerror (status));

        TOOLS::print_state (iter, s);
        status = gsl_multifit_test_delta (s->dx, s->x,
                1e-4, 1e-4);

    }
    while (iter < 500&& status==GSL_CONTINUE);



    double chi = gsl_blas_dnrm2(s->f);




    gsl_multifit_covar (s->J, 0.0, _covMat);

    gsl_matrix_fprintf (stdout, _covMat, "%g");

#define FIT(i) gsl_vector_get(s->x, i)
#define ERR(i) sqrt(gsl_matrix_get(_covMat,i,i))

    printf ("status = %s\n", gsl_strerror (status));
    for (unsigned i=0;i<p;++i) {
        gsl_vector_set(_results,i,FIT(i));
    }
    gsl_multifit_fdfsolver_free(s);
    return GSL_SUCCESS;
}

/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
const vector<Data>& DataHandle::getData(){
    return _dataVec;
}
/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
ostream& operator<<(ostream& stream,const DataHandle& dataHandle){
    stream<<"Main information: "<<endl;
    stream<<"\tNumber of measurements: "<<dataHandle._dataVec.size()<<endl;
    return stream;
}
ostream& operator<<(ostream& stream,const DataHandle* dataHandle){
    stream<<*dataHandle<<endl;
    return stream;
}
