/*******************************************************************************
*File Name: ACControl.cpp
*
* Version: 1.0
*
* Description:
* In this source code for AC control
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ACControl.h>
#include <METIMemory.h>
#include <METIUtils.h>
#include <WebSocketsClient.h> 
#include <METIAlexaV2.h>
#include <METIFile.h>

// Constructors ////////////////////////////////////////////////////////////////
ACControl::ACControl(){}


void ACControl::Start(void){
    uint8_t ui8LoopCounter=0;

    Serial1.println("Read AC control packets ");
    MMemory.Read_ACControl(aui8ACPackets);

    ui8100usCounter=0;
    ui81msFlag = 0;
    ui8TimeCounter = 0;

    for(ui8LoopCounter=0;ui8LoopCounter<AC_MAX_CHANNEL;ui8LoopCounter++){
        aui8ACControlCounter[ui8LoopCounter] = 100;
        aui8ACTriacStatus[ui8LoopCounter] = 0;
        aui8ACControlType[ui8LoopCounter] = aui8ACPackets[ui8LoopCounter];
        aui8ACControlValue[ui8LoopCounter] = aui8ACPackets[ui8LoopCounter + 4];
    }

    //pinMode(ZERO_CROSS_PIN,INPUT);
    pinMode(AC_CONTROL_PIN_1,OUTPUT);
    pinMode(AC_CONTROL_PIN_2,OUTPUT);
    pinMode(AC_CONTROL_PIN_3,OUTPUT);
    pinMode(AC_CONTROL_PIN_4,OUTPUT);

    PinControl(AC_INPUT_1_INDEX,0);
    PinControl(AC_INPUT_2_INDEX,0);
    PinControl(AC_INPUT_3_INDEX,0);
    PinControl(AC_INPUT_4_INDEX,0);

    Update_All_Mqtt_Parameters();
}

void ACControl::ZeroCrossDetect(void){
    ui8100usCounter=0;
    ui81msFlag = 0;
    ui8TimeCounter = 0;
    
    /*lTimeEnd = micros();
    Serial1.print(lTimeEnd - lTimeStart);
    Serial1.print("  ");
    lTimeStart = micros();
    
    Serial1.print(aui8ACControlCounter[0]);
    Serial1.print("  ");
    Serial1.print(aui8ACControlCounter[1]);
    Serial1.print("  ");
    Serial1.print(aui8ACControlCounter[2]);
    Serial1.print("  ");
    Serial1.print(aui8ACControlCounter[3]);
    Serial1.println(" ");*/

    aui8ACControlCounter[0] = 100;
    aui8ACControlCounter[1] = 100;
    aui8ACControlCounter[2] = 100;
    aui8ACControlCounter[3] = 100;

    aui8ACTriacFlag[0] = 0;
    aui8ACTriacFlag[1] = 0;
    aui8ACTriacFlag[2] = 0;
    aui8ACTriacFlag[3] = 0;

    /*PinControl(AC_INPUT_1_INDEX,0);
    PinControl(AC_INPUT_2_INDEX,0);
    PinControl(AC_INPUT_3_INDEX,0);
    PinControl(AC_INPUT_4_INDEX,0);*/
    
    ui8ZeroCrossDetected = 1;
    //Serial1.println("Z");
    /*Serial1.println(aui8ACControlValue[0]);
    Serial1.println(aui8ACControlValue[1]);
    Serial1.println(aui8ACControlValue[2]);
    Serial1.println(aui8ACControlValue[3]);*/
    //Check_All_Device();   
}

