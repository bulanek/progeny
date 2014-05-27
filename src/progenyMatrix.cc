/**
  @brief

  Detailed description follows here.

  @author	Boris Bulanek (),
  @date	07/25/13
  */
// =====================================================================================

#include	"progenyMatrix.hh"


#include	<cmath>
#include	<cstdio>
#include	<iostream>

#include	"gsl/gsl_linalg.h"

/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
ProgenyMatrix::ProgenyMatrix(const double* lambda,const double T ){
    this->setCoeficients(lambda,T);
}
/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
void ProgenyMatrix::setCoeficients(const double* lambda,const double T){
    for ( int i=0 ;i<NUM_R ;++i ) {
        for ( int j=0;j<NUM_R ;++j ) {
            for ( int k=0;k<NUM_R ;++k ) {
                _coefficients[i][j][k]=0;
                _coefficients2[i][j][k]=0;
            }
        }
    }
    _T=T;

    for ( int i=0;i<NUM_R ;++i) {
        _lambda[i]=lambda[i];
        _oneMinusExp[i]=1-exp(-_lambda[i]*_T);
    }

    _lambda21=_lambda[1]-_lambda[0];
    _lambda31=_lambda[2]-_lambda[0];
    _lambda32=_lambda[2]-_lambda[1];


    /*  my */
    _coefficients[0][0][0]=1/_lambda[0];
    _coefficients[1][0][0]=1/_lambda21;          /*  - ?? */
    _coefficients[1][1][0]=-_lambda[0]/_lambda[1]/_lambda21; /*  -->+?? */
    _coefficients[1][1][1]=_lambda21/_lambda[1]/_lambda21;
    _coefficients[2][0][0]=_lambda[1]/_lambda31/_lambda21;
    _coefficients[2][1][0]=-_lambda[0]/_lambda32/_lambda21;
    _coefficients[2][1][1]=_lambda21/_lambda32/_lambda21;
    _coefficients[2][2][0]=_lambda[0]*_lambda[1]/_lambda[2]/_lambda32/_lambda31;
    //_coefficients[2][2][1]=-_lambda[2]*_lambda31/_lambda[2]/_lambda32/_lambda31;
    _coefficients[2][2][1]=-_lambda[1]*_lambda31/_lambda[2]/_lambda32/_lambda31;
    _coefficients[2][2][2]=_lambda31*_lambda32/_lambda[2]/_lambda32/_lambda31;

    /*  coefficients for progeny without source with N1,N2,N3 in. */
    _coefficients2[0][0][0]=1.;
    _coefficients2[1][0][0]=_lambda[0]/_lambda21;
    _coefficients2[1][1][0]=-_lambda[0]/_lambda21;
    _coefficients2[1][1][1]=1.;
    _coefficients2[2][0][0]=_lambda[0]*_lambda[1]/_lambda31/_lambda21;
    _coefficients2[2][1][0]=-_lambda[0]*_lambda[1]/_lambda32/_lambda21;
    _coefficients2[2][1][1]=_lambda[1]/_lambda32;
    _coefficients2[2][2][0]=_lambda[0]*_lambda[1]/_lambda31/_lambda32;
    _coefficients2[2][2][1]=-_lambda[1]/_lambda32;
    _coefficients2[2][2][2]=1.;
}

/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
void ProgenyMatrix::test(){
    printf ( "Test of consistency\n" );

    double value[]={0,0,0};
    double conc[]={1,1,1};
    double aTime=1.;
    for ( int j=0;j<NUM_R ;++j ) {
        for ( int k=0;k<NUM_R ;++k ) {
            value[0]+=(-_lambda[j]*_coefficients[0][j][k]*conc[k]*_oneMinusExp[j]*exp(-_lambda[j]*aTime))+_lambda[0]*_coefficients[0][j][k]*conc[k]*_oneMinusExp[j]*exp(-_lambda[j]*aTime);
            value[1]+=(-_lambda[j]*_coefficients[1][j][k]*conc[k]*_oneMinusExp[j]*exp(-_lambda[j]*aTime))-_lambda[0]*_coefficients[0][j][k]*conc[k]*_oneMinusExp[j]*exp(-_lambda[j]*aTime)+_lambda[1]*_coefficients[1][j][k]*conc[k]*_oneMinusExp[j]*exp(-_lambda[j]*aTime);
            value[2]+=(-_lambda[j]*_coefficients[2][j][k]*conc[k]*_oneMinusExp[j]*exp(-_lambda[j]*aTime))-_lambda[1]*_coefficients[1][j][k]*conc[k]*_oneMinusExp[j]*exp(-_lambda[j]*aTime)+_lambda[2]*_coefficients[2][j][k]*conc[k]*_oneMinusExp[j]*exp(-_lambda[j]*aTime);
        }
    }

    for ( int i=0;i<NUM_R ;++i ) {
        printf ( "Test %i: %.12f\n",i,value[i] );
    }
}


