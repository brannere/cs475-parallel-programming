#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


// how many tries to discover the maximum performance:
#ifndef NUMNODES
#define NUMNODES	1000
#endif

#ifndef NUMT
#define NUMT	4
#endif

// You could use a single for-loop over all the nodes that looks like this:
// #pragma omp parallel for default(none) . . . // reduction here
// for( int i = 0; i < NUMNODES*NUMNODES; i++ )
// {
// 	int iu = i % NUMNODES;
// 	int iv = i / NUMNODES;
// 	double z = Height( iu, iv );
// 	. . .
// }

// . . .




// The code to evaluate the height at a given <i>iu</i> and <i>iv</i> is:

const double N = 2.5f;
const double R = 1.2f;

//  . . .

double
Height( int iu, int iv )	// iu,iv = 0 .. NUMNODES-1
{
	double x = -1.  +  2.*(double)iu /(double)(NUMNODES-1);	// -1. to +1.
	double y = -1.  +  2.*(double)iv /(double)(NUMNODES-1);	// -1. to +1.

	double xn = pow( fabs(x), (double)N );
	double yn = pow( fabs(y), (double)N );
	double rn = pow( fabs(R), (double)N );
	double r = rn - xn - yn;
	if( r <= 0. )
	        return 0.;
	double height = pow( r, 1./(double)N );
	return height;
}


// The main Program

#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

double Height( int, int );	// function prototype

double volume(double area, double height){
	return area * height;
}

int main( int argc, char *argv[ ] )
{
	#ifndef _OPENMP
        fprintf( stderr, "No OpenMP support!\n" );
        return 1;
	#endif
	double volume_sum = 0.;
	omp_set_num_threads( NUMT );
	// the area of a single full-sized tile:
	// (not all tiles are full-sized, though)

	double fullTileArea = (  ( ( XMAX - XMIN )/(double)(NUMNODES-1) )  *
				( ( YMAX - YMIN )/(double)(NUMNODES-1) )  );
	double maxPerformance = 0.;

	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for-loop and a reduction:

	/* NUMNODES^2 height samples */
	double time0 = omp_get_wtime( );
	#pragma omp parallel for default(none) shared(fullTileArea) reduction(+:volume_sum)
	for( unsigned long long int i = 0; i < NUMNODES*NUMNODES; i++ )
	{
		
		unsigned long long int iu = i % NUMNODES;
		unsigned long long int iv = i / NUMNODES;
		double z = Height( iu, iv );
		double vol = volume(z, fullTileArea);
		/* Corner */
		if(iu == 0 && iv == 0){
			vol = vol * 0.25;
		}
		/* Side */
		else if((iu == 0 && iv != 0) || (iu != 0 && iv == 0)){
			vol = vol * 0.5;
		}
		volume_sum += vol;

	}
	double time1 = omp_get_wtime( );
	double megaHeightComputePerSecond = (time1 - time0);
	maxPerformance = megaHeightComputePerSecond;
	
	fprintf(stdout, "%2d,%8d,%f,%6.2lf\n",
                NUMT, NUMNODES, volume_sum*2, maxPerformance); 

	fprintf(stderr, "%2d threads : %8d nodes ; volume = %f ; execution time = %6.2lf\n",
                NUMT, NUMNODES, volume_sum*2, maxPerformance); 
}
