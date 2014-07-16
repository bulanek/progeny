#include "sqlhandle.h"
#include <QDateTime>

 using namespace std;


/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
SqlHandle* SqlHandle::instance=0;

/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
SqlHandle* SqlHandle::getInstance()
{
    if(instance==0){
        instance=new SqlHandle();
    }
    _dataHandle=DataHandle::getInstance();
    return instance;
}


/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
DataHandle* SqlHandle::_dataHandle=DataHandle::getInstance();



/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
void SqlHandle::createMainTables(){
    QSqlQuery query;
    bool queryReturn;
    const MainWindowData mainData=_dataHandle->getMainWindowData();
    string execStr=string("create table if not exists INFO_1(")
                           +"id INTEGER primary key autoincrement,"
                           +"name VARCHAR(20),"
                           +"eff_filter DOUBLE,"
                           +"filt_time DOUBLE,"
                           +"volume DOUBLE,";
    for (int i = 0; i < 3; ++i) {
        execStr+="l"+TOOLS::convertToString(i+1)+" DOUBLE default "+TOOLS::convertToString(mainData.lambda[i])+",";
    }
    execStr+=string("save_time DATETIME default current_timestamp")
            +");";
    queryReturn=query.exec(execStr.c_str());

    if (!queryReturn){
        cout<<query.lastQuery().toStdString()<<endl;
        qDebug()<<"Cannot create table!";
    }

    _id=1;
    queryReturn=query.exec("select id from INFO_1;");
    if(!queryReturn){
        qDebug()<<"Error in ID selection!";
        qDebug()<<query.lastQuery();
    }else{
        while(query.next()) {
            _id= query.value(0).toInt()+1;
        }
    }
}

/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
void SqlHandle::insertIntoMainInfoTable(const MainWindowData& mainWindowData){

    string execStr=string("insert into INFO_1 (id,name,eff_filter,filt_time,volume,l1,l2,l3) values (")
                    +TOOLS::convertToString<int>(_id)+","
                    +"\""+mainWindowData.name+"\","
                     +"\""+TOOLS::convertToString<double>(mainWindowData.eff_filter)+"\","
                     +"\""+TOOLS::convertToString<double>(mainWindowData.filt_time)+"\","
                     +"\""+TOOLS::convertToString<double>(mainWindowData.volume)+"\","
                     +"\""+TOOLS::convertToString<double>(mainWindowData.lambda[0])+"\","
                     +"\""+TOOLS::convertToString<double>(mainWindowData.lambda[1])+"\","
                     +"\""+TOOLS::convertToString<double>(mainWindowData.lambda[2])
                    +");";
    QSqlQuery query;
    bool queryReturn=query.exec(execStr.c_str());
    if(!queryReturn){
        cout<<query.lastQuery().toStdString()<<endl;;
        qDebug()<<"Cannot write table!";
        qDebug()<<execStr.c_str();
    }
}





/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
void SqlHandle::createSecondTable(const int ID){
    QSqlQuery query;
    string info_2_name="INFO_2_"+TOOLS::convertToString<int>(ID);
    string execStr="create table if not exists "+info_2_name+"("
                    "num_m INTEGER primary key autoincrement,"
            "timeStart DOUBLE,"
            "timeDelta DOUBLE,"
            "eff DOUBLE,"
            "type TINYINT,"
            "area_all DOUBLE,"
            "area_A DOUBLE,"
            "area_C DOUBLE)";

    bool queryReturn=query.exec(execStr.c_str());
    if(!queryReturn){
        qDebug()<<query.lastQuery();
        qDebug()<<"Cannot create INFO_2!";
    }
}


/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
void SqlHandle::insertIntoSecondTable(const int ID){

    const string tableName="INFO_2_"+TOOLS::convertToString<int>(ID);
    QSqlQuery query;
    bool queryReturn;
    string execStr;
    const vector<Data> data=_dataHandle->getData();

    execStr="insert into "+tableName+"("
            "timeStart,"
            "timeDelta,"
            "eff,"
            "type,"
            "area_all,"
            "area_A,"
            "area_C"
        ") VALUES (";

    query.prepare(execStr.c_str());

    queryReturn=query.exec();

    if(!queryReturn){
        cout<<query.lastQuery().toStdString()<<endl;;
        qDebug()<<"Cannot insert into "<<tableName.c_str();
    }
    //        emit valueChanged(counter);

}


/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
const vector<Data> SqlHandle::getSqlData(const int ID){
    QSqlQuery q;
    bool queryReturn;
    MainWindowData mainData;
    queryReturn=q.exec("select id,filtration_time,air_volume, filter_efficiency from INFO_1;");
    if (!queryReturn){
        qDebug()<<q.lastQuery();
        qDebug()<<"Cannot read table "<<"INFO_1!";
    }
    while (q.next()){
        if(q.value(0).toInt()==ID) break;
    }
    QSqlRecord mainRec=q.record();
    mainData.eff_filter=q.value(mainRec.indexOf("filter_efficiency")).toDouble();
    mainData.filt_time=q.value(mainRec.indexOf("filtration_time")).toDouble();
    mainData.volume=q.value(mainRec.indexOf("air_volume")).toDouble();

    for(int i=0;i<3;++i){
        mainData.lambda[i]=_dataHandle->getMainWindowData().lambda[i]; /* TODO not very clever idea, if there will be lambda in db..*/
    }
    //mainData.lambda[0]=q.value(mainRec.indexOf("l1")).toDouble();
    //mainData.lambda[1]=q.value(mainRec.indexOf("l2")).toDouble();
    //mainData.lambda[2]=q.value(mainRec.indexOf("l3")).toDouble();

    _dataHandle->setMainWindowData(mainData);

    vector<Data> dataVec;
    QSqlQuery q1;
    string infoName="INFO_2";//+TOOLS::convertToString<int>(ID);
    string queryStr="select "
            "signal,"
            "startTime,"
            "timeDelta,"
            "detector_efficiency,"
            "type,"
            "id"
            " from "+infoName +" WHERE id=="+TOOLS::convertToString(ID);
    queryReturn=q1.exec(queryStr.c_str());
    QSqlRecord record=q1.record();
    if(!queryReturn){
        qDebug()<<q1.lastQuery();
        qDebug()<<"Cannot read table "<<infoName.c_str()<<"!";
    }else{
        int counter=0;
        while (q1.next()){
            Data data;
            data=this->getDataFromQuery(q1,record);
            dataVec.push_back(data);
            ++counter;
        }
    }
    return dataVec;
}


/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
Data SqlHandle::getDataFromQuery(const QSqlQuery& query,const QSqlRecord& record){
    Data data;
    data._aTime=query.value(record.indexOf("startTime")).toDouble();
    data._timeDelta=query.value(record.indexOf("timeDelta")).toDouble();
    data._efficiency=query.value(record.indexOf("detector_efficiency")).toDouble();
    data._type=query.value(record.indexOf("type")).toDouble();
    data._signal=query.value(record.indexOf("signal")).toDouble();
   return data;
}


/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
void SqlHandle::deleteMeasurement(const int ID){
    QSqlQuery query;
    string queryStr="select num_m from INFO_2_"+TOOLS::convertToString<int>(ID);
    bool queryReturn;
    query.exec(queryStr.c_str());
    queryStr="drop table if exists INFO_2_"+TOOLS::convertToString<int>(ID);
    queryReturn=query.exec(queryStr.c_str());
    if(!queryReturn){
        qDebug()<<"Cannot drop table INFO_2!";
        qDebug()<<query.lastQuery();
    }
}
