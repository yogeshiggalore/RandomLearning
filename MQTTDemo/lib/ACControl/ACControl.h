/*******************************************************************************
*File Name: ACControl.h
*
* Version: 1.0
*
* Description:
* In this header file for AC control
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/

#ifndef ACControl_h
#define ACControl_h
#include <Arduino.h>
#include <ESP8266WiFi.h>

#define AC_MAX_CHANNEL  4

#define ZERO_CROSS_PIN      5
#define AC_CONTROL_PIN_1    4
#define AC_CONTROL_PIN_2    14
#define AC_CONTROL_PIN_3    12
#define AC_CONTROL_PIN_4    13

#define AC_INPUT_1_INDEX    0
#define AC_INPUT_2_INDEX    1
#define AC_INPUT_3_INDEX    2
#define AC_INPUT_4_INDEX    3

#define AC_TYPE_NONE    0
#define AC_TYPE_BULB    1
#define AC_TYPE_FAN     2
#define AC_TYPE_DIMMER  3

#define AC_MAX_FREQ_TIME_10    10
#define AC_MAX_FREQ_TIME_20    20

static const char *sbulbdimmer[] = {
    "bulboff.png",
    "bulb_10_30.png",
    "bulb_10_30.png",
    "bulb_30_45.png",
    "bulb_30_45.png",
    "bulb_45_60.png",
    "bulb_45_60.png",
    "bulb_60_75.png",
    "bulb_60_75.png",
    "bulb_75_100.png",
    "bulbon.png"
};

static const char *sfan[] = {
    "fan_0_10.gif",
    "fan_10_15.gif",
    "fan_10_15.gif",
    "fan_15_30.gif",
    "fan_15_30.gif",
    "fan_30_45.gif",
    "fan_30_45.gif",
    "fan_45_60.gif",
    "fan_45_60.gif",
    "fan_75_90.gif",
    "fan_75_90.gif"
};

class ACControl{
public:
    uint8_t ui8TimeCounter=0;
    uint8_t ui8PhaseShiftFlag=0;
    uint8_t aui8ACPackets[16];
    uint8_t aui8ACControlType[AC_MAX_CHANNEL];
    uint8_t aui8ACControlValue[AC_MAX_CHANNEL];
    uint8_t aui8ACControlCounter[AC_MAX_CHANNEL];
    uint8_t aui8ACTriacStatus[AC_MAX_CHANNEL];
    uint8_t aui8ACTriacFlag[AC_MAX_CHANNEL];
    uint8_t ui8100usCounter=0;
    uint8_t ui81msFlag=0;
    uint8_t ui8ZeroCrossDetected=0;
    uint8_t ui8SkipTraicCheckFlag = 0;
    String sMqttPubAC1Topic;
    String sMqttPubAC2Topic;
    String sMqttPubAC3Topic;
    String sMqttPubAC4Topic;
    
    String sMqttSubAC1Topic;
    String sMqttSubAC2Topic;
    String sMqttSubAC3Topic;
    String sMqttSubAC4Topic;
    String sMqttSubACAllTopic;
    String sMqttDeviceStatus;

    String sMqttServer;
    uint16_t ui16MqttPort;
    String sMqttUsername;
    String sMqttPassword;
    String sMqttClientId;

    uint8_t ui8MqttQoS;
    uint16_t ui16MqttKeepalive;
        
    long lTimeEnd=0;
    long lTimeStart=0;
    ACControl();
    void Start(void);
    void ICACHE_RAM_ATTR ZeroCrossDetect(void);
    void ICACHE_RAM_ATTR Check_Timer_20ms(void);
    void ICACHE_RAM_ATTR Check_Timer_10ms(void);
    void ICACHE_RAM_ATTR Check_All_Device(void);
    void ICACHE_RAM_ATTR Check_Device(uint8_t ui8DeviceId);
    void ICACHE_RAM_ATTR PinControl(uint8_t ui8DeviceId, uint8_t ui8Value);
    void ICACHE_RAM_ATTR Check_Triac_State(void);
    uint8_t Get_ACControlValue(uint8_t ui8DeviceID);
    uint8_t Set_ACControlValue(uint8_t ui8DeviceID, uint8_t ui8Value);
    uint8_t Get_ACControlType(uint8_t ui8DeviceID);
    uint8_t Set_ACControlType(uint8_t ui8DeviceID, uint8_t ui8Value);
    String Get_ACControlType_In_String(uint8_t ui8DeviceID);
    String Get_AC_Image(uint8_t ui8DeviceID);

    String Get_Mqtt_AC_String(uint8_t ui8DeviceID);
    void Update_Mqtt_Parameter(uint8_t ui8ID);
    void Update_All_Mqtt_Parameters(void);
    String Get_Value_Of_Mqtt_File(uint8_t ui8Index);

    void Test_Memory_ReadWrite(void);
    void Test_AC_ControlValue(uint8_t ui8DeviceId, uint8_t ui8Value);
    void Test_AC_ControlType(uint8_t ui8DeviceId, uint8_t ui8Value);
    void Test_Get_Set_ACControlValue(uint8_t ui8DeviceId, uint8_t ui8Value);
    void Test_Get_Set_ACControlType(uint8_t ui8DeviceId, uint8_t ui8Value);
    
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
extern ACControl AC;
#endif
#endif