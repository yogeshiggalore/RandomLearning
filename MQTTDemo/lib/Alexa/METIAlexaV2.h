/*******************************************************************************
*File Name: METIAlexa.h
*
* Version: 1.0
*
* Description:
* In this header file for Alexa functions
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/
#ifndef METIAlexa_h
#define METIAlexa_h
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h> 
#include <ArduinoJson.h> 
#include <StreamString.h>

/* yogesh */
//#define MyApiKey "6cbd92b8-caf8-45f7-b749-0297e971fc1e" // TODO: Change to your sinric API Key. Your API Key is displayed on sinric.com dashboard

/* nisarga */
//#define MyApiKey "3b9d225b-51cf-4d70-b5da-80832eaba7ea"

/*shariff */
//#define MyApiKey "ca66a0f6-00db-4b59-95f9-ada66b10f4de"

#define SWITCH_ID    "5ce4de16e545ec083fee0945"
#define LIGHT_ID     "5ce4de28e545ec083fee0947"
#define THERMO_ID    "5ce4de3ee545ec083fee0949"
#define SHUTTER_ID   "5ce4de50e545ec083fee094b"
#define TV_ID        "5ce4de5ee545ec083fee094d"
#define SPEAKER_ID   "5ce4de6fe545ec083fee094f"
#define DOORLOCK_ID  "5ce4de83e545ec083fee0951"
#define PROJECTOR_ID "5ce4de96e545ec083fee0953"
#define FAN_ID       "5ce4e763e545ec083fee095d"

/* Yogesh*/
//#define HALL_LIGHT_ID       "5cf25998bfe01b2db3b7197d"
//#define ROOM_LIGHT_ID       "5cf259a7bfe01b2db3b7197f"
//#define KITCHEN_LIGHT_ID    "5cf259cdbfe01b2db3b71981"

/* Shariff */
//#define HALL_LIGHT_ID       "5d136276e32b137cab0dca41"
//define ROOM_LIGHT_ID       "5d136294e32b137cab0dca43"
//#define KITCHEN_LIGHT_ID    "5d1362a2e32b137cab0dca45"

//#define PRODUCTION_LIGHT_ID     "5d136276e32b137cab0dca41"
//#define HARDWARE_LIGHT_ID       "5d136294e32b137cab0dca43"
//#define FAN_ID                  "5d1362a2e32b137cab0dca45"

/* Nisarga */
#define HALL_LIGHT_ID       "5d42cd3256393964a3c9cbe1"
#define ROOM_LIGHT_ID       "5d42cd1b56393964a3c9cbdf"
#define KITCHEN_LIGHT_ID    "5d42cd3e56393964a3c9cbe3"

#define HEARTBEAT_INTERVAL 300000

#define DEVICE_ON   "ON"
#define DEVICE_OFF   "OFF"

#define HALL_LIGHT_FLAG     0x1
#define ROOM_LIGHT_FLAG     0x2
#define KITCHEN_LIGHT_FLAG  0x4

#define ALEXA_BRIGHTNESS_LEVEL  25
class METIAlexa{
public:
    String sAlexaAPIKey;
    String sAlexaACInput1ID;
    String sAlexaACInput2ID;
    String sAlexaACInput3ID;
    String sAlexaACInput4ID;
    String sAlexaACInput1Name;
    String sAlexaACInput2Name;
    String sAlexaACInput3Name;
    String sAlexaACInput4Name;
    
    METIAlexa(void);
    //void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);
    void Start(void);
    void handleRequest(void);
    void Update_All_Key_And_ID(void);
    void Update_Specified_File(uint8_t ui8ID);
    String Get_Value_Of_File(uint8_t ui8Index);
private:
    WebSocketsClient webSocket;
    WiFiClient client;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
extern METIAlexa MAlexa;
#endif
#endif
