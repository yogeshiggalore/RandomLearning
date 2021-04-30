#ifndef _HEADER_ESPTIMER_H_
#define _HEADER_ESPTIMER_H_

#include <Arduino.h>

/* unlocking timer mux */
//extern portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#define MAX_TIMERS  	7

#define TIMER_1_MS 		0
#define TIMER_50_MS 	1
#define TIMER_100_MS 	2
#define TIMER_500_MS 	3
#define TIMER_1000_MS 	4
#define TIMER_10000_MS  5
#define TIMER_60000_MS  6

/* hardware timer */
extern hw_timer_t * timer;

extern bool timeout;

struct TimeSlot{    
    uint32_t ui32Milisecond;
    uint32_t ui32Downcounter;
    bool 	 bTimeoutFlag;
};

extern struct TimeSlot tdfTimeSlot[MAX_TIMERS];

//Functions
void esptimer_start(void);
void IRAM_ATTR esptimer_callback(void);
void esptimer_time_check(void);
void esptimer_timeout_1ms(void);
void esptimer_timeout_50ms(void);
void esptimer_timeout_100ms(void);
void esptimer_timeout_500ms(void);
void esptimer_timeout_1000ms(void);
void esptimer_timeout_10s(void);
void esptimer_timeout_60s(void);
#endif