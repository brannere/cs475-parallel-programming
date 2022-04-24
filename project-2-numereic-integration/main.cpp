#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <float.h>

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
// 	float z = Height( iu, iv );
// 	. . .
// }

// . . .




// The code to evaluate the height at a given <i>iu</i> and <i>iv</i> is:

const float N = 2.5f;
const float R = 1.2f;

//  . . .

float
Height( int iu, int iv )	// iu,iv = 0 .. NUMNODES-1
{
	float x = -1.  +  2.*(float)iu /(float)(NUMNODES-1);	// -1. to +1.
	float y = -1.  +  2.*(float)iv /(float)(NUMNODES-1);	// -1. to +1.

	float xn = pow( fabs(x), (double)N );
	float yn = pow( fabs(y), (double)N );
	float rn = pow( fabs(R), (double)N );
	float r = rn - xn - yn;
	if( r <= 0. )
	        return 0.;
	float height = pow( r, 1./(double)N );
	return height;
}


// The main Program

#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

float Height( int, int );	// function prototype

float volume(float area, float height){
	return area * height;
}

int main( int argc, char *argv[ ] )
{
	#ifndef _OPENMP
        fprintf( stderr, "No OpenMP support!\n" );
        return 1;
	#endif
	float volume_sum = 0.;

	// the area of a single full-sized tile:
	// (not all tiles are full-sized, though)

	float fullTileArea = (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  *
				( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );

	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for-loop and a reduction:

	/* NUMNODES^2 height samples */
	#pragma omp parallel for default(none) shared(fullTileArea) reduction(+:volume_sum)
	for( int i = 0; i < NUMNODES*NUMNODES; i++ )
	{
		int iu = i % NUMNODES;
		int iv = i / NUMNODES;
		float z = Height( iu, iv );
		float vol = volume(z, fullTileArea);
		/* Corner */
		if(iu == 0 && iv == 0){
			vol = vol * 0.25;
		}
		/* Side */
		else if((iu == 0 && iv != 0) || (iu != 0 && iv == 0)){
			vol = vol * 0.5;
		}
		volume_sum += vol;

		// . . .
	}
	fprintf(stdout, "Volume %f\n", volume_sum*2);
	// ?????
}
