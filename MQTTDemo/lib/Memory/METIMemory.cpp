/*******************************************************************************
*File Name: METIMemory.c
*
* Version: 1.0
*
* Description:
* In this source code for eeprom usage
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/

#include <METIMemory.h>
#include <EEPROM.h>
#include <METIUtils.h>

uint8_t aui8MemoryMap[MEMORY_SIZE] = {
                                6,'I','O','T','D','e','v',0,0,0,0,0,0,0,0,0,
                                10,'I','O','T','D','e','v','1','2','3','4',0,0,0,0,0,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                1,
                            };
/*
    1st 16 char indicates ssid of wifi:
        0: number of bytes followed by
    2nd 16 char inidicates password of wifi
        0: number of bytes followed by
    3rd 16 gives hostname
    4th and 5th usees wifi config parameters
        0: Use static configuration
        1: Use static IP
        2: Use gateway
        3: Use subnet
        4: use dns
        5 to 15   : Reserved
    5th wifi settings
        0 to 3   : Statc IP
        4 to 7   : gateway
        8 to 11  : subnet
        12 to 15 : dns

    6th: Reserved:
    7th : AC config packets
*/


// Constructors ////////////////////////////////////////////////////////////////
METIMemory::METIMemory(){}

void METIMemory::Start(void){
    EEPROM.begin(MEMORY_SIZE);
    if(MEMORY_WRITE_DEFAULT){
        delay(10);
        Initial();
        delay(10);
        Restore();
    }
}

void METIMemory::Restore(void){
    uint16_t ui16LoopCounter=0;

    for(ui16LoopCounter=0;ui16LoopCounter<MEMORY_SIZE;ui16LoopCounter++){
        aui8MemoryMap[ui16LoopCounter] = EEPROM.read(ui16LoopCounter);
    }
}

void METIMemory::Initial(void){
    uint16_t ui16LoopCounter=0;

    if(EEPROM.read(INITIAL_CHECK_INDEX) != 1){
        for(ui16LoopCounter=0;ui16LoopCounter<MEMORY_SIZE;ui16LoopCounter++){
            EEPROM.write(ui16LoopCounter,aui8MemoryMap[ui16LoopCounter]);

            if(ui16LoopCounter == INITIAL_CHECK_INDEX){
                Serial1.println("Reached");
                Serial1.println(aui8MemoryMap[ui16LoopCounter]);
            }
        }
        EEPROM.commit();
        delay(10);
    }
}

void METIMemory::Store(void){
    uint16_t ui16LoopCounter=0;

    for(ui16LoopCounter=0;ui16LoopCounter<MEMORY_SIZE;ui16LoopCounter++){
        EEPROM.write(ui16LoopCounter,aui8MemoryMap[ui16LoopCounter]);
    }
    EEPROM.commit();
}

uint16_t METIMemory::MemoryMap_Read(uint16_t ui16Address){
    return aui8MemoryMap[ui16Address];
}

void METIMemory::MemoryMap_Write(uint16_t ui16Address,uint16_t ui16Value){
    aui8MemoryMap[ui16Address] = ui16Value;
}

String METIMemory::Read_SSID(void){
    String sSSID;
    uint8_t ui8LoopCounter=0;
    uint8_t ui8SSIDBytes=0;

    ui8SSIDBytes = MemoryMap_Read(SSID_BYTES_INDEX);
    if(ui8SSIDBytes){
        if(ui8SSIDBytes <= MAX_SSID_ALLOWED){
            for(ui8LoopCounter=0;ui8LoopCounter<ui8SSIDBytes;ui8LoopCounter++){
                sSSID  = String(sSSID + char(MemoryMap_Read(ui8LoopCounter+START_INDEX_SSID)));
            }
        }
    }

    return sSSID;
}

uint8_t METIMemory::Write_SSID(String sSSID){
    uint8 ui8Bytes;
    uint8_t ui8LoopCounter=0;

    ui8Bytes = sSSID.length();
    if(ui8Bytes){
        if(ui8Bytes <= MAX_SSID_ALLOWED){
            MemoryMap_Write(SSID_BYTES_INDEX,ui8Bytes);
            for(ui8LoopCounter=0;ui8LoopCounter<ui8Bytes;ui8LoopCounter++){
                MemoryMap_Write(START_INDEX_SSID+ui8LoopCounter,sSSID.charAt(ui8LoopCounter));
            }
            return 1;
        }
    }
    return 0;
}

String METIMemory::Read_Password(void){
    String sPassword;
    uint8_t ui8LoopCounter=0;
    uint8_t ui8PasswordBytes=0;

    ui8PasswordBytes = MemoryMap_Read(PASSWORD_BYTES_INDEX);
    if(ui8PasswordBytes){
        if(ui8PasswordBytes <= MAX_PASSWORD_ALLOWED){
            for(ui8LoopCounter=0;ui8LoopCounter<ui8PasswordBytes;ui8LoopCounter++){
                sPassword  = String(sPassword + char(MemoryMap_Read(ui8LoopCounter+START_INDEX_PASSWORD)));
            }
        }
    }

    return sPassword;
}

