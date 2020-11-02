/*******************************************************************************
*File Name: METITimer.h
*
* Version: 1.0
*
* Description:
* In this header file handle 1ms timer
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/

#ifndef METITIMER_h
#define METITIMER_h
#include <Arduino.h>
#include <ESP8266WiFi.h>

class METITimer{
public:
    METITimer();
    void Start(void);
};
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
extern METITimer MTimer;
#endif
#endif