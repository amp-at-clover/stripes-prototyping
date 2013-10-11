#ifndef __TIME_TOOLS_H__
#define __TIME_TOOLS_H__

#include <cctype>
#include <cstdlib>
#include <cerrno>
//#include <limits>
//#include <string>

#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
//#include <signal.h>


namespace CloverTools {
/** This class contains tools that are useful for obtaining the current time in
 * seconds, milliseconds and so on.
 */
class TimeTools {
private:
	double startTime;
public:
	TimeTools() {
		startTime = TimeTools::getEpochTime();
	}


	/** Provides current time in seconds. Useful when one needs to get a time-stamp.
 	 *  Resolution is in microseconds (accuracy might not be as high though).
	 * @return seconds since epoch (Jan 1 1970, 00:00:00.000000)
	 */
	static double getEpochTime() {
		struct timeval tv; double sec;
		gettimeofday( &tv, NULL );
		sec = (double)tv.tv_sec + (double) tv.tv_usec/1e6;
		return sec;
	}

	/** Provides the current time in milliseconds as a long. May be useful
	 * for providing integer time-stamps if the actual date/time are not
	 * that important. This might be useful for time-stamping packets.
	 *
	 */
	static unsigned long msTimer()
	{
		unsigned long myTimeIn_ms = (unsigned long long)(TimeTools::getEpochTime() * 1000.0)
						& 0xffffffff;
		return myTimeIn_ms;
	}

	/** Get the current time in seconds since the epoch */
	static unsigned long long get_secs()
	{
		return 	(unsigned long)TimeTools::getEpochTime();
	}

	/** Get the relative time since this class was first constructed
	 *
	 * @return seconds since construction of this TimeTools class instance.
	 */
	double timeSinceStart() {
		return (TimeTools::getEpochTime() - startTime);
	}

	/** Reset the timeSinceStart
	*/
	void resetTimer() {
	    startTime = TimeTools::getEpochTime();
	}

};

};

#endif // #define __TIME_TOOLS_H__
