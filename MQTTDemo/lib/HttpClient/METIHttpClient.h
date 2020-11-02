/*******************************************************************************
*File Name: METIHttpClient.h
*
* Version: 1.0
*
* Description:
* In this header file  for http client functions
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/
#ifndef METIHttpClient_h
#define METIHttpClient_h
#include <Arduino.h>
#include <ESP8266WiFi.h>

#define GOOGLE_SERVER_NONE  0
#define GOOGLE_SERVER_OK    1
#define GOOGLE_SERVER_ERROR 2

#define APP_SERVER_NONE  0
#define APP_SERVER_OK    1
#define APP_SERVER_ERROR 2

#define METI_SERVER_NONE  0
#define METI_SERVER_OK    1
#define METI_SERVER_ERROR 2

class METIHttpClient{
public:
    uint8_t ui8GoogleServerStatus=0;
    int16_t i16GoogleServerErrorCode=0;
    uint8_t ui8AppServerStatus=0;
    int16_t i16AppServerErrorCode=0;
    uint8_t ui8MetiServerStatus=0;
    int16_t i16MetiServerErrorCode=0;
    int16_t i16MetiResponseCode;
    int16_t i16GoogleResponseCode=0;
    long    lGoogleSucessCounter=0;
    long    lGoogleFailCounter=0;
    long    lMetiSucessCounter=0;
    long    lMetiFailCounter=0;

    String sGoogleURL = "http://www.google.com";
    //String sGoogleURL = "http://yogesh.meti.in/dummy";
    String sMETIDefaultURL = "http://scada.meti.in/services/nodeping.php?pkt=!0701PINGPINGPINGPING02000D070812042B,0000002D";

    METIHttpClient();

    void Set_Google_Server_Status(uint8_t ui8Status);
    void Set_Google_Server_ErrorCode(int16_t i16ErrorCod);
    uint8_t Get_Google_Server_Status(void);
    int16_t Get_Google_Server_ErrorCode(void);

    void Set_App_Server_Status(uint8_t ui8Status);
    void Set_App_Server_ErrorCode(int16_t i16ErrorCode);
    uint8_t Get_App_Server_Status(void);
    int16_t Get_App_Server_ErrorCode(void);

    void Check_Google_Server(uint16_t ui16Timeout);
    void Check_METI_Default_Ping(uint16_t ui16Timeout);
    uint8_t Get_Internet_Status(void);
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
extern METIHttpClient MHttpClient;
#endif
#endif
