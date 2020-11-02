/*******************************************************************************
*File Name: METIHttpClient.c
*
* Version: 1.0
*
* Description:
* In this source file  for http client functions
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/
#include <METIHttpClient.h>
#include <METIHTTPClientCore.h>
#include "METIUtils.h"
#include <ESP8266WiFi.h>

// Constructors ////////////////////////////////////////////////////////////////
METIHttpClient::METIHttpClient(){}

uint8_t METIHttpClient::Get_Internet_Status(void){
    return ui8MetiServerStatus;
}

void METIHttpClient::Set_Google_Server_Status(uint8_t ui8Status){
    ui8GoogleServerStatus = ui8Status;
}

void METIHttpClient::Set_Google_Server_ErrorCode(int16_t i16ErrorCode){
    i16GoogleServerErrorCode = i16ErrorCode;
}

uint8_t METIHttpClient::Get_Google_Server_Status(void){
    return ui8GoogleServerStatus;
}

int16_t METIHttpClient::Get_Google_Server_ErrorCode(void){
    return i16GoogleServerErrorCode;
}

void METIHttpClient::Set_App_Server_Status(uint8_t ui8Status){
    ui8AppServerStatus = ui8Status;
}

void METIHttpClient::Set_App_Server_ErrorCode(int16_t i16ErrorCode){
    i16AppServerErrorCode = i16ErrorCode;
}

uint8_t METIHttpClient::Get_App_Server_Status(void){
    return ui8AppServerStatus;
}

int16_t METIHttpClient::Get_App_Server_ErrorCode(void){
    return i16AppServerErrorCode;
}

void METIHttpClient::Check_Google_Server(uint16_t ui16Timeout){
    long lTimeStart=0;
    long lTimeEnd=0;
    String sResponse;
    HTTPClient http;
    lTimeStart = millis();
    http.setTimeout(ui16Timeout);
    http.begin(sGoogleURL);
    i16GoogleResponseCode = http.GET();
    sResponse = "Url:Google";
    sResponse = sResponse + " RCode:"+ String(i16GoogleResponseCode);
    if(i16GoogleResponseCode > 0){
        if(i16GoogleResponseCode == 200){
            i16GoogleServerErrorCode = 0;
        }else{
            i16GoogleServerErrorCode = i16GoogleResponseCode;
        }
        ui8GoogleServerStatus = GOOGLE_SERVER_OK;
        //MUtils.println(http.getString());
    }else{
        ui8GoogleServerStatus = GOOGLE_SERVER_ERROR;
        i16GoogleServerErrorCode = i16GoogleResponseCode;
    }
    http.end();


    lTimeEnd = millis();
    sResponse = sResponse + " Time:"+ String(lTimeEnd-lTimeStart);
    MUtils.println(sResponse);
}

void METIHttpClient::Check_METI_Default_Ping(uint16_t ui16Timeout){
    long lTimeStart=0;
    long lTimeEnd=0;
    String sResponse;
    HTTPClient http;
    lTimeStart = millis();
    http.setTimeout(ui16Timeout);
    http.begin(sMETIDefaultURL);
    i16MetiResponseCode = http.GET();
    sResponse = "Url:METID";
    sResponse = sResponse + " RCode:"+ String(i16MetiResponseCode);
    if(i16MetiResponseCode > 0){
        if(i16MetiResponseCode == 200){
            i16MetiServerErrorCode = 0;
            lMetiSucessCounter++;
        }else{
            i16MetiServerErrorCode = i16MetiResponseCode;
            lMetiFailCounter++;
        }
        ui8MetiServerStatus = METI_SERVER_OK;
        //MUtils.println(http.getString());
    }else{
        ui8MetiServerStatus = METI_SERVER_ERROR;
        i16MetiServerErrorCode = i16MetiResponseCode;
        lMetiFailCounter++;
    }
    http.end();
    lTimeEnd = millis();
    sResponse = sResponse + " Time:"+ String(lTimeEnd-lTimeStart)+ + " T:" + String(lMetiSucessCounter+lMetiFailCounter)+" S:" + String(lMetiSucessCounter)+ " F:" + String(lMetiFailCounter);
    MUtils.println(sResponse);
}

// Preinstantiate Objects //////////////////////////////////////////////////////

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
METIHttpClient MHttpClient;
#endif
