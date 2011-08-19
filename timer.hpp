// Timer (.Hpp)
// ------------

#ifndef __Timer_hpp__
#define __Timer_hpp__ 

struct timer_t   
{
 LARGE_INTEGER tstart, tticks, tnow, tlast;
 int high_freq, logic_frames, logic_fps, gfx_frames, gfx_fps;
};

extern struct timer_t timer;

void start_timer(void);
void reset_timer(void);
int check_timer(int frac_sec);
void reset_timer_alt(void);

#endif
