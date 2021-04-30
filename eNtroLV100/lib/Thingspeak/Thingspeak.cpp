/*******************************************************************************
* File Name: Thingspeak.cpp
*
* Version: 1.0
*
* Description:
* This is source file for thingspeak server. All the functions related to
* thingspeak server are implemented in this file
*
* Owner:
* Yogesh M Iggalore
*
* Code Tested With:
* 1. platformio and esp8266
*
********************************************************************************
* Copyright (2020-21) , eNtroL
********************************************************************************/

#include <Arduino.h>
#include <Thingspeak.h>
#include <ESPUtils.h>
#include <WiFiClient.h>
#include <EnergyMeter.h>
#include <Application.h>
#include <ESPFile.h>

// Constructors ////////////////////////////////////////////////////////////////
Thingspeak::Thingspeak(void):tsclient(){}

void Thingspeak::Start(void){
    Read_All_Thingspeak_Parameters();
}

uint8_t Thingspeak::Connect_To_Server(uint16_t ui16Timeout){
    if(tsclient.status()){
        tsclient.stop();
        delay(10);
    }

    tsclient.setTimeout(ui16Timeout);
    if(!tsclient.connect(cThingspeakhost,ui8ThingspeakPort)){
        return 0;
    }else{
        return 1;
    }
}

uint8_t Thingspeak::Set_FieldValue(uint8_t ui8Field, String sString){
    if(ui8Field < 1){
        return 0;
    }else if(ui8Field > 8){
        return 0;
    }else{
        if(ui8Field == 1){
            sField1 = sString;
            ui8SendFieldFlag = ui8SendFieldFlag | 0x01;
        }else if(ui8Field == 2){
            sField2 = sString;
            ui8SendFieldFlag = ui8SendFieldFlag | 0x02;
        }else if(ui8Field == 3){
            sField3 = sString;
            ui8SendFieldFlag = ui8SendFieldFlag | 0x04;
        }else if(ui8Field == 4){
            sField4 = sString;
            ui8SendFieldFlag = ui8SendFieldFlag | 0x08;
        }else if(ui8Field == 5){
            sField5 = sString;
            ui8SendFieldFlag = ui8SendFieldFlag | 0x10;
        }else if(ui8Field == 6){
            sField6 = sString;
            ui8SendFieldFlag = ui8SendFieldFlag | 0x20;
        }else if(ui8Field == 7){
            sField7 = sString;
            ui8SendFieldFlag = ui8SendFieldFlag | 0x40;
        }else{
            sField8 = sString;
            ui8SendFieldFlag = ui8SendFieldFlag | 0x80;
        }
    }
    return 1;
}

uint8_t Thingspeak::Send_Data_To_Server(uint16_t ui16Timeout){
    String sData;
    uint8_t ui8ProccedFlag=0;
    uint16_t ui16TimeCounter=0;
    
    Serial1.print("Flag:");
    Serial1.println(ui8SendFieldFlag);

    sData = "GET /update?api_key=" + sThingspeakChAPIKey;
    if(ui8SendFieldFlag & 0x01){
        sData = sData +"&field1=" + sField1;
        ui8ProccedFlag = 1;
    }
    
    if(ui8SendFieldFlag & 0x02){
        sData = sData +"&field2=" + sField2;
        ui8ProccedFlag = 1;
    }
    
    if(ui8SendFieldFlag & 0x04){
        sData = sData +"&field3=" + sField3;
        ui8ProccedFlag = 1;
    }
    
    if(ui8SendFieldFlag & 0x04){
        sData = sData +"&field4=" + sField4;
        ui8ProccedFlag = 1;
    }
    
    if(ui8SendFieldFlag & 0x10){
        sData = sData +"&field5=" + sField5;
        ui8ProccedFlag = 1;
    }
    
    if(ui8SendFieldFlag & 0x20){
        sData = sData +"&field6=" + sField6;
        ui8ProccedFlag = 1;
    }
    
    if(ui8SendFieldFlag & 0x40){
        sData = sData +"&field7=" + sField7;
        ui8ProccedFlag = 1;
    }
    
    if(ui8SendFieldFlag & 0x80){
        sData = sData +"&field8=" + sField8;
        ui8ProccedFlag = 1;
    }

    ui8SendFieldFlag=0;
    sData = sData + " HTTP/1.1\r\n" + "Host: " + "api.thingspeak.com" + "\r\n" + "Connection: close\r\n\r\n";
    if(ui8ProccedFlag){
        if(tsclient.status()){
            tsclient.print(sData);
            Serial1.print(sData);
            Serial1.println("Thingspeak sent data sucess");
            while((!tsclient.available()) && (ui16TimeCounter < ui16Timeout)){
                delay(1);
                ui16TimeCounter++;
            }
            if(ui16TimeCounter < ui16Timeout){
                while(tsclient.available()){
                    Serial1.println(tsclient.readString());       
                }
            }else{
                Serial1.println("Request timeout..");
            }
            tsclient.stop();
            return 1;
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}

void Thingspeak::Update_Energy(void){
    
    if(ui8ThingspeakEnable){
        Set_FieldValue(1,String(EMeter.fVoltage));
        Set_FieldValue(2,String(EMeter.fCurrent));
        Set_FieldValue(3,String(EMeter.fPower));
        Set_FieldValue(4,String(EMeter.fUnit));
        Set_FieldValue(5,String(EMeter.fFrequency));
        Set_FieldValue(6,String(EMeter.fPowerfactor));
        Set_FieldValue(7,String(EApp.ui8RelayStatus));

        if(TS.Connect_To_Server(500)){
            TS.Send_Data_To_Server(1000);
        }else{
            Serial1.println("thingspeak Error ");
        }
    }
}


void Thingspeak::Read_Thingspeak_Parameter(uint8_t ui8ID){
    String sString;

    sString = EFile.Read_Data_From_File(sESPFiles[ui8ID]);
    EUtils.println(sString);

    if(ui8ID == TS_CHANNEL_API){
        sThingspeakChAPIKey = sString;
    }

    if(ui8ID == TS_ENABLE_FLAG){
        ui8ThingspeakEnable = sString.toInt();
    }

}   

void Thingspeak::Read_All_Thingspeak_Parameters(void){
    
    sThingspeakChAPIKey = EFile.Read_Data_From_File(sESPFiles[TS_CHANNEL_API]);
    EUtils.print("Thingspeak channel API:");
    EUtils.println(sThingspeakChAPIKey);

    ui8ThingspeakEnable = EFile.Read_Data_From_File(sESPFiles[TS_ENABLE_FLAG]).toInt();
    EUtils.print("Thingspeak enable flag:");
    EUtils.println(ui8ThingspeakEnable);

}

// Preinstantiate Objects //////////////////////////////////////////////////////
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
Thingspeak TS;
#endif