/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
vector<double> ProgenyMatrix::getConcFromInfAlphas(const double* par){
    double aLeftSide[]={par[0],par[2],par[4]};

    double aMatrix[3][3];
    for ( int i=0;i<3 ;++i ) {
        for ( int j=0;j<3 ;++j ) {
            aMatrix[i][j]=0;
        }
    }

    for ( int i=0;i<NUM_R ;++i ) {
        for ( int j=0;j<NUM_R ;++j ) {
            aMatrix[i][j]=(_lambda[0]*_coefficients[0][i][j]+_lambda[2]*_coefficients[2][i][j])*_oneMinusExp[i]/_lambda[i];
        }
    }

    gsl_matrix_view m = gsl_matrix_view_array (&aMatrix[0][0], NUM_R, NUM_R);
    gsl_vector_view b = gsl_vector_view_array (aLeftSide, NUM_R);
    gsl_vector *x = gsl_vector_alloc (NUM_R);

    int s;
    gsl_permutation * p = gsl_permutation_alloc (NUM_R);
    gsl_linalg_LU_decomp (&m.matrix, p, &s);
    gsl_linalg_LU_solve (&m.matrix, p, &b.vector, x);

    //       printf ("x = \n");
    //       gsl_vector_fprintf (stdout, x, "%g");

    gsl_permutation_free (p);

    vector<double> returnVec;

    for ( int i=0;i<NUM_R ;++i ) {
        returnVec.push_back(gsl_vector_get(x,i));
    }
    gsl_vector_free (x);
    return returnVec;
}
/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
vector<double> ProgenyMatrix::getConcFromAlphas(const double* par,const double& timeDelta){
    double aLeftSide[]={par[0],par[2],par[4]};

    double aMatrix[NUM_R][NUM_R];
    for ( int i=0;i<NUM_R ;++i ) {
        for ( int j=0;j<NUM_R ;++j ) {
            aMatrix[i][j]=0;
        }
    }

    for ( int i=0;i<NUM_R ;++i ) {
        for ( int j=0;j<NUM_R ;++j ) {
            aMatrix[i][j]=(_coefficients[0][i][j]+_coefficients[2][i][j])*_oneMinusExp[i]/_lambda[i]*(1-exp(-_lambda[i]*timeDelta));
        }
    }

    gsl_matrix_view m = gsl_matrix_view_array (&aMatrix[0][0], NUM_R, NUM_R);
    gsl_vector_view b = gsl_vector_view_array (aLeftSide, NUM_R);
    gsl_vector *x = gsl_vector_alloc (NUM_R);

    int s;
    gsl_permutation * p = gsl_permutation_alloc (NUM_R);
    gsl_linalg_LU_decomp (&m.matrix, p, &s);
    gsl_linalg_LU_solve (&m.matrix, p, &b.vector, x);

    //       printf ("x = \n");
    //       gsl_vector_fprintf (stdout, x, "%g");

    gsl_permutation_free (p);

    vector<double> returnVec;

    for ( int i=0;i<NUM_R ;++i ) {
        returnVec.push_back(gsl_vector_get(x,i));
    }
    gsl_vector_free (x);
    return returnVec;
}
/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
double ProgenyMatrix::getActivityFilter(const int& which, const double* conc, const double& aTime, const double & volumeFiltered, const double&  timeFiltration){

    double returnValue=0;
    if(aTime>=0){
    for ( int i=0;i<NUM_R ;++i ) {
        for ( int j=0;j<NUM_R ;++j ) {
            returnValue+=_coefficients[which][i][j]*(1-exp(-_lambda[i]*timeFiltration))/_lambda[j]*conc[j]*(volumeFiltered/timeFiltration)*exp(-_lambda[i]*aTime);
        }
    }
    }else if(aTime>-timeFiltration && aTime<0){
     for ( int i=0;i<NUM_R ;++i ) {
        for ( int j=0;j<NUM_R ;++j ) {
            returnValue+=_coefficients[which][i][j]*(1-exp(-_lambda[i]*(timeFiltration+aTime)))/_lambda[j]*conc[j]*(volumeFiltered/timeFiltration);
        }
    }
    }else{
        returnValue=0;
    }
    return _lambda[which]*returnValue;
}

