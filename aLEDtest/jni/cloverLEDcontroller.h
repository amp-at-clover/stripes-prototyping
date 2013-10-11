#ifndef __CLOVER_LED_CONTROLLER_H__
#define __CLOVER_LED_CONTROLLER_H__

#include "cloverTimeTools.h"
#include <time.h>
#include <cstdlib>
#include <cstdio>

#define MAX_GPIO_STR_LEN 60

using CloverTools::TimeTools;

class CloverLedController
{
    private:
        bool ledOnState;
        int percentOn;
        int cyclePeriod_ms;
        TimeTools ourTimer;
        char ledPIN[ MAX_GPIO_STR_LEN ];

        void setPinStr( int gpioPin ) {
            sprintf( ledPIN, "/sys/class/gpio/gpio%d/value", gpioPin );
        }

        FILE *fLED;

    public:
        CloverLedController() {
            percentOn = 100;
            cyclePeriod_ms = 1000L;
            ledOnState = false;
            int GPIOPIN  = 156;
            setPinStr( GPIOPIN );
            fLED = NULL;
        }

        ~CloverLedController() {
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

        void run4aSecond( int cyclePeriod_ms, int dutyCycle  )
        {
           	fLED = fopen( "/sys/class/gpio/gpio156/value", "w" );
            ourTimer.resetTimer();
			if( cyclePeriod_ms > 1000 ) cyclePeriod_ms = 1000;

            if( dutyCycle > 100 ) dutyCycle = 100;
            if( dutyCycle < 0 ) dutyCycle   = 0;
            float onTime = (dutyCycle/100.0 * cyclePeriod_ms);
            float offTime = (cyclePeriod_ms - onTime);
			printf("OnTime=%f, OffTime=%f\n",onTime, offTime);
            while( ourTimer.timeSinceStart() <= 1.0  ) {
                    turnOnLED( onTime );
                    turnOffLED( offTime );
            }
            fclose( fLED );
			fLED = NULL;
        }
};

#endif // __CLOVER_LED_CONTROLLER_H__
