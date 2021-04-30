/*******************************************************************************
* File Name: Thingspeak.h
*
* Version: 1.0
*
* Description:
* This is header file for thingspeak server. All the functions related to
* thingspeak server are implemented in this file
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
#ifndef Thingspeak_h
#define Thingspeak_h

#include <Arduino.h>
#include <WiFiClient.h>

class Thingspeak{
public:
    Thingspeak();
    const char *cThingspeakhost = "api.thingspeak.com";
    uint8_t ui8ThingspeakPort=80;
    String sThingspeakChAPIKey;

    uint8_t ui8ThingspeakEnable=0;

    uint8_t ui8SendFieldFlag=0;
    String sField1;
    String sField2;
    String sField3;
    String sField4;
    String sField5;
    String sField6;
    String sField7;
    String sField8;

    void Start(void);
    uint8_t Send_Data_To_Server(uint16_t ui16Timeout);
    uint8_t Connect_To_Server(uint16_t ui16Timeout);
    uint8_t Set_FieldValue(uint8_t ui8Field, String sString);
    void    Update_Energy(void);
    void Read_Thingspeak_Parameter(uint8_t ui8ID);
    void Read_All_Thingspeak_Parameters(void);

private:
    WiFiClient tsclient;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
extern Thingspeak TS;
#endif
#endif