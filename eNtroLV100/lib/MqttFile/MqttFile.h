/*******************************************************************************
*File Name: MQTTFile.h
*
* Version: 1.0
*
* Description:
* In this header used for managing mqtt files in spiffs system
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2020-21) , eNtroL
*******************************************************************************/
#ifndef MQTTFILE_h
#define MQTTFILE_h
#include <Arduino.h>

class MqttFile{
public:
    MqttFile();

    String sMqttPubVoltageTopic;
    String sMqttPubCurrentTopic;
    String sMqttPubPowerTopic;
    String sMqttPubUnitTopic;
    String sMqttPubFrequencyTopic;
    String sMqttPubpowerfactorTopic;
    String sMqttPubAllTopic;
    String sMqttPubRelayTopic;
    String sMqttSubRelayTopic;

    String sMqttDeviceStatus;

    String sMqttServer;
    uint16_t ui16MqttPort;
    String sMqttUsername;
    String sMqttPassword;
    String sMqttClientId;

    uint8_t  ui8MqttQoS;
    uint16_t ui16MqttKeepalive;

    void Read_Mqtt_Parameter(uint8_t ui8ID);
    void Read_All_Mqtt_Parameters(void);
    String Get_Value_Of_Mqtt_File(uint8_t ui8Index);
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
extern MqttFile MqFile;
#endif
#endif