/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
double ProgenyMatrix::getNumParticles(const int& which,const double* conc,const double& aTime, const double timeDelta){
    double returnValue=0;
    for ( int i=0;i<NUM_R ;++i ) {
        for ( int j=0;j<NUM_R ;++j ) {
            if(timeDelta!=0){
                returnValue+=_coefficients[which][i][j]*_oneMinusExp[i]/_lambda[i]*conc[j]*exp(-_lambda[i]*aTime)*(1-exp(-_lambda[i]*timeDelta));
            }else{
                returnValue+=_coefficients[which][i][j]*_oneMinusExp[i]/_lambda[i]*conc[j]*exp(-_lambda[i]*aTime);
            }
        }
    }
    return _lambda[which]*returnValue;
}

/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
double ProgenyMatrix::getNumParticles(const int& which,const double* conc,const double& aTime, const double timeDelta,const double volumeFiltered,double timeFiltration){
    if(timeFiltration==0) timeFiltration=_T;
    double returnValue=0;
    for ( int i=0;i<NUM_R ;++i ) {
        for ( int j=0;j<NUM_R ;++j ) {
            if(timeDelta!=0){
                returnValue+=_coefficients[which][i][j]*(1-exp(-_lambda[i]*timeFiltration))*conc[j]/_lambda[j]/_lambda[i]*(volumeFiltered/timeFiltration)*exp(-_lambda[i]*aTime)*(1-exp(-_lambda[i]*timeDelta));
            }else{
                returnValue+=_coefficients[which][i][j]*(1-exp(-_lambda[i]*timeFiltration))*conc[j]/_lambda[j]/_lambda[i]*(volumeFiltered/timeFiltration)*exp(-_lambda[i]*aTime);
            }
        }
    }
    return _lambda[which]*returnValue;
}

/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
vector<double> ProgenyMatrix::getProgenyWihoutFilter(const double* N,const double t){
    vector<double> returnVec(3,0);

    for ( int i=0;i<NUM_R ;++i ) {
        for (int j=0 ;j<NUM_R ;++j ) {
            for ( int k=0;k<NUM_R ;++k ) {
                returnVec[i]+=_coefficients2[i][j][k]*_oneMinusExp[j]*exp(-_lambda[j]*t)*N[k];
            }
        }
    }

    return returnVec;
}


/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
vector<double> ProgenyMatrix::getNumCreatedParticles(const double *N,const double t, const double timeDelta){

    vector<double> returnVec(3,0);
    for ( int i=0;i<NUM_R ;++i ) {
        for (int j=0 ;j<NUM_R ;++j ) {
            for ( int k=0;k<NUM_R ;++k ) {
                returnVec[i]+=_lambda[i]*_coefficients2[i][j][k]*exp(-_lambda[j]*t)/_lambda[j]*(1-exp(-_lambda[j]*timeDelta))*N[k];
            }
        }
    }
    return returnVec;
}

/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
vector<double> ProgenyMatrix::getNumParticles(const double *N,const double timeDelta){
    vector<double> returnVec(3,0);
    for ( int i=0;i<NUM_R ;++i ) {
        for (int j=0 ;j<NUM_R ;++j ) {
            for ( int k=0;k<NUM_R ;++k ) {
                returnVec[i]+=_coefficients2[i][j][k]*exp(-_lambda[j]*timeDelta)*N[k];
            }
        }
    }
    return returnVec;

}


/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
ostream& operator<<(ostream& stream,const ProgenyMatrix& progenyMatrix){
    stream<<"Progeny Matrix: "<<endl;
    for ( int i=0 ;i<NUM_R ;++i ) {
        stream<<"Coeficients for source "<<i<<":"<<endl;
        for ( int j=0;j<NUM_R ;++j ) {
            for ( int k=0;k<NUM_R ;++k ) {
                stream<<progenyMatrix._coefficients[i][j][k]<<",";
            }
            stream<<endl;
        }
        stream<<endl;
    }
    return stream;
}
/*-----------------------------------------------------------------------------
 *
 *-----------------------------------------------------------------------------*/
ostream& operator<<(ostream& stream,const ProgenyMatrix* progenyMatrix){
    stream<<*progenyMatrix;
    return stream;
}

