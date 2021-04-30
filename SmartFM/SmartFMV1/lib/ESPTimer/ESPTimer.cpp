#include <Arduino.h>
#include <ESPTimer.h>
#include <LogLevel.h>
#include <logger.h>
#include <WiFiControl.h>
#include <ESPGPIO.h>

bool timeout = false;

struct TimeSlot tdfTimeSlot[MAX_TIMERS] = {
    {1, 1, false},           //TIMER_1_MS    
    {50, 50, false},         //TIMER_50_MS    
    {100, 100, false},      //TIMER_100_MS
    {500, 500, false},      //TIMER_500_MS
    {1000, 1000, false},    //TIMER_1000_MS
	{10000, 10000, false},    //TIMER_10000_MS
	{60000,60000,false}
};   

/* This function sets 1 milisecond timer */
void esptimer_start(void){

	/* set timer0 to 80MHz */
	timer = timerBegin(0, 80, true);

	/* attach interrupt to timer */
	timerAttachInterrupt(timer, &esptimer_callback, true);

	/* 1 ms interrupt */
	timerAlarmWrite(timer, 1000, true);

	/* enable timer */ 
	timerAlarmEnable(timer);
}

/* callback function 1ms interrupt */
void IRAM_ATTR esptimer_callback(void){
    uint8_t ui8LoopCounter=0;

	for (ui8LoopCounter = 0; ui8LoopCounter < MAX_TIMERS; ui8LoopCounter++)
    {
        tdfTimeSlot[ui8LoopCounter].ui32Downcounter--;
        if (tdfTimeSlot[ui8LoopCounter].ui32Downcounter <= 0){
            tdfTimeSlot[ui8LoopCounter].ui32Downcounter = tdfTimeSlot[ui8LoopCounter].ui32Milisecond;
            tdfTimeSlot[ui8LoopCounter].bTimeoutFlag = true;
        }
    }
}

void esptimer_time_check(void) {
	//1ms Activities
	if (tdfTimeSlot[TIMER_1_MS].bTimeoutFlag == true) {
		tdfTimeSlot[TIMER_1_MS].bTimeoutFlag = false;
        esptimer_timeout_1ms();
		eio.led_timer();
	}

	//50ms Activities
	if (tdfTimeSlot[TIMER_50_MS].bTimeoutFlag == true) {
		tdfTimeSlot[TIMER_50_MS].bTimeoutFlag = false;
        esptimer_timeout_50ms();
	}

	//100ms Activities
	if (tdfTimeSlot[TIMER_100_MS].bTimeoutFlag == true) {
		tdfTimeSlot[TIMER_100_MS].bTimeoutFlag = false;
        esptimer_timeout_100ms();
	}

	//
	if (tdfTimeSlot[TIMER_500_MS].bTimeoutFlag == true) {
		tdfTimeSlot[TIMER_500_MS].bTimeoutFlag = false;
        esptimer_timeout_500ms();
	}

	if (tdfTimeSlot[TIMER_1000_MS].bTimeoutFlag == true) {
		tdfTimeSlot[TIMER_1000_MS].bTimeoutFlag = false;
        esptimer_timeout_1000ms();
	}

	if (tdfTimeSlot[TIMER_10000_MS].bTimeoutFlag == true) {
		tdfTimeSlot[TIMER_10000_MS].bTimeoutFlag = false;
        esptimer_timeout_10s();
	}

	if (tdfTimeSlot[TIMER_60000_MS].bTimeoutFlag == true) {
		tdfTimeSlot[TIMER_60000_MS].bTimeoutFlag = false;
        esptimer_timeout_60s();
	}
}

void esptimer_timeout_1ms(void){
    
}

void esptimer_timeout_50ms(void){
	
}

void esptimer_timeout_100ms(void){

}

void esptimer_timeout_500ms(void){
	
}

void esptimer_timeout_1000ms(void){
	/* Check wifi connection */

	//elog.Write(LogLevel::Info,"loop", "loop test");
}
void esptimer_timeout_10s(void){

}

void esptimer_timeout_60s(void){
	elog.Write(LogLevel::Error,"Timer", "Sync elog display timer");
	elog.set_time();
}
