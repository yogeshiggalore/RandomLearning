/*******************************************************************************
* File Name: Application.h
*
* Version: 1.0
*
* Description:
* This is header file for application. All the functions related to
* application are implemented in this file
*
* Owner:
* Yogesh M Iggalore
*
* Code Tested With:
* 1. platformio and esp8266
*
********************************************************************************
* Copyright (2020-21) , eNtroL
********************************************************************************/
#ifndef Application_h
#define Application_h

#include <Arduino.h>

#define GREEN_LED       1
#define YELLOW_LED      2
#define RELAY           3

#define PIN_GREEN_LED      D6 
#define PIN_YELLOW_LED     D7
#define PIN_ENTROL_RELAY   D1   

class Application{
public:
    Application();
    uint8_t ui8GreenLedStatus=0;
    uint8_t ui8YellowLedStatus=0;
    uint8_t ui8MqttStatus=0;
    uint8_t ui8AlexaStatus=0;
    uint8_t ui8RelayStatus=0;

    uint8_t ui8AlexaUpdated=0;
    void Start(void);
    void Application_Pin_Write(uint8_t ui8Pin, uint8_t ui8State);
    uint8_t Application_Pin_Read(uint8_t ui8Pin);
    void HandleAlexa(uint8_t * payload);
    uint8_t Check_For_Device_ID(String sDeviceID);
    void Check_For_Action(String sAction, String sValue, int i16jsonValue, int* ai16ResponseData);
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
extern Application EApp;
#endif
#endif