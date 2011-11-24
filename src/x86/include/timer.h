/* 
  $Id: timer.h 67 2007-04-28 20:01:04Z raistlinthewiz $
*/
#ifndef __TIMER_H__
#define __TIMER_H__

#define PIT_CHAN1	0x40 /* PIT interval channel */
#define PIT_CHAN2	0x41 /* PIT system specific channel */
#define PIT_CHAN3	0x42 /* PIT system speaker channel */
#define PIT_CHAN_CMD	0x43 /* PIT command register */
#define PIT_CMD		0x36


#define TIMER_CLOCK 1193180 /* 1.19 Mhz, 1193180 Hz */
#define TIMER_FREQ  1000 /* 1000 Hz */

/* stop timer functions */
void set_timer(void);
void start_timer(void);
void stop_timer(void);

extern uint32 timer_ticks; /* PIT tick counter */
extern uint32 stop_time; /* stop_watch timer */

#endif

