/*******************************************************************************
* File Name: ESPGPIO.h
*
* Version: 1.0
*
* Description:
* This is header file for GPIO. All the functions related to
* GPIO are here.
*
* Owner:
* Yogesh M Iggalore
*
* Code Tested With:
* 1. platformio and esp32
*
********************************************************************************
* Copyright (2021-22) ,
********************************************************************************/
#ifndef ESPGPIO_h
#define ESPGPIO_h
#include <Arduino.h>

#define LED_MAX_PINS    2

#define LED_PIN_WIFI	2
#define LED_PIN_MQTT    5

//led index
#define LED_WIFI   0
#define LED_MQTT   1

//led modes
#define LED_MODE_OFF        0
#define LED_MODE_ON         1
#define LED_MODE_BLINK      2
#define LED_MODE_FAST_BLINK 3

// pin state
#define PIN_LOW         0
#define PIN_HIGH        1

#define LED_BLINK_TIME          500
#define LED_FAST_BLINK_TIME     100

//Variables
struct ledControl{
    uint8_t ui8Mode;
    uint8_t ui8PinState;
    uint16_t ui16DownCounter;
};

class ESPGPIO{
    public:
        struct ledControl tdfled[LED_MAX_PINS];
        ESPGPIO();
		void start(void);
        void led_set_mode(uint8_t led, uint8_t mode);
        void led_timer(void);
        void led_apply(void);
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_ESPGPIO)
extern ESPGPIO eio;
#endif
#endif