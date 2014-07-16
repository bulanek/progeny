#ifndef TOOLS_H
#define TOOLS_H


#include	<sstream>
#include	<cmath>
#include	<string>
#include <vector>
#include <set>
#include <sstream>
#include <iostream>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlin.h>

#include "progenyMatrix.hh"

using namespace std;

class Data{
public:
    double _aTime;
    double _timeDelta;
    double _signal;
    double _signalA;
    double _signalC;
    double _eSignal;
    double _eSignalA;
    double _eSignalC;
    double _efficiency;
    double _eff_filter;
    double _timeFiltration;
    double _volumeFiltered;
    int _type;
    set<int> _isotopes;
    inline friend ostream& operator<<(ostream& stream,const Data& aData){
        stream<<"Data: "<<endl;
        stream<<"\t aTime: "<<aData._aTime<<endl
             <<"\t timeDelta: "<<aData._timeDelta<<endl;
//        if(aData._type==1){
//            stream<<"\t signal: "<<aData._signalA<<"+-"<<aData._eSignalA<<endl;
//       }else if(aData._type==3){
//          stream<<"\t signal: "<<aData._signalC<<"+-"<<aData._eSignalC<<endl;
//     }else{
//          stream<<"\t signal: "<<aData._signal<<"+-"<<aData._eSignal<<endl;
//      }
        stream<<"\t signal: "<<aData._signal<<"+-"<<aData._eSignal<<endl;
           stream<<"\t efficiency: "<<aData._efficiency<<endl
          <<"\t eff_filter; "<<aData._eff_filter<<endl
         <<"\t timeFiltration: "<<aData._timeFiltration<<endl
        <<"\t volumeFiltered: "<<aData._volumeFiltered<<endl
        <<"\t type: "<<aData._type<<endl;
        return stream;
    }
};
struct InputData{
    vector<Data> aData;
    ProgenyMatrix* progenyMatrix;
};

namespace TOOLS{

template <class T>
string convertToString(const T value){
    stringstream strStream;
    cout<<value<<endl;
    strStream<<value;
    return strStream.str();
}

int chiSquare(const gsl_vector* xGSL,void* params,gsl_vector* f);
int chiSquareDer(const gsl_vector* xGSL,void* params,gsl_matrix* J);
int chiSquare_fdf(const gsl_vector * params, void *data, gsl_vector * f, gsl_matrix * J);

void print_state (size_t iter, gsl_multifit_fdfsolver* s);
}


#endif // TOOLS_H
