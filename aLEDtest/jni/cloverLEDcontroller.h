#ifndef __CLOVER_LED_CONTROLLER_H__
#define __CLOVER_LED_CONTROLLER_H__

#include "cloverTimeTools.h"
#include <time.h>
#include <cstdlib>
#include <cstdio>

#define MAX_GPIO_STR_LEN 60
#define GPIOPIN 156

using CloverTools::TimeTools;

class CloverLedController
{
    private:
        TimeTools ourTimer;
        char ledPIN[ MAX_GPIO_STR_LEN ];
        FILE *fLED;

        void setPinStr( int gpioPin ) {
            sprintf( ledPIN, "/sys/class/gpio/gpio%d/value", gpioPin );
        }


        void turnOnLED( int milliSeconds ) {
            if ( milliSeconds < 0 ) milliSeconds = 0;
            long msAtStart = ourTimer.msTimer();
            while( ourTimer.msTimer() - msAtStart < milliSeconds ) {
            	fprintf(fLED,"1");
				fflush( fLED );
                usleep( 10 );
            }
        }

        void turnOffLED( int milliSeconds ) {
            if ( milliSeconds < 0 ) milliSeconds = 0;
                long msAtStart = ourTimer.msTimer();
                while( ourTimer.msTimer() - msAtStart < milliSeconds ) {
            		fprintf(fLED,"0");
					fflush( fLED );
                    usleep( 10 );
                }
        }

    public:
        CloverLedController() {
            setPinStr( GPIOPIN );
            fLED = NULL;
        }

        ~CloverLedController() {
        }


        void run4someSeconds( int cyclePeriod_ms, float dutyCycle , float secondsToRunFor )
        {
           	fLED = fopen( "/sys/class/gpio/gpio156/value", "w" );
            ourTimer.resetTimer();
			if( cyclePeriod_ms > 1000 ) cyclePeriod_ms = 1000;

            if( dutyCycle > 100 ) dutyCycle = 100;
            if( dutyCycle < 0 ) dutyCycle   = 0;
            float onTime = (dutyCycle/100.0 * cyclePeriod_ms);
            float offTime = (cyclePeriod_ms - onTime);
			printf("OnTime=%f, OffTime=%f\n",onTime, offTime);
            while( ourTimer.timeSinceStart() <= secondsToRunFor  ) {
                    turnOnLED( onTime );
                    turnOffLED( offTime );
            }
            fclose( fLED );
			fLED = NULL;
        }
};

#endif // __CLOVER_LED_CONTROLLER_H__
