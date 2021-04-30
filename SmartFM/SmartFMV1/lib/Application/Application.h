/*******************************************************************************
* File Name: Application .h
*
* Version: 1.0
*
* Description:
* This is header file for smart fm application. All the functions related to
* application are here.
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
#ifndef APPLICATION_h
#define APPLICATION_h
#include <Arduino.h>
#include <SI470X.h>

#define RESET_PIN       33
#define ESP32_I2C_SDA   21

class Application{
    public:    
        Application();
		void start(void);
        void test_fm(void);
    private:
        SI470X fm;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_APPLICATION)
extern Application app;
#endif
#endif