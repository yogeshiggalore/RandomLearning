/*******************************************************************************
*File Name: MqttFile.cpp
*
* Version: 1.0
*
* Description:
* In this source code for managing mqtt files
*
*
* Owner:
* eNtroL
*
********************************************************************************
* Copyright (2020-21) , eNtroL
*******************************************************************************/
#include <Arduino.h>
#include <FS.h>
#include <ESPUtils.h>
#include <ESPFile.h>
#include <MqttFile.h>
#include <ESPUtils.h>

// Constructors ////////////////////////////////////////////////////////////////
MqttFile::MqttFile(){}

void MqttFile::Read_Mqtt_Parameter(uint8_t ui8ID){
    String sString;

    sString = EFile.Read_Data_From_File(sESPFiles[ui8ID]);
    EUtils.println(sString);

    if(ui8ID == MQTT_PUB_VOLTAGE_TOPIC_INDEX){
        sMqttPubVoltageTopic = sString;
    }else if(ui8ID == MQTT_PUB_CURRENT_TOPIC_INDEX){
        sMqttPubCurrentTopic = sString;
    }else if(ui8ID == MQTT_PUB_POWER_TOPIC_INDEX){
        sMqttPubPowerTopic = sString;
    }else if(ui8ID == MQTT_PUB_UNIT_TOPIC_INDEX){
        sMqttPubUnitTopic = sString;
    }else if(ui8ID == MQTT_PUB_FREQUENCY_TOPIC_INDEX){
        sMqttPubFrequencyTopic = sString;
    }else if(ui8ID == MQTT_PUB_PF_TOPIC_INDEX){
        sMqttPubpowerfactorTopic = sString;
    }else if(ui8ID == MQTT_PUB_RELAY_TOPIC_INDEX){
        sMqttPubRelayTopic = sString;
    }else if(ui8ID == MQTT_SUB_RELAY_TOPIC_INDEX){
        sMqttSubRelayTopic = sString;
    }else if(ui8ID == MQTT_SUB_GETALL_TOPIC_INDEX){
        sMqttPubAllTopic = sString;
    }else if(ui8ID == MQTT_SERVER_INDEX){
        sMqttServer = sString;
    }else if(ui8ID == MQTT_PORT_INDEX){
        ui16MqttPort = sString.toInt();
    }else if(ui8ID == MQTT_USERNAME_INDEX){
        sMqttUsername = sString;
    }else if(ui8ID == MQTT_PASSWORD_INDEX){
        sMqttPassword = sString;
    }else if(ui8ID == MQTT_CLIENTID_INDEX){
        sMqttClientId = sString;
    }else if(ui8ID == MQTT_QOS_INDEX){
        ui8MqttQoS = sString.toInt();
    }else if(ui8ID == MQTT_KEEPALIVE_INDEX){
        ui16MqttKeepalive = sString.toInt();
    }else{
        EUtils.println("file error");
    }
}

