/*******************************************************************************
* File Name: MqttFile.h
*
* Version: 1.0
*
* Description:
* This is header file for ESP MQTT. All the functions related to
* modbus are here.
*
* Owner:
* Yogesh M Iggalore
*
* Code Tested With:
* 1. platformio and esp32
*
********************************************************************************
* Copyright (2020-21) , Lattech
********************************************************************************/
#ifndef MqttFile_h
#define MqttFile_h
#include <Arduino.h>

typedef struct{
    String sWill;
    String sPubJson;
    String sSubJson;
    String sPubTime;
    String sSubTime;
}MQTT_TOPIC;

typedef struct{
    bool bStatus;
    bool bReadStatus;
    bool bWriteStatus;
    bool bStartInitiated;
    String sServer;
    uint16_t ui16Port;
    uint16_t ui16KeepAlive;
    uint8_t ui8QoS;
    uint8_t ui8Retain;
    String sClientId;
    String sUserName;
    String sPassword;
    String sTopicPassword;
    MQTT_TOPIC tdfTopic;
}MQTT_PARAMETER;


class MqttFile{
    public:
        MqttFile();
        MQTT_PARAMETER tdfMqttPar; 
        void Start(void);
        void Write_Parameter_To_File(void);
        void Read_Parameter_From_File(void);
        void Print_Mqtt_Parameters(void);
        void Refresh_All_Mqtt_Parameters(void);
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_MQTTFILE)
extern MqttFile FMqtt;
#endif
#endif