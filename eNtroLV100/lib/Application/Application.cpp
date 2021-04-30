/*******************************************************************************
* File Name: Application.cpp
*
* Version: 1.0
*
* Description:
* This is source file for application. All the functions related to
* application are implemented in this file
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
#include <ESPUtils.h>
#include <Application.h>
#include <EEPROM.h>
#include <FS.h>
#include <ESPMemory.h>
#include <ESPFile.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h> 
#include <ESPAlexaV2.h>

// Constructors ////////////////////////////////////////////////////////////////
Application::Application(){}

void Application::Start(void){
    pinMode(PIN_GREEN_LED,OUTPUT);
    pinMode(PIN_YELLOW_LED,OUTPUT);
    pinMode(PIN_ENTROL_RELAY,OUTPUT);

    digitalWrite(PIN_GREEN_LED,ui8GreenLedStatus);
    digitalWrite(PIN_YELLOW_LED,ui8YellowLedStatus);
    digitalWrite(PIN_ENTROL_RELAY,ui8RelayStatus);
}

void Application::Application_Pin_Write(uint8_t ui8Pin, uint8_t ui8State){
    if(ui8Pin == GREEN_LED){
        if(ui8State){
            digitalWrite(PIN_GREEN_LED,HIGH);
            ui8GreenLedStatus = HIGH; 
        }else{
            digitalWrite(PIN_GREEN_LED,LOW);
            ui8GreenLedStatus = LOW; 
        }
    }else if(ui8Pin == YELLOW_LED){
        if(ui8State){
            digitalWrite(PIN_YELLOW_LED,HIGH);
            ui8YellowLedStatus = HIGH; 
        }else{
            digitalWrite(PIN_YELLOW_LED,LOW);
            ui8YellowLedStatus = LOW; 
        }
    }else if(ui8Pin == RELAY){
        if(ui8State){
            digitalWrite(PIN_ENTROL_RELAY,HIGH);
            ui8RelayStatus = HIGH; 
        }else{
            digitalWrite(PIN_ENTROL_RELAY,LOW);
            ui8RelayStatus = LOW; 
        }
        EEPROM.write(MEMORY_RELAY_STATUS_BYTE,ui8RelayStatus);
        EEPROM.commit();
    }else{

    }    
}

uint8_t Application::Application_Pin_Read(uint8_t ui8Pin){
    uint8_t ui8Response=0;

    if(ui8Pin == GREEN_LED){
        ui8Response = ui8GreenLedStatus;
    }else if(ui8Pin == YELLOW_LED){
        ui8Response = ui8YellowLedStatus;
    }else if(ui8Pin == RELAY){
        ui8Response = ui8RelayStatus;
    }else{

    }

    return ui8Response;
}

void Application::HandleAlexa(uint8_t * payload){
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
        EUtils.println(sString1);
        if(sString1 == "true"){
            i16jsonValue = 1;
            EUtils.println("OK");
        }else{
            i16jsonValue = 0;
            EUtils.println("Error");
        }
    }else if(sAction == "action.devices.commands.BrightnessAbsolute"){
        String sString1 = json["value"]["brightness"];
        EUtils.println(sString1);
        i16jsonValue = json["value"]["brightness"];
        EUtils.println("brigthness");
        EUtils.println(i16jsonValue);
    }else{
        i16jsonValue =  0;
    }

    EUtils.print("Sinric data: ");
    EUtils.print(sDeviceId);
    EUtils.print(" ");
    EUtils.print(sAction);
    EUtils.print(" ");
    EUtils.print(sValue);
    EUtils.print(" ");
    EUtils.println(i16jsonValue);

    //serializeJsonPretty(json,sString);
    ui8DeviceIndex = Check_For_Device_ID(sDeviceId);

    if(ui8DeviceIndex != 0xFF){
        Check_For_Action(sAction,sValue,i16jsonValue,ai16ResponseData);
    }else{
        EUtils.print("No config id:");
        EUtils.println(sDeviceId);
    }
}

uint8_t Application::Check_For_Device_ID(String sDeviceID){
    uint8_t ui8Index=0xFF;
 
    if(sDeviceID.equals(MAlexa.sEntrolRelayId)){
        //EUtils.print("Device is correct:");
        //EUtils.println(MAlexa.sAlexaACInput1ID);
        EUtils.println(MAlexa.sEntrolRelayId);
        ui8Index =  1;
    }else{
        ui8Index=0xFF;
    }

    return ui8Index;
}

void Application::Check_For_Action(String sAction, String sValue,int i16jsonValue, int* ai16ResponseData){

    if(sAction.equals("setPowerState")){
        
        if(sValue.equals(DEVICE_ON)){
            Application_Pin_Write(RELAY, HIGH);
            ui8AlexaUpdated = 1;
        }else{
            Application_Pin_Write(RELAY, LOW);
            ui8AlexaUpdated = 1;
        }
    }else if(sAction == "action.devices.commands.OnOff"){
        if(i16jsonValue){
            Application_Pin_Write(RELAY, HIGH);
            ui8AlexaUpdated = 1;
        }else{
            Application_Pin_Write(RELAY, LOW);
            ui8AlexaUpdated = 1;
        }
    }else{
        // do nothing
        ai16ResponseData[2] = 0;
    }
}

// Preinstantiate Objects //////////////////////////////////////////////////////
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
Application EApp;
#endif