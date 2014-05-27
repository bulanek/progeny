/**
  @brief	

  Detailed description follows here.

  @author	Boris Bulanek (), 
  @date	07/25/13
  */
// =====================================================================================
#ifndef PROGENYMATRIX_HH 
#define PROGENYMATRIX_HH 1
#define NUM_R 3

#include	<vector>
#include	<ostream>

using namespace std;
/*-----------------------------------------------------------------------------
 *  
 *-----------------------------------------------------------------------------*/
class ProgenyMatrix{
    public:
        ProgenyMatrix(){};
        ProgenyMatrix(const double* lambda,const double T=0);
        void setCoeficients(const double* lambda,const double T=0);

        vector<double> getConcFromInfAlphas(const double* par);
        vector<double> getConcFromAlphas(const double* par,const double& timeDelta);
        double getActivityFilter(const int& which, const double* conc, const double& aTime, const double & volumeFiltered, const double&  timeFiltration);

        double getNumParticles(const int& which,const double* conc,const double& aTime,const double timeDelta=0);
        double getNumParticles(const int& which,const double* conc,const double& aTime, const double timeDelta,const double volumeFiltered,double timeFiltration=0);

        vector<double> getProgenyWihoutFilter(const double* N,const double t);
        vector<double> getNumCreatedParticles(const double *N,const double t, const double timeDelta);
        vector<double> getNumParticles(const double *N,const double timeDelta);
        void test();

    private:
        double _lambda[NUM_R];
        double _t,_T;
        double _coefficients[3][3][3]; /*  N_n,exp[n],Kn */
        double _coefficients2[3][3][3]; /*  N_n,exp[n],Kn */

        double _oneMinusExp[3];
        double _denominator3[3];
        double _denominator2[2];
        double _denominator1;
        double _lambda21,_lambda31,_lambda32;
        friend ostream& operator<<(ostream& stream,const ProgenyMatrix& progenyMatrix);
        friend ostream& operator<<(ostream& stream,const ProgenyMatrix* progenyMatrix);

};
#endif
