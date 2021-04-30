/*******************************************************************************
* File Name: ModbusSlave.h
*
* Version: 1.0
*
* Description:
* This is header file for memory. All the functions related to
* Memory are implemented in this file
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
#ifndef ESPMemory_h
#define ESPMemory_h

#include <Arduino.h>

#define MEMORY_SIZE       10

#define MEMORY_RELAY_STATUS_BYTE    0
#define MEMORY_GREENL_STATUS_BYTE   1
#define MEMORY_YELLOWL_STATUS_BYTE  2


#define MEMORY_INIT_BYTE            9

class ESPMemory{
public:
    ESPMemory();
    void Init(void);
    void Update_All(void);
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
extern ESPMemory EMemory;
#endif
#endif