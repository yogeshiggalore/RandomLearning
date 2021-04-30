/*******************************************************************************
* File Name: ModbusMaster.h
*
* Version: 1.0
*
* Description:
* This is header file for energy meter. All the functions related to
* energy meter are implemented in this file
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
#ifndef EnergyMeter_h
#define EnergyMeter_h

#include <Arduino.h>

class EnergyMeter{
public:
    EnergyMeter();
    float fVoltage=0;
    float fCurrent=0;
    float fPower=0;
    float fUnit=0;
    float fFrequency=0;
    float fPowerfactor=0;
    uint8_t ui8EnergyFlag=0;
    void Process(void);
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
extern EnergyMeter EMeter;
#endif
#endif