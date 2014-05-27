#include	"tools.h"

using namespace std;
namespace TOOLS{
/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
int chiSquare(const gsl_vector* xGSL,void* params,gsl_vector* f){
    double parameters[2]={gsl_vector_get(xGSL,0),gsl_vector_get(xGSL,1)};

    InputData* inputData=(InputData*)params;
    vector<Data> dataVec=inputData->aData;

    int n=dataVec.size();
    double delta=0;
    double sqrtError=0;
    double par[3];
    for (int i = 0; i < 3; ++i) {
        par[i]=gsl_vector_get(xGSL,i);
    }
    double progenyNumbers;

    for (unsigned i=0;i<dataVec.size(); i++) {
        progenyNumbers=0;
    switch(dataVec[i]._type){
    case 0:{
        progenyNumbers=inputData->progenyMatrix->getNumParticles(0,par,dataVec[i]._aTime,dataVec[i]._timeFiltration,dataVec[i]._timeDelta,dataVec[i]._volumeFiltered);
        progenyNumbers+=inputData->progenyMatrix->getNumParticles(2,par,dataVec[i]._aTime,dataVec[i]._timeFiltration,dataVec[i]._timeDelta,dataVec[i]._volumeFiltered);
        delta=progenyNumbers-dataVec[i]._signal/(dataVec[i]._efficiency*dataVec[i]._eff_filter);
        sqrtError=abs(dataVec[i]._signal/(dataVec[i]._efficiency*dataVec[i]._eff_filter));
        break;
    }
    case 1:{
        progenyNumbers=inputData->progenyMatrix->getNumParticles(0,par,dataVec[i]._aTime,dataVec[i]._timeFiltration,dataVec[i]._timeDelta,dataVec[i]._volumeFiltered);
        delta=progenyNumbers-dataVec[i]._signalA/(dataVec[i]._efficiency*dataVec[i]._eff_filter);
        sqrtError=abs(dataVec[i]._signalA/(dataVec[i]._efficiency*dataVec[i]._eff_filter));

        break;
    }
    case 2:{
        progenyNumbers=inputData->progenyMatrix->getNumParticles(1,par,dataVec[i]._aTime,dataVec[i]._timeFiltration,dataVec[i]._timeDelta,dataVec[i]._volumeFiltered);

        delta=progenyNumbers-dataVec[i]._signal/(dataVec[i]._efficiency*dataVec[i]._eff_filter);
        sqrtError=abs(dataVec[i]._signal/(dataVec[i]._efficiency*dataVec[i]._eff_filter));

        break;
    }
    case 3:{
        progenyNumbers=inputData->progenyMatrix->getNumParticles(2,par,dataVec[i]._aTime,dataVec[i]._timeFiltration,dataVec[i]._timeDelta,dataVec[i]._volumeFiltered);
        delta=progenyNumbers-dataVec[i]._signalC/(dataVec[i]._efficiency*dataVec[i]._eff_filter);
        sqrtError=abs(dataVec[i]._signalC/(dataVec[i]._efficiency*dataVec[i]._eff_filter));

        break;
    }
    case 4:{
        progenyNumbers=par[3]*inputData->progenyMatrix->getNumParticles(1,par,dataVec[i]._aTime,dataVec[i]._timeFiltration,dataVec[i]._timeDelta,dataVec[i]._volumeFiltered);
        progenyNumbers+=par[4]*inputData->progenyMatrix->getNumParticles(2,par,dataVec[i]._aTime,dataVec[i]._timeFiltration,dataVec[i]._timeDelta,dataVec[i]._volumeFiltered);
        delta=progenyNumbers-dataVec[i]._signal/(dataVec[i]._efficiency*dataVec[i]._eff_filter);
        sqrtError=abs(dataVec[i]._signal/(dataVec[i]._efficiency*dataVec[i]._eff_filter));
    }
    }
        gsl_vector_set(f,i,delta/sqrtError);
    }
    return GSL_SUCCESS;
}


/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
int chiSquareDer(const gsl_vector* xGSL,void* params,gsl_matrix* J){
    double parameters[2]={gsl_vector_get(xGSL,0),gsl_vector_get(xGSL,1)};

    InputData* inputData=(InputData*)params;
    vector<Data> dataVec=inputData->aData;

    int n=int(dataVec.size());
    double delta=0;
    double sqrtError=0;
    double par[3];
    double derValue=0;

    for (unsigned i=0;i<dataVec.size(); i++) {
        for (int j = 0; j < 3; ++j) {
            for(int k=0;k<3;++k){
                par[k]=0;
                if(k==j) par[k]=1;
            }

            derValue=0;

            switch(dataVec[i]._type){
            case 0:
            {
                delta  = dataVec[i]._signal/(dataVec[i]._efficiency*dataVec[i]._eff_filter);
                sqrtError=abs(dataVec[i]._signal/dataVec[i]._efficiency*dataVec[i]._eff_filter);
                derValue=inputData->progenyMatrix->getNumParticles(0,par,dataVec[i]._aTime,dataVec[i]._timeFiltration,dataVec[i]._timeDelta,dataVec[i]._volumeFiltered);
                derValue+=inputData->progenyMatrix->getNumParticles(2,par,dataVec[i]._aTime,dataVec[i]._timeFiltration,dataVec[i]._timeDelta,dataVec[i]._volumeFiltered);
                break;
            }
            case 1:{
                delta  = dataVec[i]._signalA/(dataVec[i]._efficiency*dataVec[i]._eff_filter);
                sqrtError=abs(dataVec[i]._signalA/dataVec[i]._efficiency*dataVec[i]._eff_filter);
                derValue=inputData->progenyMatrix->getNumParticles(0,par,dataVec[i]._aTime,dataVec[i]._timeFiltration,dataVec[i]._timeDelta,dataVec[i]._volumeFiltered);
                break;
            }
            case 2:{
                delta  = dataVec[i]._signal/(dataVec[i]._efficiency*dataVec[i]._eff_filter);
                sqrtError=abs(dataVec[i]._signal/dataVec[i]._efficiency*dataVec[i]._eff_filter);
                derValue=inputData->progenyMatrix->getNumParticles(1,par,dataVec[i]._aTime,dataVec[i]._timeFiltration,dataVec[i]._timeDelta,dataVec[i]._volumeFiltered);
                break;
            }
            case 3:{
                delta  = dataVec[i]._signalC/(dataVec[i]._efficiency*dataVec[i]._eff_filter);
                sqrtError=abs(dataVec[i]._signalC/dataVec[i]._efficiency*dataVec[i]._eff_filter);
                derValue=inputData->progenyMatrix->getNumParticles(2,par,dataVec[i]._aTime,dataVec[i]._timeFiltration,dataVec[i]._timeDelta,dataVec[i]._volumeFiltered);
                break;
            }
            case 4:{
                delta  = dataVec[i]._signal/(dataVec[i]._efficiency*dataVec[i]._eff_filter);
                sqrtError=abs(dataVec[i]._signal/dataVec[i]._efficiency*dataVec[i]._eff_filter);
                derValue=par[3]*inputData->progenyMatrix->getNumParticles(1,par,dataVec[i]._aTime,dataVec[i]._timeFiltration,dataVec[i]._timeDelta,dataVec[i]._volumeFiltered);
                derValue+=par[4]*inputData->progenyMatrix->getNumParticles(2,par,dataVec[i]._aTime,dataVec[i]._timeFiltration,dataVec[i]._timeDelta,dataVec[i]._volumeFiltered);
                break;
            }
            }
            gsl_matrix_set(J,i,j,derValue/sqrtError);
        }
    }
    return GSL_SUCCESS;
}
/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
int chiSquare_fdf(const gsl_vector * params, void *data, gsl_vector * f, gsl_matrix * J)
{
    chiSquare(params, data, f);
    chiSquareDer(params, data, J);

    return GSL_SUCCESS;
}

/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
void print_state (size_t iter, gsl_multifit_fdfsolver* s)
{
    printf ("iter: %3u x = % 15.8f % 15.8f % 15.8f "
            "|f(x)| = %g\n",
            iter,
            gsl_vector_get (s->x, 0),
            gsl_vector_get (s->x, 1),
            double(gsl_blas_dnrm2 (s->f)));
}
}
