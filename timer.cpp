#include <Windows.h>
#include "timer.hpp"



// High resolution timer code for Windows. Call start_timer first and then call 
// check_timer with the required accuracy range.

struct timer_t timer;

void start_timer(void) 
{
 timer.high_freq = QueryPerformanceFrequency(&timer.tticks);
 timer.logic_frames = 0;
 	
 if (timer.high_freq)	
 {
  QueryPerformanceCounter(&timer.tstart);
  timer.tlast = timer.tstart;	
 } 
} 

void reset_timer(void) 
{
 if (timer.high_freq)
 {
  QueryPerformanceCounter(&timer.tnow);
  timer.tstart = timer.tnow;
 } 
}

int check_timer(int frac_sec) 
{
 int t;

 if (timer.high_freq)
 {
  QueryPerformanceCounter(&timer.tnow);
  t = (int) (((timer.tnow.QuadPart - timer.tstart.QuadPart) * frac_sec) / timer.tticks.QuadPart);		
	  
  // Have we done 1 second since the timer was last reset?
  if (t > 240)
  {
   // Yes, so reset again and update details.
   // If we reset the timer after each check we get errors building up causing a lack
   // of precision. We also reset it here to make it as fast as possible.
   
   timer.tstart = timer.tlast;
   t -= timer.logic_frames;
   timer.logic_fps = timer.logic_frames;
   timer.logic_frames = 0;
	   
   timer.gfx_fps = timer.gfx_frames;
   timer.gfx_frames = 0;   
  }
  timer.tlast = timer.tnow;
    
  return t;
 }
 	
 return 0;
}

//#endif	 
 
/*
#if defined(ALLEGRO_MACOSX) || defined(ALLEGRO_UNIX) || defined(ALLEGRO_LINUX)
#include <sys/time.h>

static struct timeval tstart;
static struct timeval tlast;

void start_timer(void)
{
 gettimeofday(&tstart, NULL);
 tlast.tv_sec = tstart.tv_sec;
 tlast.tv_usec = tstart.tv_usec;
}

void reset_timer(void)
{
 gettimeofday(&tstart, NULL);
}

int check_timer(int frac_sec)
{
 struct timeval now;
 double hi, low;
 int t, ms;

 rest(0);
 gettimeofday(&now, NULL);
 
 hi = (double) (now.tv_sec - tstart.tv_sec);
 low = ((double) (now.tv_usec - tstart.tv_usec)) / 1.0e6;
 t = (int) ((hi + low) * ((double) frac_sec));

 if ( t > 240 ) 
 {
  tstart.tv_usec = tlast.tv_usec;
  tstart.tv_sec = tlast.tv_sec;
  
  t -= timer.logic_frames;
  timer.logic_fps = timer.logic_frames;
  timer.logic_frames = 0;
	   
  timer.gfx_fps = timer.gfx_frames;
  timer.gfx_frames = 0;   
 }

 tlast.tv_usec = now.tv_usec;
 tlast.tv_sec = now.tv_sec;
 return t;
}

#endif
*/