uint8_t METIMemory::Write_Password(String sPassword){
    uint8 ui8Bytes;
    uint8_t ui8LoopCounter=0;

    ui8Bytes = sPassword.length();
    if(ui8Bytes){
        if(ui8Bytes <= MAX_PASSWORD_ALLOWED){
            MemoryMap_Write(PASSWORD_BYTES_INDEX,ui8Bytes);
            for(ui8LoopCounter=0;ui8LoopCounter<ui8Bytes;ui8LoopCounter++){
                MemoryMap_Write(START_INDEX_PASSWORD+ui8LoopCounter,sPassword.charAt(ui8LoopCounter));
            }
            return 1;
        }
    }
    return 0;
}

String METIMemory::Read_Hostname(void){
    String sHostname;
    uint8_t ui8LoopCounter=0;
    uint8_t ui8HostnameBytes=0;

    ui8HostnameBytes = MemoryMap_Read(HOSTNAME_BYTES_INDEX);
    if(ui8HostnameBytes){
        if(ui8HostnameBytes <= MAX_HOSTNAME_ALLOWED){
            for(ui8LoopCounter=0;ui8LoopCounter<ui8HostnameBytes;ui8LoopCounter++){
                sHostname  = String(sHostname + char(MemoryMap_Read(ui8LoopCounter+START_INDEX_HOSTNAME)));
            }
        }
    }

    return sHostname;
}

uint8_t METIMemory::Write_Hostname(String sHostname){
    uint8 ui8Bytes;
    uint8_t ui8LoopCounter=0;

    ui8Bytes = sHostname.length();
    if(ui8Bytes){
        if(ui8Bytes <= MAX_HOSTNAME_ALLOWED){
            MemoryMap_Write(HOSTNAME_BYTES_INDEX,ui8Bytes);
            for(ui8LoopCounter=0;ui8LoopCounter<ui8Bytes;ui8LoopCounter++){
                MemoryMap_Write(START_INDEX_HOSTNAME+ui8LoopCounter,sHostname.charAt(ui8LoopCounter));
            }
            return 1;
        }
    }
    return 0;
}

void METIMemory::Read_ACControl(uint8_t* aui8Data){
    uint8_t ui8LoopCounter=0;

    for(ui8LoopCounter=0;ui8LoopCounter<16;ui8LoopCounter++){
        aui8Data[ui8LoopCounter] = MemoryMap_Read(ui8LoopCounter + ACCONTROL_START_INDEX);
    }
}

void METIMemory::Write_ACControl(uint8_t* aui8Data){
    uint8_t ui8LoopCounter=0;

    for(ui8LoopCounter=0;ui8LoopCounter<16;ui8LoopCounter++){
        MemoryMap_Write((ui8LoopCounter + ACCONTROL_START_INDEX),aui8Data[ui8LoopCounter]);
    }
}

void METIMemory::Read_Wifi_Config_Status(uint8_t* aui8Data){
    uint8_t ui8LoopCounter=0;

    for(ui8LoopCounter=0;ui8LoopCounter<16;ui8LoopCounter++){
        aui8Data[ui8LoopCounter] = MemoryMap_Read(ui8LoopCounter + WIFI_CONFIG_STATUS_START_INDEX);
    }
}

void METIMemory::Write_Wifi_Config_Status(uint8_t* aui8Data){
    uint8_t ui8LoopCounter=0;

    for(ui8LoopCounter=0;ui8LoopCounter<16;ui8LoopCounter++){
        MemoryMap_Write((ui8LoopCounter + WIFI_CONFIG_STATUS_START_INDEX),aui8Data[ui8LoopCounter]);
    }

}

void METIMemory::Read_Wifi_Config_Parameters(uint8_t* aui8Data){
    uint8_t ui8LoopCounter=0;

    for(ui8LoopCounter=0;ui8LoopCounter<16;ui8LoopCounter++){
        aui8Data[ui8LoopCounter] = MemoryMap_Read(ui8LoopCounter + WIFI_CONFIG_PARA_START_INDEX);
    }
}

void METIMemory::Write_Wifi_Config_Parameters(uint8_t* aui8Data){
    uint8_t ui8LoopCounter=0;

    for(ui8LoopCounter=0;ui8LoopCounter<16;ui8LoopCounter++){
        MemoryMap_Write((ui8LoopCounter + WIFI_CONFIG_PARA_START_INDEX),aui8Data[ui8LoopCounter]);
    }

}

// Preinstantiate Objects //////////////////////////////////////////////////////

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
METIMemory MMemory;
#endif