void ACControl::Check_Timer_10ms(void){
    ui8100usCounter++;

    Check_All_Device();

    if(ui8100usCounter > 9){
        ui81msFlag = 1;
        ui8100usCounter = 0;
    }

    if(ui81msFlag){
        ui81msFlag = 0;
        
        //Check_Triac_State();

        if(ui8TimeCounter < (AC_MAX_FREQ_TIME_10 + 2)){
            ui8TimeCounter++;
        }

        //Check_All_Device();

        if(ui8TimeCounter > AC_MAX_FREQ_TIME_10){
            ui8TimeCounter = 0;
            aui8ACControlCounter[0] = 100;
            aui8ACControlCounter[1] = 100;
            aui8ACControlCounter[2] = 100;
            aui8ACControlCounter[3] = 100;
            //Serial1.println("T");
        }else{
            aui8ACControlCounter[0] = aui8ACControlCounter[0] - 10;
            aui8ACControlCounter[1] = aui8ACControlCounter[1] - 10;
            aui8ACControlCounter[2] = aui8ACControlCounter[2] - 10;
            aui8ACControlCounter[3] = aui8ACControlCounter[3] - 10;
            /*Serial1.println(aui8ACControlCounter[0]);
            Serial1.println(aui8ACControlCounter[1]);
            Serial1.println(aui8ACControlCounter[2]);
            Serial1.println(aui8ACControlCounter[3]);*/
        }
    }
}

void ACControl::Check_Timer_20ms(void){
    ui8100usCounter++;

    Check_All_Device();

    if(ui8100usCounter > 9){
        ui81msFlag = 1;
        ui8100usCounter = 0;
    }

    if(ui81msFlag){
        ui81msFlag = 0;
        
        //Check_Triac_State();

        if(ui8TimeCounter < (AC_MAX_FREQ_TIME_20 + 2)){
            ui8TimeCounter++;
        }

        //Check_All_Device();

        if(ui8TimeCounter == 10){
            aui8ACControlCounter[0] = 100;
            aui8ACControlCounter[1] = 100;
            aui8ACControlCounter[2] = 100;
            aui8ACControlCounter[3] = 100;

            aui8ACTriacFlag[0] = 0;
            aui8ACTriacFlag[1] = 0;
            aui8ACTriacFlag[2] = 0;
            aui8ACTriacFlag[3] = 0;
        }

        if(ui8TimeCounter > AC_MAX_FREQ_TIME_20){
            ui8TimeCounter = 0;
            aui8ACControlCounter[0] = 100;
            aui8ACControlCounter[1] = 100;
            aui8ACControlCounter[2] = 100;
            aui8ACControlCounter[3] = 100;
            aui8ACTriacFlag[0] = 0;
            aui8ACTriacFlag[1] = 0;
            aui8ACTriacFlag[2] = 0;
            aui8ACTriacFlag[3] = 0;
            //Serial1.println("T");
        }else{
            aui8ACControlCounter[0] = aui8ACControlCounter[0] - 10;
            aui8ACControlCounter[1] = aui8ACControlCounter[1] - 10;
            aui8ACControlCounter[2] = aui8ACControlCounter[2] - 10;
            aui8ACControlCounter[3] = aui8ACControlCounter[3] - 10;
            /*Serial1.println(aui8ACControlCounter[0]);
            Serial1.println(aui8ACControlCounter[1]);
            Serial1.println(aui8ACControlCounter[2]);
            Serial1.println(aui8ACControlCounter[3]);*/
        }
    }
}

void ACControl::Check_All_Device(void){
    Check_Device(0);
    Check_Device(1);
    Check_Device(2);
    Check_Device(3);
}

void ACControl::Check_Device(uint8_t ui8DeviceId){

    if(aui8ACControlType[ui8DeviceId] == AC_TYPE_BULB){
        if(aui8ACControlValue[ui8DeviceId]){
            PinControl(ui8DeviceId,HIGH);
        }else{
            PinControl(ui8DeviceId,LOW);
        }
    }else if(aui8ACControlType[ui8DeviceId] == AC_TYPE_FAN){
        if(aui8ACControlValue[ui8DeviceId] >= aui8ACControlCounter[ui8DeviceId]){
            PinControl(ui8DeviceId,HIGH);
        }else{
            PinControl(ui8DeviceId,LOW);
        }
    }else if(aui8ACControlType[ui8DeviceId] == AC_TYPE_DIMMER){
        if(aui8ACControlValue[ui8DeviceId] >= aui8ACControlCounter[ui8DeviceId]){
            PinControl(ui8DeviceId,HIGH);
        }else{
            PinControl(ui8DeviceId,LOW);
        }
    }else{
        PinControl(ui8DeviceId,LOW);
    }
}

