/*******************************************************************************
*File Name: METIFile.cpp
*
* Version: 1.0
*
* Description:
* In this source code for managing files in SPIFFS
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/
#include <Arduino.h>
#include <Application.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <METIUtils.h>
#include <METIMemory.h>
#include <METIFile.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h> 
#include <METIAlexaV2.h>
#include <ACControl.h>

// Constructors ////////////////////////////////////////////////////////////////
METIApplication::METIApplication(){}

void METIApplication::HandleAlexa(uint8_t * payload){
    DynamicJsonDocument json(1024);
    deserializeJson(json, payload,DeserializationOption::NestingLimit(20));
    uint8_t ui8DeviceIndex=0;

    String sDeviceId = json ["deviceId"];
    String sAction = json ["action"];
    String sValue = json["value"];
    int    i16jsonValue=0;
    int ai16ResponseData[3];

    ai16ResponseData[0] = 0x7FFF;
    ai16ResponseData[1] = 0x7FFF;
    ai16ResponseData[2] = 1;

    if(sAction == "SetPercentage"){
        i16jsonValue = json["value"]["percentage"];
    }else if(sAction == "AdjustPercentage"){
        i16jsonValue = json["value"]["percentageDelta"];
    }else if(sAction == "action.devices.commands.OnOff"){
        String sString1 = json["value"]["on"];
        MUtils.println(sString1);
        if(sString1 == "true"){
            i16jsonValue = 1;
            MUtils.println("OK");
        }else{
            i16jsonValue = 0;
            MUtils.println("Error");
        }
    }else if(sAction == "action.devices.commands.BrightnessAbsolute"){
        String sString1 = json["value"]["brightness"];
        MUtils.println(sString1);
        i16jsonValue = json["value"]["brightness"];
        MUtils.println("brigthness");
        MUtils.println(i16jsonValue);
    }else{
        i16jsonValue =  0;
    }

    MUtils.println(sDeviceId);
    MUtils.println(sAction);
    MUtils.println(sValue);
    MUtils.println(i16jsonValue);

    //serializeJsonPretty(json,sString);
    ui8DeviceIndex = Check_For_Device_ID(sDeviceId);

    if(ui8DeviceIndex != 0xFF){
        Check_For_Action(sAction,sValue,i16jsonValue,ai16ResponseData);
        MUtils.print("ai16ResponseData[0]:");
        MUtils.println(ai16ResponseData[0]);
        MUtils.print("ai16ResponseData[1]:");
        MUtils.println(ai16ResponseData[1]);
        Update_AC_Input_Values(ui8DeviceIndex, ai16ResponseData);
    }else{
        MUtils.print("No config id:");
        MUtils.println(sDeviceId);
    }
}

uint8_t METIApplication::Check_For_Device_ID(String sDeviceID){
    uint8_t ui8Index=0xFF;
 
    if(sDeviceID.equals(MAlexa.sAlexaACInput1ID)){
        //MUtils.print("Device is correct:");
        //MUtils.println(MAlexa.sAlexaACInput1ID);
        ui8Index =  AC_INPUT_1_INDEX;
    }else if(sDeviceID.equals(MAlexa.sAlexaACInput2ID)){
        //MUtils.print("Device is correct:");
        //MUtils.println(MAlexa.sAlexaACInput2ID);
        ui8Index = AC_INPUT_2_INDEX;
    }else if(sDeviceID.equals(MAlexa.sAlexaACInput3ID)){
        //MUtils.print("Device is correct:");
        //MUtils.println(MAlexa.sAlexaACInput3ID);
        ui8Index = AC_INPUT_3_INDEX;
    }else if(sDeviceID.equals(MAlexa.sAlexaACInput4ID)){
        //MUtils.print("Device is correct:");
        //MUtils.println(MAlexa.sAlexaACInput4ID);
        ui8Index = AC_INPUT_4_INDEX;
    }else{
        ui8Index=0xFF;
    }

    return ui8Index;
}

void METIApplication::Check_For_Action(String sAction, String sValue,int i16jsonValue, int* ai16ResponseData){
    int i16Value=0;

    if(sAction.equals("setPowerState")){
        
        if(sValue.equals(DEVICE_ON)){
            ai16ResponseData[0] = 100;
        }else{
            ai16ResponseData[0] = 0;
        }
    }else if(sAction.equals("SetBrightness")){
        i16Value = sValue.toInt();
        if(i16Value > 100){
            i16Value = 100;
        }
        if(i16Value < 0){
            i16Value = 0;
        }
        ai16ResponseData[0] = i16Value;
    }else if(sAction.equals("SetPercentage")){
        ai16ResponseData[0] = i16jsonValue;
    }else if(sAction.equals("AdjustBrightness")){
        ai16ResponseData[1] = sValue.toInt();
    }else if(sAction.equals("AdjustPercentage")){
        ai16ResponseData[1] = i16jsonValue;
    }else if(sAction == "action.devices.commands.OnOff"){
        if(i16jsonValue){
            ai16ResponseData[0] = 100;
        }else{
            ai16ResponseData[0] = 0;
        }
    }else if(sAction == "action.devices.commands.BrightnessAbsolute"){
        if(i16jsonValue > 100){
            i16jsonValue = 100;
        }
        if(i16jsonValue < 0){
            i16jsonValue = 0;
        }
        ai16ResponseData[0] = i16jsonValue;
    }else{
        // do nothing
        ai16ResponseData[2] = 0;
    }
}

void METIApplication::Update_AC_Input_Values(uint8_t ui8DeviceIndex, int* ai16ResponseData){
    uint8_t ui8GetCurrentValue=0;
    if(ui8DeviceIndex < 4){
        if(ai16ResponseData[2]){
            if(ai16ResponseData[1] == 0x7FFF){
                if(ai16ResponseData[0] == 0x7FFF){
                    ai16ResponseData[0] = 0;
                }
            }else{
                ui8GetCurrentValue = AC.Get_ACControlValue(ui8DeviceIndex);
                ai16ResponseData[0] = ui8GetCurrentValue + ai16ResponseData[1];
            }

            if((ai16ResponseData[0] >= 0) and (ai16ResponseData[0] <= 100)){
                AC.Set_ACControlValue(ui8DeviceIndex,ai16ResponseData[0]);
                MUtils.println("OK");
                MUtils.println(AC.Get_ACControlValue(ui8DeviceIndex));
            }else if(ai16ResponseData[0] > 100){
                AC.Set_ACControlValue(ui8DeviceIndex,100);
                MUtils.println(">100");
                MUtils.println(AC.Get_ACControlValue(ui8DeviceIndex));
            }else{
                AC.Set_ACControlValue(ui8DeviceIndex,0);
                MUtils.println("<0");
                MUtils.println(AC.Get_ACControlValue(ui8DeviceIndex));
            }


        }
    }
}

// Preinstantiate Objects //////////////////////////////////////////////////////
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
METIApplication MApp;
#endif