#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

unsigned int seed = 0;
int	NowYear=2022;		// 2022 - 2027
int	NowMonth =0;		// 0 - 11
float	NowPrecip = 1;		// inches of rain per month
float	NowTemp = 32;		// temperature this month
float	NowHeight = 1.;		// grain height in inches
int	NowNumDeer = 1;		// number of deer in the current population
int NowRadiation = 0;
const float GRAIN_GROWS_PER_MONTH =		9.0;
const float ONE_DEER_EATS_PER_MONTH =		1.0;
const float AVG_PRECIP_PER_MONTH =		7.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise
const float AVG_TEMP =				60.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise
const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;

float
SQR( float x )
{
        return x*x;
}

float
Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int
Ranf( unsigned int *seedp, int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;

        return (int)(  Ranf(seedp, low,high) );
}

// call this if you want to force your program to use
// a different random number sequence every time you run it:
void
TimeOfDaySeed( )
{
  struct tm y2k = { 0 };
  y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
  y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

  time_t  timer;
  time( &timer );
  double seconds = difftime( timer, mktime(&y2k) );
  unsigned int seed = (unsigned int)( 1000.*seconds );    // milliseconds
  srand( seed );
}

void Deer(){


	while( NowYear < 2028 ){

		// DoneComputing barrier:
		/* compute next deer amount */
		int nextNumDeer = NowNumDeer;
		int carryingCapacity = (int)( NowHeight );
		if( nextNumDeer < carryingCapacity ){
		  nextNumDeer++;
		}
		else if( nextNumDeer > carryingCapacity ){
		  nextNumDeer--;
		}

		if(NowRadiation == 100) nextNumDeer = 0;
		if( nextNumDeer < 0 )
		 nextNumDeer = 0;
		

		#pragma omp barrier
		NowNumDeer = nextNumDeer;
		// DoneAssigning barrier:
		#pragma omp barrier

		// DonePrinting barrier:
		#pragma omp barrier
	}
}

void Grain(){
	/* compute next gra	in height */
	while( NowYear < 2028 ){

		// compute a temporary next-value for this quantity
		// based on the current state of the simulation:
		float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
		float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );
		float nextHeight = NowHeight;
 		nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
	 	nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
		if( nextHeight < 0. ) nextHeight = 0.; 
		// DoneComputing barrier:
		#pragma omp barrier
		
		NowHeight = nextHeight;
		// DoneAssigning barrier:
		#pragma omp barrier

		// DonePrinting barrier:
		#pragma omp barrier
}

}

void Watcher(){

	while(NowYear < 2028){

		// DoneComputing barrier:
		#pragma omp barrier
		
		// DoneAssigning barrier:
		#pragma omp barrier
		
		fprintf(stdout, 
		"%f,%f,%d,%f,%d,%d,%d\n", 
		(5./9.)*(NowTemp-32),
		NowHeight*2.54,
		NowNumDeer,NowPrecip,
		NowRadiation,
		NowMonth,NowYear);

		float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

		float temp = AVG_TEMP - AMP_TEMP * cos( ang );
		NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

		float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
		NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
		if( NowPrecip < 0. )
			NowPrecip = 0.;
		

		if((NowMonth+1) % 12 == 0){
			NowYear += 1;
		}
		NowMonth +=1;
		// DonePrinting barrier:
		#pragma omp barrier
	
	}
}
void MyAgent(){
	/* 	
			Less grain means less food. Hungry workers means 
			more chance to make mistakes when operating 
			RBMK-1000 Nuclear reactors. So, when there is less grain,
			there are random Cherenobyl 2.0 events that are more
			likely to happen (a lot of radiation).

			Assuming that grains are not affected by radiation,
			the deer population becomes 0. 
			The fallout only stays for one month.
		*/
	
	while( NowYear < 2028 ){

		// DoneComputing barrier:
		int nextDeer = NowNumDeer;
		int nextRadiation = NowRadiation;
		bool isCheronobyl = false;
		int roll1 = Ranf(&seed, 0, (int)NowHeight);
		int roll2 = Ranf(&seed, 0, (int)NowHeight);
		if(roll1 == roll2) isCheronobyl = true;
		if(isCheronobyl){
			nextDeer = 0;
			nextRadiation = 100;
		}
		else{
			nextRadiation = 0;
		}
		#pragma omp barrier
		NowRadiation = nextRadiation;
		// DoneAssigning barrier:
		#pragma omp barrier

		// DonePrinting barrier:
		#pragma omp barrier
	}
}

int main(){

	fprintf(stdout, "temp,grain height,num deer,percip,radiaton,month,year\n");
	omp_set_num_threads( 4 );	// same as # of sections
	#pragma omp parallel sections
	{
	#pragma omp section
	{
		Deer( );
	}

	#pragma omp section
	{
		Grain( );
	}

	#pragma omp section
	{
		Watcher( );
	}

	#pragma omp section
	{
		MyAgent( );	// your own
	}
	
}       // implied barrier -- all functions must return in order
	// to allow any of them to get past here

	return 0;
}