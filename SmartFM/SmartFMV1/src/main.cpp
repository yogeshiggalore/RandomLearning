/*******************************************************************************
* Project Name: SmartFMV1
*
* Version: 1.0
*
* Description:
* In this project FM application developed using Si470X 
*
*
* Owner
* Yogesh M Iggalore
********************************************************************************
* Copyright (2021-22) 
*******************************************************************************/

#include <Arduino.h>
#include <main.h>
#include <ESPTimer.h>
#include <WiFiControl.h>
#include <ESPGPIO.h>
#include <SPIFFS.h>
#include <FS.h>
#include <Application.h>


/* hardware timer */
hw_timer_t * timer;

/* unlocking timer mux */
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

LogLevel loglevel = LogLevel::Trace;

void setup() {
	delay(1000);

    /* start GPIO */
	eio.start();

	/* Initialize SPIFFS */
	if(!SPIFFS.begin()){
		elog.Write(LogLevel::Info,"setup","An Error has occurred while mounting SPIFFS");
		while(1);
	}

	Serial.begin(921600);
	Serial.println("\r\n");
	elog.start(LogLevel::Trace);
	
	elog.Write(LogLevel::Info,"setup", "SmartFMV1");
	
	/* start wifi module */
	ewifi.start();

	esptimer_start();

	app.start();

	app.test_fm();
	
}

void loop() {
	esptimer_time_check();
}