void ACControl::PinControl(uint8_t ui8DeviceId, uint8_t ui8Value){
    if(ui8DeviceId == 0){
        if(ui8Value){
            if(aui8ACTriacFlag[ui8DeviceId] == 0){
                aui8ACTriacFlag[ui8DeviceId] = 1;
                digitalWrite(AC_CONTROL_PIN_1,HIGH);
                aui8ACTriacStatus[ui8DeviceId] = HIGH;
            }
        }else{
            digitalWrite(AC_CONTROL_PIN_1,LOW);
            aui8ACTriacStatus[ui8DeviceId] = LOW;
        }
    }

    if(ui8DeviceId == 1){
        if(ui8Value){
            if(aui8ACTriacFlag[ui8DeviceId] == 0){
                aui8ACTriacFlag[ui8DeviceId] = 1;
                digitalWrite(AC_CONTROL_PIN_2,HIGH);
                aui8ACTriacStatus[ui8DeviceId] = HIGH;
            }
        }else{
            digitalWrite(AC_CONTROL_PIN_2,LOW);
            aui8ACTriacStatus[ui8DeviceId] = LOW;
        }
    }

    if(ui8DeviceId == 2){
        if(ui8Value){
            if(aui8ACTriacFlag[ui8DeviceId] == 0){
                aui8ACTriacFlag[ui8DeviceId] = 1;
                digitalWrite(AC_CONTROL_PIN_3,HIGH);
                aui8ACTriacStatus[ui8DeviceId] = HIGH;
            }
        }else{
            digitalWrite(AC_CONTROL_PIN_3,LOW);
            aui8ACTriacStatus[ui8DeviceId] = LOW;
        }
    }

    if(ui8DeviceId == 3){
        if(ui8Value){
            if(aui8ACTriacFlag[ui8DeviceId] == 0){
                aui8ACTriacFlag[ui8DeviceId] = 1;
                digitalWrite(AC_CONTROL_PIN_4,HIGH);
                aui8ACTriacStatus[ui8DeviceId] = HIGH;
            }
        }else{
            digitalWrite(AC_CONTROL_PIN_4,LOW);
            aui8ACTriacStatus[ui8DeviceId] = LOW;
        }
    }
}

void ACControl::Check_Triac_State(void){
    uint8_t ui8LoopCounter=0;

    for(ui8LoopCounter=0;ui8LoopCounter<AC_MAX_CHANNEL;ui8LoopCounter++){
        if(aui8ACTriacStatus[ui8LoopCounter] == HIGH){
            PinControl(ui8LoopCounter,LOW);
        }
    }
}

uint8_t ACControl::Get_ACControlValue(uint8_t ui8DeviceID){
    return aui8ACControlValue[ui8DeviceID];
}

uint8_t ACControl::Set_ACControlValue(uint8_t ui8DeviceID, uint8_t ui8Value){
    uint8_t ui8LoopCounter=0;

    if(aui8ACControlValue[ui8DeviceID] == ui8Value){
        return 1;
    }else{
        
        aui8ACControlValue[ui8DeviceID] = ui8Value;
        aui8ACPackets[ui8DeviceID + 4] = ui8Value;
    
        MMemory.Write_ACControl(aui8ACPackets);
        MMemory.Store();
        MMemory.Restore();

        MMemory.Read_ACControl(aui8ACPackets);
        for(ui8LoopCounter=0;ui8LoopCounter<AC_MAX_CHANNEL;ui8LoopCounter++){
            aui8ACControlType[ui8LoopCounter] = aui8ACPackets[ui8LoopCounter];
            aui8ACControlValue[ui8LoopCounter] = aui8ACPackets[ui8LoopCounter + 4];
        }

        if(aui8ACControlValue[ui8DeviceID] == ui8Value){
            return 1;
        }else{
            return 0;
        }
    }
}

