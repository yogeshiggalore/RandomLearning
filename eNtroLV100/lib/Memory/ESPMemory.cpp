/*******************************************************************************
* File Name: ESPMemory.cpp
*
* Version: 1.0
*
* Description:
* This is source file for memory. All the functions related to
* memory are implemented in this file
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
#include <EEPROM.h>
#include <ESPMemory.h>
#include <Application.h>

// Constructors ////////////////////////////////////////////////////////////////
ESPMemory::ESPMemory(){}

/* ini memory and update all bytes*/
void ESPMemory::Init(void){
    EEPROM.begin(MEMORY_SIZE);
    if(EEPROM.read(MEMORY_INIT_BYTE) != 1){
        EEPROM.write(MEMORY_RELAY_STATUS_BYTE,0);
        EEPROM.write(MEMORY_GREENL_STATUS_BYTE,0);
        EEPROM.write(MEMORY_YELLOWL_STATUS_BYTE,0);
        EEPROM.write(MEMORY_INIT_BYTE,1);
        EEPROM.commit();
    }

	/* update all memory bytes */
	Update_All();
}

/* This function updates all memory bytes */
void ESPMemory::Update_All(void){
    EApp.ui8RelayStatus = EEPROM.read(MEMORY_RELAY_STATUS_BYTE);
    EApp.ui8GreenLedStatus = EEPROM.read(MEMORY_GREENL_STATUS_BYTE);
    EApp.ui8YellowLedStatus = EEPROM.read(MEMORY_YELLOWL_STATUS_BYTE);
}

// Preinstantiate Objects //////////////////////////////////////////////////////
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
ESPMemory EMemory;
#endif