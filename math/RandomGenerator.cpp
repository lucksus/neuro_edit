// Details im Header!
#include "RandomGenerator.h"
#include <cmath>
#include <math.h>
#include <ctime>
#include "math_constants.h"

static const double dbl_min = 1e-300;
static const double dbl_exp = 1e40;

NeuroMath::RandomGenerator::RandomGenerator()
: m_defaultIdum(DETERMINISTIC_SEED_IDUM)
{
		NewRandSeed();
		gaussAlreadyComputed = false;
}



NeuroMath::RandomGenerator::~RandomGenerator(){
}


void NeuroMath::RandomGenerator::seed(const qint32 idum){
	this->idum = idum;
	if (idum > 0)
	{
//		logOut.warning("RandomGenerator: seed is positive, but must be negative. Setting seed = -seed");
		this->idum *= -1;
	}
}


void NeuroMath::RandomGenerator::NewRandSeed(){
	struct tm *newtime;
	time_t ltime;
	clock_t cl;
	qint32 cl_int;

	qint32 tmp = 0;
	while(tmp >= 0){
		//first we take the absolute time
		time( &ltime );
		newtime = gmtime( &ltime );

		//second we take the processor clock
		cl = clock();
		cl_int = qint32(cl);

		tmp = newtime->tm_sec*cl_int * (-1);
	}

	seed(tmp);
}




NeuroMath::RandomGenerator*	NeuroMath::RandomGenerator::getInstance(){
        static NeuroMath::RandomGenerator instance;
	return &instance;
}


double	NeuroMath::RandomGenerator::uniform(double xmin, double xmax){
	return getInstance()->_uniform(xmin,xmax);
}

double	NeuroMath::RandomGenerator::gauss(double mean, double var){
	return getInstance()->_gauss(mean,var);
}

double	NeuroMath::RandomGenerator::poisson(double lambda){
	return getInstance()->_poisson(lambda);
}

double	NeuroMath::RandomGenerator::fastpoisson(double lambda){
	return getInstance()->_fastpoisson(lambda);
}



void	NeuroMath::RandomGenerator::setDeterministic(const qint32 idum){
	getInstance()->seed(idum);
	getInstance()->gaussAlreadyComputed = false;
}

void NeuroMath::RandomGenerator::setDeterministic() {
	setDeterministic( getDefaultSeed() );
}

void NeuroMath::RandomGenerator::setDefaultSeed(const qint32 idum) {
	if (idum > 0)
	{
//		logOut.warning("RandomGenerator: seed is positive, but must be negative. Setting seed = -seed");
		getInstance()->m_defaultIdum = -idum;
	} else
	{
		getInstance()->m_defaultIdum = idum;
	}
}

qint32 NeuroMath::RandomGenerator::getDefaultSeed() {
	return getInstance()->m_defaultIdum;
}


void	NeuroMath::RandomGenerator::setRandom(){
	getInstance()->NewRandSeed();
}


double NeuroMath::RandomGenerator::_uniform(double min_wert, double max_wert){
		double U = _shuffle(&idum);
		while (U == 0) U = _shuffle(&idum);
		double value = (max_wert-min_wert)*U+min_wert;
		return value;

}


// Ansgar: Die Funktion RandomGenerator::gauss() wurde am 22.Juli 2005 von Ansgar 1 Mio mal mit
// mean = 100 und var = 1 durchgenudelt.
// Die ausgespuckten 1 Mio Werte stimmen (auch bei logarithmischer Auftragung) exzellent mit der
// analytischen Gleichung für eine Gauss'sche Wahrscheinlichkeitsdichtefkt. überein.
/*
// create Gauss distribution by "Modified Acceptance/Rejection Method"
#include <limits>
double RandomGenerator::_gauss(const double mean, const double var){
	if (var == 0.0) return mean;

	double U1, U2, X, a, b;
	const double faktor = 1.0 / sqrt(2.0 * GPC::pi);
	const double faktor2 = 1.2 * faktor;
	const double sigma = sqrt(var);
	do
	{
		U1 = uniform(0.0, 1.0);


		if (U1 > 0.5)
			X = -2.0 * ::log(2.0 - 2.0*U1);
		else
			X = 2.0 * ::log(2.0*U1);

		b = faktor2 * exp( -fabs(X)/2.0 );
		U2 = uniform(0.0, b);

		a = faktor * exp( -GPC::sqr(X)/2.0 );
	} while ( U2 > a );
	return X*sigma + mean;
}
*/


double NeuroMath::RandomGenerator::_gauss(const double mean, const double var){

	static double gset;
	double fac,rsq,v1,v2;
	const double sigma = sqrt(var);
	
	if (gaussAlreadyComputed == false) {		//We dont have an extra deviate handy, so
		do {
			v1=2.0*_shuffle(&idum)-1.0;			//pick two uniform numbers in the square extending
			v2=2.0*_shuffle(&idum)-1.0; 		//from -1 to +1 in each direction,
			rsq=v1*v1+v2*v2;				//see if they are in the unit circle,
		} while (rsq >= 1.0 || rsq == 0.0); //and if they are not, try again.
			fac=sqrt(-2.0*log(rsq)/rsq);
			gset=v1*fac;					//Now make the Box-Muller transformation to get two normal deviates. Return one and
											//save the other for next time.
			gaussAlreadyComputed = true;	//Set flag.
			return v2*fac *sigma +mean;
	} else {								//We have an extra deviate handy,
		gaussAlreadyComputed = false;		//so unset the flag,
		return gset *sigma +mean;			//and return it.
	}
}

