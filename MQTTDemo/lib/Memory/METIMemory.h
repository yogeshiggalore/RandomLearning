/*******************************************************************************
*File Name: METIMemory.h
*
* Version: 1.0
*
* Description:
* In this header used for eeprom functions
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/
#ifndef METIMemory_h
#define METIMemory_h
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

#define MEMORY_SIZE 129

#define INITIAL_CHECK_INDEX 128
#define MEMORY_WRITE_DEFAULT    1

#define MAX_SSID_ALLOWED        15
#define MAX_PASSWORD_ALLOWED    15
#define MAX_HOSTNAME_ALLOWED    15

#define SSID_BYTES_INDEX    0
#define START_INDEX_SSID    1

#define PASSWORD_BYTES_INDEX    16
#define START_INDEX_PASSWORD    17

#define HOSTNAME_BYTES_INDEX    32
#define START_INDEX_HOSTNAME    33

#define WIFI_CONFIG_STATUS_START_INDEX  48 
#define WIFI_CONFIG_PARA_START_INDEX    64
#define ACCONTROL_START_INDEX           96

class METIMemory{
public:
    METIMemory();
    void Start(void);
    void Initial(void);
    void Restore(void);
    void Store(void);
    uint16_t MemoryMap_Read(uint16_t ui16Address);
    void MemoryMap_Write(uint16_t ui16Address,uint16_t ui16Value);
    String Read_SSID(void);
    uint8_t Write_SSID(String sSSID);
    String Read_Password(void);
    uint8_t Write_Password(String sPassword);
    String Read_Hostname(void);
    uint8_t Write_Hostname(String sHostname);
    
    void Read_Wifi_Config_Status(uint8_t* aui8Data);
    void Write_Wifi_Config_Status(uint8_t* aui8Data);
    void Read_Wifi_Config_Parameters(uint8_t* aui8Data);
    void Write_Wifi_Config_Parameters(uint8_t* aui8Data);

    void Read_ACControl(uint8_t* aui8Data);
    void Write_ACControl(uint8_t* aui8Data);
    
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
extern METIMemory MMemory;
#endif
#endif
