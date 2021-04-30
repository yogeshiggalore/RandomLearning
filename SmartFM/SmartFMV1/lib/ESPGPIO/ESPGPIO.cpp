/*******************************************************************************
*File Name: ESPGPIO.cpp
*
* Version: 1.0
*
* Description:
* In this source file for GPIO application
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2021-22)
*******************************************************************************/

#include<Arduino.h>
#include<ESPGPIO.h>

// Constructors ////////////////////////////////////////////////////////////////
ESPGPIO::ESPGPIO(){}

void ESPGPIO::start(void){
    pinMode(LED_PIN_WIFI,OUTPUT);
    pinMode(LED_PIN_MQTT,OUTPUT);

    led_set_mode(LED_WIFI, LED_MODE_OFF);
    led_set_mode(LED_MQTT, LED_MODE_OFF);
}

void ESPGPIO::led_set_mode(uint8_t ui8Led, uint8_t ui8Mode){
	tdfled[ui8Led].ui8Mode = ui8Mode;
}

void ESPGPIO::led_timer(void){
	for (uint8_t i = 0; i < LED_MAX_PINS; i++){
		if (tdfled[i].ui8Mode == LED_MODE_ON){
			tdfled[i].ui8PinState = PIN_HIGH;
		}else if (tdfled[i].ui8Mode == LED_MODE_OFF){
			tdfled[i].ui8PinState = PIN_LOW;
		}else if (tdfled[i].ui8Mode == LED_MODE_BLINK){
			if (tdfled[i].ui16DownCounter-- <= 0){
				tdfled[i].ui16DownCounter = LED_BLINK_TIME;
				if (tdfled[i].ui8PinState == PIN_HIGH){
					tdfled[i].ui8PinState = PIN_LOW;
				}else{
					tdfled[i].ui8PinState = PIN_HIGH;
				}
			}
		}else if (tdfled[i].ui8Mode == LED_MODE_FAST_BLINK){
			if (tdfled[i].ui16DownCounter-- <= 0){
				tdfled[i].ui16DownCounter = LED_FAST_BLINK_TIME;
				if (tdfled[i].ui8PinState == PIN_HIGH){
					tdfled[i].ui8PinState = PIN_LOW;
				}else{
					tdfled[i].ui8PinState = PIN_HIGH;
				}
			}
		}
	}

	led_apply();
}

void ESPGPIO::led_apply(void){
	digitalWrite(LED_PIN_WIFI, tdfled[LED_WIFI].ui8PinState);
	digitalWrite(LED_PIN_MQTT, tdfled[LED_MQTT].ui8PinState);
}


// Preinstantiate Objects //////////////////////////////////////////////////////
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_ESPGPIO)
ESPGPIO eio;
#endif