uint8_t ACControl::Get_ACControlType(uint8_t ui8DeviceID){
    return aui8ACControlType[ui8DeviceID];
}

uint8_t ACControl::Set_ACControlType(uint8_t ui8DeviceID, uint8_t ui8Value){
    uint8_t ui8LoopCounter;

    if(aui8ACControlType[ui8DeviceID] == ui8Value){
        return 1;
    }else{
        aui8ACControlType[ui8DeviceID] = ui8Value;
        aui8ACPackets[ui8DeviceID] = ui8Value;

        MMemory.Write_ACControl(aui8ACPackets);
        MMemory.Store();
        MMemory.Restore();

        MMemory.Read_ACControl(aui8ACPackets);
        for(ui8LoopCounter=0;ui8LoopCounter<AC_MAX_CHANNEL;ui8LoopCounter++){
            aui8ACControlType[ui8LoopCounter] = aui8ACPackets[ui8LoopCounter];
            aui8ACControlValue[ui8LoopCounter] = aui8ACPackets[ui8LoopCounter + 4];
        }
        
        if(aui8ACControlType[ui8DeviceID] == ui8Value){
            return 1;
        }else{
            return 0;
        }
    }
}

String ACControl::Get_ACControlType_In_String(uint8_t ui8DeviceID){
    String sString;

    if(aui8ACControlType[ui8DeviceID] == AC_TYPE_BULB){
        sString = "BULB";
    }else if(aui8ACControlType[ui8DeviceID] == AC_TYPE_FAN){
        sString = "FAN";
    }else if(aui8ACControlType[ui8DeviceID] == AC_TYPE_DIMMER){
        sString = "DIMMER";
    }else{
        sString = "None";
    }

    return sString;
}

String ACControl::Get_AC_Image(uint8_t ui8DeviceID){
    uint8_t ui8DeviceType=0;
    uint8_t ui8DeviceValue=0;
    uint8_t ui8ImageIndex=0;
    String sString;

    ui8DeviceType = Get_ACControlType(ui8DeviceID);
    ui8DeviceValue = Get_ACControlValue(ui8DeviceID);

    ui8ImageIndex = ui8DeviceValue / 10;
    if(ui8ImageIndex > 10){
        ui8ImageIndex = 10;
    }

    if(ui8DeviceType == AC_TYPE_BULB){
        sString = sbulbdimmer[ui8ImageIndex];
    }else if(ui8DeviceType == AC_TYPE_DIMMER){
        sString = sbulbdimmer[ui8ImageIndex];
    }else if(ui8DeviceType == AC_TYPE_FAN){
        sString = sfan[ui8ImageIndex];
    }else{
        sString = "noimage.png";
    }

    return sString;
}

void ACControl::Test_Memory_ReadWrite(void){
    uint8_t aui8Data[16];
    uint8_t ui8LoopCounter=0;

    MUtils.println("Testing AC control data read");
    MMemory.Read_ACControl(aui8Data);

    MUtils.println("Data: ");
    for(ui8LoopCounter=0;ui8LoopCounter<16;ui8LoopCounter++){
        MUtils.print(aui8Data[ui8LoopCounter]);
        MUtils.print(" ");
    }
    MUtils.println();

    MUtils.println("Testing AC control data write");
    for(ui8LoopCounter=0;ui8LoopCounter<16;ui8LoopCounter++){
        aui8Data[ui8LoopCounter] =  aui8Data[ui8LoopCounter] + 10;
    }

    MMemory.Write_ACControl(aui8Data);

    MUtils.println("Testing AC control data read");
    MMemory.Read_ACControl(aui8Data);

    MUtils.println("Data: ");
    for(ui8LoopCounter=0;ui8LoopCounter<16;ui8LoopCounter++){
        MUtils.print(aui8Data[ui8LoopCounter]);
        MUtils.print(" ");
    }
    MUtils.println();

}

