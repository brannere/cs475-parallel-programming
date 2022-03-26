
#include <omp.h>
#include <stdio.h>
#include <math.h>

#define NUMT	         4	// number of threads to use
#define SIZE       	16384  // array size -- you get to decide
#define NUMTRIES        33	// how many times to run the timing -- you get to decide



/* ./run NUM_THREADS ARR_SIZE NUM_TRILAS */
int
main(int argc, char* argv[])
{
#ifndef _OPENMP
        fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
        return 1;
#endif

        if(argc < 4){
                fprintf(stderr, "Not enough args\n");
                fprintf(stderr, "Usage: NUM_THREADS ARR_SIZE NUM_TRILAS\n");
                return 1;
        }
        const unsigned int NUM_THREADS = atoi(argv[1]);
        const unsigned int ARR_SIZE = atoi(argv[2]);
        const unsigned int NUM_TRIALS = atoi(argv[3]);
        float A[ARR_SIZE];
        float B[ARR_SIZE];
        float C[ARR_SIZE];
	// inialize the arrays:
	for( int i = 0; i < ARR_SIZE; i++ )
	{
		A[i] = 1.;
		B[i] = 2.;
	}

        omp_set_num_threads( NUM_THREADS );
        fprintf( stderr, "Using %d threads\n", NUM_THREADS );

        double maxMegaMults = 0.;

        for( int t = 0; t < NUM_TRIALS; t++ )
        {
                double time0 = omp_get_wtime( );

                #pragma omp parallel for
                for( int i = 0; i < ARR_SIZE; i++ )
                {
                        C[i] = A[i] * B[i];
                }

                double time1 = omp_get_wtime( );
                double megaMults = (double)ARR_SIZE/(time1-time0)/1000000.;
                if( megaMults > maxMegaMults )
                        maxMegaMults = megaMults;
        }

        printf( "Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults );

	// note: %lf stands for "long float", which is how printf prints a "double"
	//        %d stands for "decimal integer", not "double"

        return 0;
}
