#include <cstdio>
#include <cstdlib>
#include "cloverLEDcontroller.h"
#include "cloverTimeTools.h"

int main(int argc, char *argv[])
{
	printf("Blinky. Usage: [cycle period in ms] [dutyCycle %%] [total-on-time]\n");
	CloverLedController clc;

	int cyclePeriod_ms = 200;
	float  dutyCycle = 50.0, numSecond = 5;

	if( argc >= 2 ) {
		cyclePeriod_ms = atoi( argv[1] );
	}
	if( argc >= 3 ) {
		dutyCycle = atof( argv[2] );
	}
	if ( argc >=4 ) {
		numSecond = atof( argv[3] );
	}

	printf("Cycle period=%d ms, DutyCycle = %.1f %%, On time=%.3f seconds.\n",cyclePeriod_ms, dutyCycle, numSecond );
	
	clc.run4someSeconds( cyclePeriod_ms, dutyCycle, numSecond );

	printf( "Done. Turning LED off.\n" );
	sleep( 1 );

	FILE *test = fopen("/sys/class/gpio/gpio156/value","w+");
	fprintf( test, "%d", 0 );
	fclose( test );

	return 0;
}

