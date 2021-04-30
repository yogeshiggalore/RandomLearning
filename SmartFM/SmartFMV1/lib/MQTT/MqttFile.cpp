/*******************************************************************************
*File Name: MqttFile.cpp
*
* Version: 1.0
*
* Description:
* In this source file for mqtt application
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2020-21) Lattech
*******************************************************************************/

#include<Arduino.h>
#include<MqttFile.h>
#include<ArduinoJson.h>
#include<FS.h>
#include<SPIFFS.h>
#include<ESPUtils.h>
#include <LogLevel.h>
#include <logger.h>

// Constructors ////////////////////////////////////////////////////////////////
MqttFile::MqttFile(){}

void MqttFile::Start(void){
    elog.Write(LogLevel::Info,"MQTT", "Reading mqtt parameters ");
    Read_Parameter_From_File();
    Refresh_All_Mqtt_Parameters();
}  

void MqttFile::Read_Parameter_From_File(void){
    const char* word;

    StaticJsonDocument<512> doc;
    
    File fFile = SPIFFS.open("/mqttconfig.json","r");
    if (!fFile) {
        elog.Write(LogLevel::Error,"MQTT", "Failed to open config file");
        tdfMqttPar.bReadStatus = 0;
    }else{
        tdfMqttPar.bReadStatus = 1;
        deserializeJson(doc, fFile);
        word = doc["server"];
        tdfMqttPar.sServer = word;
        word = doc["port"];
        tdfMqttPar.ui16Port = doc["port"];
        word = doc["keepalive"];
        tdfMqttPar.ui16KeepAlive = doc["keepalive"];
        word = doc["client"];
        tdfMqttPar.sClientId = EUtils.Get_Device_Name();
        word = doc["username"];
        tdfMqttPar.sUserName = word;
        word = doc["password"];
        tdfMqttPar.sPassword = word;
        word = doc["topicpass"];
        tdfMqttPar.sTopicPassword = word;
        word = doc["qos"];
        tdfMqttPar.ui8QoS = doc["qos"];
        word = doc["retain"];
        tdfMqttPar.ui8Retain = doc["retain"];
    }
}

void MqttFile::Write_Parameter_To_File(void){
    DynamicJsonDocument doc(512) ;
    String sOutputString ;

    doc["server"]    = tdfMqttPar.sServer;
    doc["port"]      = tdfMqttPar.ui16Port;
    doc["keepalive"] = tdfMqttPar.ui16KeepAlive;
    doc["client"]    = tdfMqttPar.sClientId;
    doc["username"]  = tdfMqttPar.sUserName;
    doc["password"]  = tdfMqttPar.sPassword;
    doc["topicpass"] = tdfMqttPar.sTopicPassword;
    doc["qos"]       = tdfMqttPar.ui8QoS;
    
    serializeJsonPretty(doc, sOutputString);
    //Serial.println(sOutputString);

    File fFile = SPIFFS.open("/mqttconfig.json", "w");
    if (!fFile) {
        elog.Write(LogLevel::Error,"MQTT", "Error opening file for writing");
        tdfMqttPar.bWriteStatus = 0;
        return;
    }
    
    int bytesWritten = fFile.print(sOutputString);
    if (bytesWritten > 0) {
        elog.Write(LogLevel::Error,"MQTT", "file write sucess");
        tdfMqttPar.bWriteStatus = 1;
    }else {
        elog.Write(LogLevel::Error,"MQTT", "Error file writing");
        tdfMqttPar.bWriteStatus = 0;
    }
    fFile.close();

}

void MqttFile::Print_Mqtt_Parameters(void){
    Serial.println(tdfMqttPar.sServer);
    Serial.println(tdfMqttPar.sClientId);
    Serial.println(tdfMqttPar.sUserName);
    Serial.println(tdfMqttPar.sPassword);
    Serial.println(tdfMqttPar.bStatus);
    Serial.println(tdfMqttPar.bReadStatus);
    Serial.println(tdfMqttPar.ui16Port);
    Serial.println(tdfMqttPar.ui16KeepAlive);
    Serial.println(tdfMqttPar.sTopicPassword);
    Serial.println(tdfMqttPar.ui8QoS);
    Serial.println(tdfMqttPar.ui8Retain);
    Serial.println(tdfMqttPar.tdfTopic.sWill);
    Serial.println(tdfMqttPar.tdfTopic.sPubJson);
    Serial.println(tdfMqttPar.tdfTopic.sSubJson);
    Serial.println(tdfMqttPar.tdfTopic.sPubTime);
    Serial.println(tdfMqttPar.tdfTopic.sSubTime);
}

void MqttFile::Refresh_All_Mqtt_Parameters(void){
    Read_Parameter_From_File();

    if(tdfMqttPar.sClientId != "nomqtt"){
        tdfMqttPar.sClientId = EUtils.Get_Device_Name();
    }

    tdfMqttPar.tdfTopic.sWill = EUtils.Get_Device_Name() + "/" + tdfMqttPar.sTopicPassword + "/status";

    tdfMqttPar.tdfTopic.sPubJson = EUtils.Get_Device_Name() + "/" + tdfMqttPar.sTopicPassword + "/get";

    tdfMqttPar.tdfTopic.sSubJson = EUtils.Get_Device_Name() + "/" + tdfMqttPar.sTopicPassword + "/set";

    tdfMqttPar.tdfTopic.sPubTime = EUtils.Get_Device_Name() + "/" + tdfMqttPar.sTopicPassword + "/time/get";

    tdfMqttPar.tdfTopic.sSubTime = EUtils.Get_Device_Name() + "/" + tdfMqttPar.sTopicPassword + "/time/set";
}

// Preinstantiate Objects //////////////////////////////////////////////////////
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_MQTTFILE)
MqttFile FMqtt;
#endif