void ACControl::Test_AC_ControlValue(uint8_t ui8DeviceId, uint8_t ui8Value){
    aui8ACControlValue[ui8DeviceId] = ui8Value;
}

void ACControl::Test_AC_ControlType(uint8_t ui8DeviceId, uint8_t ui8Value){
    aui8ACControlType[ui8DeviceId] = ui8Value;
}


void ACControl::Test_Get_Set_ACControlValue(uint8_t ui8DeviceId, uint8_t ui8Value){
    MUtils.println("Testing Test_Get_Set_ACControlValue");
    MUtils.print("Read: ");
    MUtils.println(Get_ACControlValue(ui8DeviceId));
    MUtils.print("Write: ");
    MUtils.println(ui8Value);
    if(Set_ACControlValue(ui8DeviceId,ui8Value)){
        MUtils.println("Write sucess");
    }else{
        MUtils.println("Write fail");
    }
}

void ACControl::Test_Get_Set_ACControlType(uint8_t ui8DeviceId, uint8_t ui8Value){
    MUtils.println("Testing Test_Get_Set_ACControlType");
    MUtils.print("Read: ");
    MUtils.println(Get_ACControlType(ui8DeviceId));
    MUtils.print("Write: ");
    MUtils.println(ui8Value);
    if(Set_ACControlType(ui8DeviceId,ui8Value)){
        MUtils.println("Write sucess");
    }else{
        MUtils.println("Write fail");
    }
}

String ACControl::Get_Mqtt_AC_String(uint8_t ui8DeviceID){
    String sData;
    StaticJsonDocument<300> doc;

    if(ui8DeviceID == AC_INPUT_1_INDEX){
        doc["acname"] = MAlexa.sAlexaACInput1Name;
        doc["actype"] = Get_ACControlType_In_String(AC_INPUT_1_INDEX);
        doc["acvalue"] = Get_ACControlValue(AC_INPUT_1_INDEX);
    }
    
    if(ui8DeviceID == AC_INPUT_2_INDEX){
        doc["acname"] = MAlexa.sAlexaACInput2Name;
        doc["actype"] = Get_ACControlType_In_String(AC_INPUT_2_INDEX);
        doc["acvalue"] = Get_ACControlValue(AC_INPUT_2_INDEX);
    }

    if(ui8DeviceID == AC_INPUT_3_INDEX){
        doc["acname"] = MAlexa.sAlexaACInput3Name;
        doc["actype"] = Get_ACControlType_In_String(AC_INPUT_3_INDEX);
        doc["acvalue"] = Get_ACControlValue(AC_INPUT_3_INDEX);
    }

    if(ui8DeviceID == AC_INPUT_4_INDEX){
        doc["acname"] = MAlexa.sAlexaACInput4Name;
        doc["actype"] = Get_ACControlType_In_String(AC_INPUT_4_INDEX);
        doc["acvalue"] = Get_ACControlValue(AC_INPUT_4_INDEX);
    }

    serializeJson(doc, sData);

    return sData;
}