// create Poisson distributed random number by "Acceptance/Rejection Method"
double NeuroMath::RandomGenerator::_poisson(const double mean){
	double x, y;
	double z;
        const double max = 1.0 / sqrt(2.0*pi()*mean);
	//  ofstream o("poisson.dat");

	do {
		x = uniform(0.0, 2.0*mean);
		y = uniform(0.0, max);
                z = x * ::log(mean/x) - 0.5 * ::log(2.0*pi()*x) + (x-mean);
		z = exp(z);
	} while ( y > z ); /* enddo */

	return floor(x+0.5);
}

// create Poisson distribution by "Modified Acceptance/Rejection Method"

double NeuroMath::RandomGenerator::_fastpoisson(const double lambda){
	static double sq,alxm,g,oldm=(-1.0);
	double em,t,y;

	if (lambda < 12.0) {   // direct
		if (lambda != oldm) {
			oldm = lambda;
			g=exp(-lambda);
		}
		em = -1;
		t = 1.0;
		do {
			++em;
			t *= uniform(0,1);
		} while (t>g);
	} else {              // reject
		if (lambda != oldm) {
			oldm = lambda;
			sq = sqrt(2.0*lambda);
			alxm = ::log(lambda);
			g = lambda*alxm-_gammln(lambda+1.0);
		}
		do {
			do {
                                y = tan(pi()*uniform(0,1));
				em = sq*y+lambda;
			} while (em < 0.0);
			em = floor(em);
			t=0.9*(1.0+y*y)*exp(em*alxm-_gammln(em+1.0)-g);
		} while (uniform(0,1) > t);
	}

	return em;
}

double NeuroMath::RandomGenerator::_gammln(const double xx){
	double x,y,tmp,ser;
	static double cof[6]={76.18009172947146,-86.50532032941677,
		24.01409824083091,-1.231739572450155,
		0.1208650973866179e-2,-0.5395239384953e-5};
	int j;
	y=x=xx;
	tmp = x+5.5;
	tmp -= (x+0.5) * ::log(tmp);
	ser = 1.000000000190015;
	for (j=0;j<=5;j++) ser += cof[j]/++y;
	return -tmp + ::log(2.5066282746310005*ser/x);
}

double NeuroMath::RandomGenerator::_pareto(double MinValue, double MeanValue){
	const double a = MeanValue/(MeanValue-MinValue);
	const double MaxDensity = a/MinValue;
	const double gv = uniform(0.0, MaxDensity);

	return pow(a/gv, 1.0/(a+1.0))*pow(MinValue, a/(a+1.0));
}

double NeuroMath::RandomGenerator::pareto(double MinValue, double MeanValue){
	return getInstance()->_pareto(MinValue, MeanValue);
}



// Hier kommen die Zufallszahlen her.
// Liefert eine Zahl zwischen 0 und 1 (ohne Grenzen)
double NeuroMath::RandomGenerator::_shuffle(qint32 *idum)
{
	int j;
	qint32 k;
	static qint32 iy=0;
	static qint32 iv[NTAB];
	double temp;

	if (*idum <= 0 || !iy)
	{
		if (-(*idum) < 1) *idum=1;
		else *idum = -(*idum);
		for (j=NTAB+7; j>=0; j--)
		{
			k = *idum/IQ;
			*idum = IA*(*idum-k*IQ)-IR*k;
			if (*idum < 0) *idum += IM;
			if (j < NTAB) iv[j] = *idum;
		}
		iy=iv[0];
	}
	k=(*idum)/IQ;
	*idum=IA*(*idum-k*IQ)-IR*k;
	if (*idum < 0) *idum += IM;
	j=iy/NDIV;
	iy=iv[j];
	iv[j] = *idum;
	if ((temp=AM*iy) > RNMX) return RNMX;
	else return temp;

}



// Generation of Pareto distributed random numbers
//double RandomGenerator::_shuffle_pareto(double alpha, double A)
//{	
//	double denominator = pow(_shuffle(&idum),1/alpha);
//	while  (denominator == 0) denominator = pow(_shuffle(&idum), 1/alpha);
//	double value = (A/denominator);
//	return value;
//}


// Generation of Poisson distributed random numbers
//double RandomGenerator::_shuffle_poisson(double lambda)
//{
//	double k = 0;
//	double A = 1;
//	double U = _shuffle(&idum);
//	while (U == 0) U = _shuffle(&idum);
//	A = A*U;
//	while (A >= exp(-lambda)) 
//	{
//		k = k+1;
//		U = _shuffle(&idum);
//		while (U == 0) U = _shuffle(&idum);
//		A = A*U;
//	}
//	return k;
//}

// Generation of negative exponentially distributed random numbers
double NeuroMath::RandomGenerator::_negExp(double lambda)
{
	double U = _shuffle(&idum);
	while (U == 0) U = _shuffle(&idum);
	double value = /*floor(*/-lambda*log(U)/*)*/;
	return value;
}


double NeuroMath::RandomGenerator::negExp(double lambda){
	return getInstance()->_negExp(lambda);
}


void NeuroMath::RandomGenerator::initFromSimulationParameters(bool isDeterministic, bool isTransientCalc=false){
	if (isTransientCalc)
	{
		setRandom(); //Tom: Ist jetzt hier default.
	}
	else
	{
		if( isDeterministic)
			setDeterministic();
		else
			setRandom();
	}
}