void MqttFile::Read_All_Mqtt_Parameters(void){
    String sDeviceName;

    sDeviceName = EUtils.Get_Device_Name() + "/";

    EUtils.println("Updating mqtt parameters");

    sMqttDeviceStatus = sDeviceName + "Status";

    sMqttPubVoltageTopic = EFile.Read_Data_From_File(sESPFiles[MQTT_PUB_VOLTAGE_TOPIC_INDEX]);
    sMqttPubVoltageTopic  = sDeviceName + sMqttPubVoltageTopic + "/Get";
    EUtils.print("sMqttPubVoltageTopic:");
    EUtils.println(sMqttPubVoltageTopic);
    

    sMqttPubCurrentTopic = EFile.Read_Data_From_File(sESPFiles[MQTT_PUB_CURRENT_TOPIC_INDEX]);
    sMqttPubCurrentTopic = sDeviceName + sMqttPubCurrentTopic + "/Get";
    EUtils.print("sMqttPubCurrentTopic:");    
    EUtils.println(sMqttPubCurrentTopic);


    sMqttPubPowerTopic = EFile.Read_Data_From_File(sESPFiles[MQTT_PUB_POWER_TOPIC_INDEX]);
    sMqttPubPowerTopic = sDeviceName + sMqttPubPowerTopic+ "/Get";
    EUtils.print("sMqttPubPowerTopic:"); 
    EUtils.println(sMqttPubPowerTopic);


    sMqttPubUnitTopic = EFile.Read_Data_From_File(sESPFiles[MQTT_PUB_UNIT_TOPIC_INDEX]);
    sMqttPubUnitTopic = sDeviceName + sMqttPubUnitTopic+ "/Get";
    EUtils.print("sMqttPubUnitTopic:"); 
    EUtils.println(sMqttPubUnitTopic);

    sMqttPubFrequencyTopic = EFile.Read_Data_From_File(sESPFiles[MQTT_PUB_FREQUENCY_TOPIC_INDEX]);
    sMqttPubFrequencyTopic  = sDeviceName + sMqttPubFrequencyTopic + "/Get";
    EUtils.print("sMqttPubFrequencyTopic:");
    EUtils.println(sMqttPubFrequencyTopic);
    
    sMqttPubpowerfactorTopic = EFile.Read_Data_From_File(sESPFiles[MQTT_PUB_PF_TOPIC_INDEX]);
    sMqttPubpowerfactorTopic = sDeviceName + sMqttPubpowerfactorTopic + "/Get";
    EUtils.print("sMqttPubpowerfactorTopic:");    
    EUtils.println(sMqttPubpowerfactorTopic);

    sMqttPubRelayTopic = EFile.Read_Data_From_File(sESPFiles[MQTT_PUB_RELAY_TOPIC_INDEX]);
    sMqttPubRelayTopic = sDeviceName + sMqttPubRelayTopic + "/Get";
    EUtils.print("sMqttPubRelayTopic:");    
    EUtils.println(sMqttPubRelayTopic);

    sMqttSubRelayTopic = EFile.Read_Data_From_File(sESPFiles[MQTT_SUB_RELAY_TOPIC_INDEX]);
    sMqttSubRelayTopic = sDeviceName + sMqttSubRelayTopic + "/Set";
    EUtils.print("sMqttSubRelayTopic:");    
    EUtils.println(sMqttSubRelayTopic);

    sMqttPubAllTopic = EFile.Read_Data_From_File(sESPFiles[MQTT_SUB_GETALL_TOPIC_INDEX]);
    sMqttPubAllTopic = sDeviceName + sMqttPubAllTopic + "/GetAll";
    EUtils.print("sMqttPubAllTopic:"); 
    EUtils.println(sMqttPubAllTopic);

    sMqttServer = EFile.Read_Data_From_File(sESPFiles[MQTT_SERVER_INDEX]);
    EUtils.print("MqttServer:"); 
    EUtils.println(sMqttServer);

    ui16MqttPort = EFile.Read_Data_From_File(sESPFiles[MQTT_PORT_INDEX]).toInt();
    EUtils.print("MqttPort:"); 
    EUtils.println(ui16MqttPort);

    sMqttUsername = EFile.Read_Data_From_File(sESPFiles[MQTT_USERNAME_INDEX]);
    EUtils.print("MqttUsername:"); 
    EUtils.println(sMqttUsername);

    sMqttPassword = EFile.Read_Data_From_File(sESPFiles[MQTT_PASSWORD_INDEX]);
    EUtils.print("MqttPassword:"); 
    EUtils.println(sMqttPassword);

    sMqttClientId = EFile.Read_Data_From_File(sESPFiles[MQTT_CLIENTID_INDEX]);
    EUtils.print("MqttClient:"); 
    if(strcmp("noclientid",sMqttClientId.c_str()) == 0){
        sMqttClientId = EUtils.Get_Device_Name();
    }
    EUtils.println(sMqttClientId);

    ui8MqttQoS = EFile.Read_Data_From_File(sESPFiles[MQTT_QOS_INDEX]).toInt();
    EUtils.print("MqttQoS:"); 
    EUtils.println(ui8MqttQoS);

    ui16MqttKeepalive = EFile.Read_Data_From_File(sESPFiles[MQTT_KEEPALIVE_INDEX]).toInt();
    EUtils.print("MqttKeepalive:"); 
    EUtils.println(ui16MqttKeepalive);

}

String MqttFile::Get_Value_Of_Mqtt_File(uint8_t ui8Index){
    String sString;

    if(ui8Index == MQTT_PUB_VOLTAGE_TOPIC_INDEX){
        sString= sMqttPubVoltageTopic;
    }else if(ui8Index == MQTT_PUB_CURRENT_TOPIC_INDEX){
        sString= sMqttPubCurrentTopic;
    }else if(ui8Index == MQTT_PUB_POWER_TOPIC_INDEX){
        sString= sMqttPubPowerTopic;
    }else if(ui8Index == MQTT_PUB_UNIT_TOPIC_INDEX){
        sString= sMqttPubUnitTopic;
    }else if(ui8Index == MQTT_PUB_FREQUENCY_TOPIC_INDEX){
        sString= sMqttPubFrequencyTopic;
    }else if(ui8Index == MQTT_PUB_PF_TOPIC_INDEX){
        sString= sMqttPubpowerfactorTopic;
    }else if(ui8Index == MQTT_PUB_RELAY_TOPIC_INDEX){
        sString= sMqttPubRelayTopic;
    }else if(ui8Index == MQTT_SUB_RELAY_TOPIC_INDEX){
        sString= sMqttSubRelayTopic;
    }else if(ui8Index == MQTT_SUB_GETALL_TOPIC_INDEX){
        sString= sMqttPubAllTopic;
    }else if(ui8Index == MQTT_SERVER_INDEX){
        sString= sMqttServer;
    }else if(ui8Index == MQTT_PORT_INDEX){
        sString= String(ui16MqttPort);
    }else if(ui8Index == MQTT_USERNAME_INDEX){
        sString= sMqttUsername;
    }else if(ui8Index == MQTT_PASSWORD_INDEX){
        sString= sMqttPassword;
    }else if(ui8Index == MQTT_CLIENTID_INDEX){
        sString= sMqttClientId;
    }else if(ui8Index == MQTT_QOS_INDEX){
        sString= String(ui8MqttQoS);
    }else if(ui8Index == MQTT_KEEPALIVE_INDEX){
        sString= String(ui16MqttKeepalive);
    }else{
        sString= "  ";
    }

    return sString;
}

// Preinstantiate Objects //////////////////////////////////////////////////////
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
MqttFile MqFile;
#endif