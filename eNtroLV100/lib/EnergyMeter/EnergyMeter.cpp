/*******************************************************************************
* File Name: ModbusMaster.cpp
*
* Version: 1.0
*
* Description:
* This is source file for energy meter. All the functions related to
* energy meter are implemented in this file
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
#include <EnergyMeter.h>
#include <ModbusUtils.h>
#include <ESPUtils.h>

// Constructors ////////////////////////////////////////////////////////////////
EnergyMeter::EnergyMeter(){}

void EnergyMeter::Process(void){
    uint16_t ui16Value=0;
    uint32_t ui32Value=0;

    //Process voltage
    ui16Value = ModUtils.aui8ModbusRxBuffer[3];
    ui16Value = ui16Value << 8;
    ui16Value = ui16Value | ModUtils.aui8ModbusRxBuffer[4];

    fVoltage = (float)ui16Value/10.0;

    //Process current 
    ui32Value = ModUtils.aui8ModbusRxBuffer[7];
    ui32Value = ui32Value << 8;
    ui32Value = ui32Value | ModUtils.aui8ModbusRxBuffer[8];
    ui32Value = ui32Value << 8;
    ui32Value = ui32Value | ModUtils.aui8ModbusRxBuffer[5];
    ui32Value = ui32Value << 8;
    ui32Value = ui32Value | ModUtils.aui8ModbusRxBuffer[6];

    fCurrent = (float)ui32Value/100.0;

    //process power 
    ui32Value = ModUtils.aui8ModbusRxBuffer[11];
    ui32Value = ui32Value << 8;
    ui32Value = ui32Value | ModUtils.aui8ModbusRxBuffer[12];
    ui32Value = ui32Value << 8;
    ui32Value = ui32Value | ModUtils.aui8ModbusRxBuffer[9];
    ui32Value = ui32Value << 8;
    ui32Value = ui32Value | ModUtils.aui8ModbusRxBuffer[10];

    fPower = (float)ui32Value / 1000.0;

    //process unit 
    ui32Value = ModUtils.aui8ModbusRxBuffer[15];
    ui32Value = ui32Value << 8;
    ui32Value = ui32Value | ModUtils.aui8ModbusRxBuffer[16];
    ui32Value = ui32Value << 8;
    ui32Value = ui32Value | ModUtils.aui8ModbusRxBuffer[13];
    ui32Value = ui32Value << 8;
    ui32Value = ui32Value | ModUtils.aui8ModbusRxBuffer[14];

    fUnit = (float)ui32Value / 1000.0;

    // process frequency 
    ui16Value = ModUtils.aui8ModbusRxBuffer[17];
    ui16Value = ui16Value << 8;
    ui16Value = ui16Value | ModUtils.aui8ModbusRxBuffer[18];

    fFrequency = (float)ui16Value / 10.0;

    // process powerfactor
    ui16Value = ModUtils.aui8ModbusRxBuffer[19];
    ui16Value = ui16Value << 8;
    ui16Value = ui16Value | ModUtils.aui8ModbusRxBuffer[20];
    fPowerfactor = (float)ui16Value / 10.0;

    Serial1.print("Volt: ");
    Serial1.println(fVoltage);
    Serial1.print("Current: ");
    Serial1.println(fCurrent);
    Serial1.print("Power: ");
    Serial1.println(fPower);
    Serial1.print("Unit: ");
    Serial1.println(fUnit);
    Serial1.print("Frequency: ");
    Serial1.println(fFrequency);
    Serial1.print("Powerfactor: ");
    Serial1.println(fPowerfactor);
}

// Preinstantiate Objects //////////////////////////////////////////////////////
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
EnergyMeter EMeter;
#endif