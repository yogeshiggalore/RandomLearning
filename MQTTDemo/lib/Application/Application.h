/*******************************************************************************
*File Name: Appl.h
*
* Version: 1.0
*
* Description:
* In this header used for managing files in spiffs system
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/
#ifndef APPLICATION_h
#define APPLICATION_h
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FS.h>

class METIApplication{
public:
    METIApplication();
    void HandleAlexa(uint8_t * payload);
    uint8_t Check_For_Device_ID(String sDeviceID);
    void Check_For_Action(String sAction, String sValue, int i16jsonValue, int* ai16ResponseData);
    void Update_AC_Input_Values(uint8_t ui8DeviceIndex, int* ai16ResponseData);
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
extern METIApplication MApp;
#endif
#endif