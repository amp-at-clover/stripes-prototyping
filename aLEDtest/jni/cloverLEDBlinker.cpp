#include <cstdio>
#include <cstdlib>
#include "cloverLEDcontroller.h"
#include "cloverTimeTools.h"

int main(int argc, char *argv[])
{
	printf("Blinky. Usage: [cycle period in ms] [dutyCycle %%] [total-on-time]\n");
	CloverLedController clc;

	int cyclePeriod_ms = 200, dutyCycle = 50, numSecond = 5;

	if( argc >= 2 ) {
		cyclePeriod_ms = atoi( argv[1] );
	}
	if( argc >= 3 ) {
		dutyCycle = atoi( argv[2] );
	}
	if ( argc >=4 ) {
		numSecond = atoi( argv[3] );
	}

	printf("Cycle period=%d ms, DutyCycle = %d %%, On time=%d seconds.\n",cyclePeriod_ms, dutyCycle, numSecond );
	
	for ( int i=0; i < numSecond; i++ ) {
		clc.run4aSecond( cyclePeriod_ms, dutyCycle );
	}

	printf( "Done. Turning LED off.\n" );
	sleep( 1 );

	FILE *test = fopen("/sys/class/gpio/gpio156/value","w+");
	fprintf( test, "%d", 0 );
	fclose( test );

	return 0;
}

