//Timed fields Class
//Version 1.0 by Adalberto H. Llarena & David Cortez
 
#ifndef _THREADTIME_
#define _THREADTIME_

#ifdef WIN32
#include <time.h>
#else
#include <sys/time.h>
#endif

#ifdef WIN32

/***************************/

/* time between jan 1, 1601 and jan 1, 1970 in units of 100 nanoseconds
*/
#define TIMESPEC_TO_FILETIME_OFFSET (((LONGLONG)27111902 << 32) + (LONGLONG)3577643008)

static void
timespec_to_filetime(const struct timespec *ts, FILETIME *ft)
     /*
      *-------------------------------------------------------------------
	  * converts struct timespec
	  * where the time is expressed in seconds and nanoseconds from Jan 1,1970.
	  * into FILETIME (as set by GetSystemTimeAsFileTime), where the time is
	  * expressed in 100 nanoseconds from Jan 1, 1601,
      *-------------------------------------------------------------------
	  */
{
	*(LONGLONG *)ft = ts->tv_sec * 10000000 + (ts->tv_nsec + 50) / 100 + TIMESPEC_TO_FILETIME_OFFSET;
}

static void
filetime_to_timespec(const FILETIME *ft, struct timespec *ts, int nanodelay)
     /*
      *-------------------------------------------------------------------
	  * converts FILETIME (as set by GetSystemTimeAsFileTime), where the time is
	  * expressed in 100 nanoseconds from Jan 1, 1601,
	  * into struct timespec
	  * where the time is expressed in seconds and nanoseconds from Jan 1,1970.
      *-------------------------------------------------------------------
	  */
{
	ts->tv_sec = (int)((*(LONGLONG *)ft - TIMESPEC_TO_FILETIME_OFFSET) / 10000000);
	ts->tv_nsec = (int)((*(LONGLONG *)ft - TIMESPEC_TO_FILETIME_OFFSET - ((LONGLONG)ts->tv_sec *(LONGLONG)10000000)) * 100 + nanodelay);
}

#endif /* not win32 */

void delay(double seconds)
{
	clock_t ti,tf,t;
	ti = clock();
	do
	{
		tf = clock();
		t = tf - ti;
	}while((float)((float)t/(float)CLOCKS_PER_SEC) < seconds);
}

void thread_sleep(double seconds)
{
	pthread_mutex_t m;
	pthread_cond_t c;
	struct timespec to;
	int secs = (int) floor (seconds);
	int nanoseconds = (int) ((seconds-secs) * 1000000000);

	pthread_mutex_init (&m, NULL);
	pthread_cond_init (&c, NULL);

#ifdef WIN32
	FILETIME fti;
	SYSTEMTIME sti;
	GetSystemTime(&sti);
	SystemTimeToFileTime(&sti, &fti);
	//GetSystemTimeAsFileTime(&ft); would be faster, but it does not exist on WinCE 
	filetime_to_timespec(&fti, &to, nanoseconds);
#else
	struct timeval now;
	gettimeofday(&now, NULL);
	to.tv_sec = now.tv_sec;
	to.tv_nsec = now.tv_usec * 1000 + nanoseconds;
#endif

	to.tv_sec += secs;
	pthread_mutex_lock (&m);

	//waits
	{pthread_cond_timedwait (&c, &m, &to);}

	pthread_mutex_unlock (&m);
	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&c);
}

#endif