void ACControl::Update_Mqtt_Parameter(uint8_t ui8ID){
    String sString;

    sString = MFile.Read_Data_From_File(sMETIFiles[ui8ID]);
    MUtils.println(sString);

    if(ui8ID == MQTT_PUB_AC_1_TOPIC_INDEX){
        sMqttPubAC1Topic = sString;
    }else if(ui8ID == MQTT_PUB_AC_2_TOPIC_INDEX){
        sMqttPubAC2Topic = sString;
    }else if(ui8ID == MQTT_PUB_AC_3_TOPIC_INDEX){
        sMqttPubAC3Topic = sString;
    }else if(ui8ID == MQTT_PUB_AC_4_TOPIC_INDEX){
        sMqttPubAC4Topic = sString;
    }else if(ui8ID == MQTT_PUB_AC_1_TOPIC_INDEX){
        sMqttSubAC1Topic = sString;
    }else if(ui8ID == MQTT_PUB_AC_2_TOPIC_INDEX){
        sMqttSubAC2Topic = sString;
    }else if(ui8ID == MQTT_PUB_AC_3_TOPIC_INDEX){
        sMqttSubAC3Topic = sString;
    }else if(ui8ID == MQTT_PUB_AC_4_TOPIC_INDEX){
        sMqttSubAC4Topic = sString;
    }else if(ui8ID == MQTT_SUB_AC_TOPIC_INDEX){
        sMqttSubACAllTopic = sString;
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
        MUtils.println("file error");
    }
}

void ACControl::Update_All_Mqtt_Parameters(void){
    String sDeviceName;

    sDeviceName = MUtils.Get_Device_Name() + "/";

    MUtils.println("Updating mqtt parameters");

    sMqttDeviceStatus = sDeviceName + "Status";

    sMqttPubAC1Topic = MFile.Read_Data_From_File(sMETIFiles[MQTT_PUB_AC_1_TOPIC_INDEX]);
    sMqttPubAC1Topic  = sDeviceName + sMqttPubAC1Topic + "/Get";
    MUtils.print("MqttPubAC1Topic:");
    MUtils.println(sMqttPubAC1Topic);
    

    sMqttPubAC2Topic = MFile.Read_Data_From_File(sMETIFiles[MQTT_PUB_AC_2_TOPIC_INDEX]);
    sMqttPubAC2Topic = sDeviceName + sMqttPubAC2Topic + "/Get";
    MUtils.print("MqttPubAC2Topic:");    
    MUtils.println(sMqttPubAC2Topic);


    sMqttPubAC3Topic = MFile.Read_Data_From_File(sMETIFiles[MQTT_PUB_AC_3_TOPIC_INDEX]);
    sMqttPubAC3Topic = sDeviceName + sMqttPubAC3Topic+ "/Get";
    MUtils.print("MqttPubAC3Topic:"); 
    MUtils.println(sMqttPubAC3Topic);


    sMqttPubAC4Topic = MFile.Read_Data_From_File(sMETIFiles[MQTT_PUB_AC_4_TOPIC_INDEX]);
    sMqttPubAC4Topic = sDeviceName + sMqttPubAC4Topic+ "/Get";
    MUtils.print("MqttPubAC4Topic:"); 
    MUtils.println(sMqttPubAC4Topic);

    sMqttSubAC1Topic = MFile.Read_Data_From_File(sMETIFiles[MQTT_PUB_AC_1_TOPIC_INDEX]);
    sMqttSubAC1Topic  = sDeviceName + sMqttSubAC1Topic + "/Set";
    MUtils.print("MqttSubAC1Topic:");
    MUtils.println(sMqttSubAC1Topic);
    
    sMqttSubAC2Topic = MFile.Read_Data_From_File(sMETIFiles[MQTT_PUB_AC_2_TOPIC_INDEX]);
    sMqttSubAC2Topic = sDeviceName + sMqttSubAC2Topic + "/Set";
    MUtils.print("MqttSubAC2Topic:");    
    MUtils.println(sMqttSubAC2Topic);

    sMqttSubAC3Topic = MFile.Read_Data_From_File(sMETIFiles[MQTT_PUB_AC_3_TOPIC_INDEX]);
    sMqttSubAC3Topic = sDeviceName + sMqttSubAC3Topic+ "/Set";
    MUtils.print("MqttSubAC3Topic:"); 
    MUtils.println(sMqttSubAC3Topic);

    sMqttSubAC4Topic = MFile.Read_Data_From_File(sMETIFiles[MQTT_PUB_AC_4_TOPIC_INDEX]);
    sMqttSubAC4Topic = sDeviceName + sMqttSubAC4Topic+ "/Set";
    MUtils.print("MqttSubAC4Topic:"); 
    MUtils.println(sMqttSubAC4Topic);

    sMqttSubACAllTopic = MFile.Read_Data_From_File(sMETIFiles[MQTT_SUB_AC_TOPIC_INDEX]);
    sMqttSubACAllTopic = sDeviceName + sMqttSubACAllTopic + "/GetAll";
    MUtils.print("MqttSubACAllTopic:"); 
    MUtils.println(sMqttSubACAllTopic);

    sMqttServer = MFile.Read_Data_From_File(sMETIFiles[MQTT_SERVER_INDEX]);
    MUtils.print("MqttServer:"); 
    MUtils.println(sMqttServer);

    ui16MqttPort = MFile.Read_Data_From_File(sMETIFiles[MQTT_PORT_INDEX]).toInt();
    MUtils.print("MqttPort:"); 
    MUtils.println(ui16MqttPort);

    sMqttUsername = MFile.Read_Data_From_File(sMETIFiles[MQTT_USERNAME_INDEX]);
    MUtils.print("MqttUsername:"); 
    MUtils.println(sMqttUsername);

    sMqttPassword = MFile.Read_Data_From_File(sMETIFiles[MQTT_PASSWORD_INDEX]);
    MUtils.print("MqttPassword:"); 
    MUtils.println(sMqttPassword);

    sMqttClientId = MFile.Read_Data_From_File(sMETIFiles[MQTT_CLIENTID_INDEX]);
    MUtils.print("MqttClient:"); 
    MUtils.println(sMqttClientId);
    if(strcmp("noclientid",sMqttClientId.c_str()) == 0){
        sMqttClientId = sDeviceName;
    }

    ui8MqttQoS = MFile.Read_Data_From_File(sMETIFiles[MQTT_QOS_INDEX]).toInt();
    MUtils.print("MqttQoS:"); 
    MUtils.println(ui8MqttQoS);

    ui16MqttKeepalive = MFile.Read_Data_From_File(sMETIFiles[MQTT_KEEPALIVE_INDEX]).toInt();
    MUtils.print("MqttKeepalive:"); 
    MUtils.println(ui16MqttKeepalive);

}

String ACControl::Get_Value_Of_Mqtt_File(uint8_t ui8Index){
    String sString;

    if(ui8Index == MQTT_PUB_AC_1_TOPIC_INDEX){
        sString= sMqttPubAC1Topic;
    }else if(ui8Index == MQTT_PUB_AC_2_TOPIC_INDEX){
        sString= sMqttPubAC2Topic;
    }else if(ui8Index == MQTT_PUB_AC_3_TOPIC_INDEX){
        sString= sMqttPubAC3Topic;
    }else if(ui8Index == MQTT_PUB_AC_4_TOPIC_INDEX){
        sString= sMqttPubAC4Topic;
    }else if(ui8Index == MQTT_PUB_AC_1_TOPIC_INDEX){
        sString= sMqttSubAC1Topic;
    }else if(ui8Index == MQTT_PUB_AC_2_TOPIC_INDEX){
        sString= sMqttSubAC2Topic;
    }else if(ui8Index == MQTT_PUB_AC_3_TOPIC_INDEX){
        sString= sMqttSubAC3Topic;
    }else if(ui8Index == MQTT_PUB_AC_4_TOPIC_INDEX){
        sString= sMqttSubAC4Topic;
    }else if(ui8Index == MQTT_SUB_AC_TOPIC_INDEX){
        sString= sMqttSubACAllTopic;
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
ACControl AC;
#endif