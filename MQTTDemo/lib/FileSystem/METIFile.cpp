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
#include <ESP8266WebServer.h>
#include <FS.h>
#include <METIUtils.h>
#include <METIMemory.h>
#include <METIFile.h>

// Constructors ////////////////////////////////////////////////////////////////
METIFile::METIFile(){}

void METIFile::Start(void){
    uint8_t ui8LoopCounter=0;
    uint8_t ui8Status=0;
    String sLog;
    String sFileName;

    //Delete_All_Files();

    Read_FileInfo();

    for(ui8LoopCounter=0;ui8LoopCounter<MAX_FILE_COUNT;ui8LoopCounter++){
        ui8Status = CheckFile(sMETIFiles[ui8LoopCounter],&aui32METIFileSize[ui8LoopCounter]);
        if(ui8Status == FILE_NOT_EXIST){
            Serial1.printf("File %s doenot exist\r\n",sMETIFiles[ui8LoopCounter]);
            ui8Status = CreateFile(sMETIFiles[ui8LoopCounter],&aui32METIFileSize[ui8LoopCounter]);
            if(ui8Status == FILE_CREATE_SUCCESS){
                Serial1.printf("File %s created and size %d \r\n",sMETIFiles[ui8LoopCounter],aui32METIFileSize[ui8LoopCounter]);
            }else{
                Serial1.printf("File %s create fail\r\n",sMETIFiles[ui8LoopCounter]);
            }
        }else{
            Serial1.printf("File %s exist\r\n",sMETIFiles[ui8LoopCounter]);
        }
        Serial1.println("Complete current");
    }
    Serial1.println("Complete all");

    ui8Status = CheckFile("/log.txt",&ui32LogFileSize);
    if(ui8Status == FILE_NOT_EXIST){
        Serial1.printf("File /log.txt doenot exist\r\n");
        ui8Status = CreateFile("/log.txt",&ui32LogFileSize);
        if(ui8Status == FILE_CREATE_SUCCESS){
            Serial1.printf("File /log.txt created\r\n");
        }else{
            Serial1.printf("File log.txt create fail\r\n");
        }
    }else{
        Serial1.printf("File /log.txt exist\r\n");
    }

    File fFile = SPIFFS.open("/log.txt","a");
    sLog = MUtils.GetTime() + " Device restarted";
    fFile.println(sLog);
    ui32LogFileSize = fFile.size();
    fFile.close();

    Read_FileInfo();
}

void METIFile::Read_FileInfo(void){

    SPIFFS.info(fFileInfo);
    Serial1.println("SPIFFS file info \r\n");
    Serial1.printf("totalBytes    : %d \r\n",fFileInfo.totalBytes);
    Serial1.printf("usedBytes     : %d \r\n",fFileInfo.usedBytes);
    Serial1.printf("blockSize     : %d \r\n",fFileInfo.blockSize);
    Serial1.printf("pageSize      : %d \r\n",fFileInfo.pageSize);
    Serial1.printf("maxOpenFiles  : %d \r\n",fFileInfo.maxOpenFiles);
    Serial1.printf("maxPathLength : %d \r\n",fFileInfo.maxPathLength);
}

uint8_t METIFile::CreateFile(String sFileName,uint32_t* ui32FileSize){
    String sData;
    uint8_t ui8FileStatus;

    File fFile = SPIFFS.open(sFileName,"r");
    if(fFile){
        ui8FileStatus = FILE_ALREADY_CREATED;
    }else{
        fFile = SPIFFS.open(sFileName,"w");
        if(!fFile){
            ui8FileStatus =  FILE_CREATE_FAIL;
        }else{
            sData = " ";
            fFile.print(sData);
            *ui32FileSize = fFile.size();
            ui8FileStatus =  FILE_CREATE_SUCCESS;
        }
    }

    /*File fFile = SPIFFS.open(sFileName,"w");
    if(!fFile){
        ui8FileStatus =  FILE_CREATE_FAIL;
    }else{
        sData = "PacketCount,PacketStatus,Packet";
        fFile.println(sData);
        ui8FileStatus =  FILE_CREATE_SUCCESS;
    }*/

    fFile.close();
    return ui8FileStatus;
}

uint8_t METIFile::DeleteFile(String sFileName,uint32_t* ui32FileSize){
    uint8_t ui8FileStatus;
    File fFile = SPIFFS.open(sFileName,"r");

    if(!fFile){
        ui8FileStatus = FILE_NOT_EXIST;
        fFile.close();
    }else{
        SPIFFS.remove(sFileName);
        delay(1000);
        fFile = SPIFFS.open(sFileName,"r");
        if(fFile){
            ui8FileStatus = FILE_DELETE_FAIL;
            fFile.close();
        }else{
            ui8FileStatus = FILE_DELETE_SUCCESS;
        }
    }

    *ui32FileSize = 0;

    return ui8FileStatus;
}

uint8_t METIFile::CheckFile(String sFileName,uint32_t* ui32FileSize){
    uint8_t ui8FileStatus;
    File fFile = SPIFFS.open(sFileName,"r");
    if(fFile){
        *ui32FileSize = fFile.size();
        fFile.close();
        ui8FileStatus = FILE_EXIST;
    }else{
        *ui32FileSize = 0;
        ui8FileStatus = FILE_NOT_EXIST;
    }

    return ui8FileStatus;
}

uint8_t METIFile::Write_Data_To_File(String sFileName, String sData){
    uint8_t ui8Status;
    String sLog;

    File fFile = SPIFFS.open(sFileName,"w");
    if(!fFile){
        ui8Status = FILE_STORE_FAIL;
        sLog = "FILE_STORE_FAIL " + sFileName;
    }else{
        fFile.println(sData);
        ui8Status = FILE_STORE_SUCCESS;
    }

    fFile.close();

    return ui8Status;
}

String METIFile::Read_Data_From_File(String sFileName){
    String sString;
    uint16_t ui16Length=0;

    File fFile = SPIFFS.open(sFileName,"r");
    
    if(fFile){
        sString = fFile.readStringUntil('\n');
    }
    
    fFile.close();

    if(sString){
        ui16Length = sString.length();
        sString.remove(ui16Length-1);
    }
    
    return sString;
}

uint8_t METIFile::Store_To_File(String sData){
   /* uint8_t  ui8FileIndex=0;
    uint8_t  ui8Status=0;
    String   sPacket;
    uint32_t ui32StoreCounter=0;
    String   sLog;
    uint8_t  ui8LogFlag=0;

    ui8FileIndex = MMemory.Read_DataLogger_StoreFile();
    ui32StoreCounter = MMemory.Read_DataLogger_StoreCount();
    ui32StoreCounter = ui32StoreCounter + 1;

    if(ui8FileIndex < MAX_FILE_COUNT){
        File fFile = SPIFFS.open(sMETIFiles[ui8FileIndex],"a");
        if(!fFile){
            ui8Status = FILE_STORE_FAIL;
            sLog = "FILE_STORE_FAIL " + String(ui8FileIndex);
            ui8LogFlag = 1;
        }else{
            if(fFile.size() > MAX_FILE_SIZE_IN_BYTES){
                if(ui8FileIndex > (MAX_FILE_COUNT - 2)){
                    ui8FileIndex = 0;
                    ui8Status = DeleteFile(sMETIFiles[ui8FileIndex],&aui32METIFileSize[ui8FileIndex]);
                    if(ui8Status == FILE_DELETE_SUCCESS){
                        sLog = sLog + " FILE_DELETE_SUCCESS " + String(ui8FileIndex);
                        ui8LogFlag = 1;
                        ui8Status = CreateFile(sMETIFiles[ui8FileIndex],&aui32METIFileSize[ui8FileIndex]);
                        if(ui8Status == FILE_CREATE_SUCCESS){
                            sLog = sLog + " FILE_CREATE_SUCCESS " + String(ui8FileIndex);
                            ui8LogFlag = 1;
                            MMemory.Write_DataLogger_StoreFile(ui8FileIndex);
                            ui32StoreCounter = 1;
                            fFile = SPIFFS.open(sMETIFiles[ui8FileIndex],"a");
                            sPacket = String(ui32StoreCounter) + "," + String(1) + "," + MUtils.GetTime() + "," + sData;
                            fFile.println(sPacket);
                            MMemory.Write_DataLogger_StoreCount(ui32StoreCounter);
                        }else{
                            sLog = sLog + " FILE_CREATE_FAIL " + String(ui8FileIndex);
                            ui8LogFlag = 1;
                        }
                    }else{
                        sLog = sLog + " FILE_DELETE_FAIL " + String(ui8FileIndex);
                        ui8LogFlag = 1;
                    }
                }else{
                    ui8FileIndex = ui8FileIndex + 1;
                    sLog = sLog + " Data to NewFile " + String(ui8FileIndex);
                    ui8LogFlag = 1;
                    fFile = SPIFFS.open(sMETIFiles[ui8FileIndex],"a");
                    sPacket = String(ui32StoreCounter) + "," + String(1) + "," + MUtils.GetTime() + "," + sData;
                    fFile.println(sPacket);
                    MMemory.Write_DataLogger_StoreCount(ui32StoreCounter);
                    MMemory.Write_DataLogger_StoreFile(ui8FileIndex);
                }

            }else{
                sPacket = String(ui32StoreCounter) + "," + String(1) + "," + MUtils.GetTime() + "," + sData;
                fFile.println(sPacket);
                MMemory.Write_DataLogger_StoreCount(ui32StoreCounter);
            }

            aui32METIFileSize[ui8FileIndex] = fFile.size();
            fFile.close();
        }
    }else{
        ui8Status = FILE_NAME_ERROR;
        sLog = " FILE_NAME_ERROR " + String(ui8FileIndex);
        ui8LogFlag = 1;
    }

    if(ui8LogFlag){
        Store_To_LogFile(sLog);
    }*/

    return 0;
}

void METIFile::Store_To_LogFile(String sData){
    /*uint8_t ui8Status;
    uint32_t ui32FileSize;
    String  sLogError;

    File fFile = SPIFFS.open("/log.txt","a");

    if(fFile){
        ui32FileSize = fFile.size();
        if(ui32FileSize > MAX_LOG_FILE_SIZE_IN_BYTES){
            sLogError = " MAX_LOG_FILE_SIZE_IN_BYTES ";
            ui8Status = DeleteFile("/log.txt",&ui32LogFileSize);
            if(ui8Status == FILE_DELETE_SUCCESS){
                sLogError = sLogError + " FILE_DELETE_SUCCESS ";
                ui8Status = CreateFile("/log.txt",&ui32LogFileSize);
                if(ui8Status == FILE_CREATE_SUCCESS){
                    sLogError = sLogError + " FILE_DELETE_SUCCESS ";
                    sLogError = MUtils.GetTime() + "= " + sLogError;
                    fFile.println(sLogError);
                    sLogError = MUtils.GetTime() + " = " + sData;
                    fFile.println(sLogError);
                    ui32LogFileSize = fFile.size();
                    fFile.close();
                }
            }
        }else{
            sLogError = MUtils.GetTime() + " = " + sData;
            fFile.println(sLogError);
            ui32LogFileSize = fFile.size();
            fFile.close();
        }
        Serial1.println("Log data added");
    }else{
        ui8Status = FILE_LOG_FAIL;
    }*/
}

uint32_t METIFile::Get_File_Size(uint8_t ui8Index){
    return aui32METIFileSize[ui8Index];
}

uint32_t METIFile::Get_LogFile_Size(void){
    return ui32LogFileSize;
}

void METIFile::Delete_All_Files(void){
    uint8_t ui8LoopCounter=0;
    uint8_t ui8Status;

    Serial1.println("Deleting all files");
    for(ui8LoopCounter=0;ui8LoopCounter<MAX_FILE_COUNT;ui8LoopCounter++){
        ui8Status = CheckFile(sMETIFiles[ui8LoopCounter],&aui32METIFileSize[ui8LoopCounter]);
        if(ui8Status == FILE_EXIST){
            ui8Status = DeleteFile(sMETIFiles[ui8LoopCounter],&aui32METIFileSize[ui8LoopCounter]);
            if(ui8Status == FILE_DELETE_SUCCESS){
                Serial1.printf("File %s deleted\r\n",sMETIFiles[ui8LoopCounter]);
            }else{
                Serial1.printf("File %s deleted fail\r\n",sMETIFiles[ui8LoopCounter]);
            }
        }
    }

    ui8Status = CheckFile("/log.txt",&ui32LogFileSize);
    if(ui8Status == FILE_EXIST){
        ui8Status = DeleteFile("/log.txt",&ui32LogFileSize);
        if(ui8Status == FILE_DELETE_SUCCESS){
            Serial1.printf("File /log.txt deleted\r\n");
        }else{
            Serial1.printf("File /log.txt deleted fail\r\n");
        }
    }
}

String METIFile::ReStore_From_File(uint32_t ui32LineToRead){
    String sString;
    //uint32_t ui32LoopCounter=0;

    //File fFile = SPIFFS.open("/DataLogger1.csv","r");

    //for(ui32LoopCounter=0;ui32LoopCounter<ui32LineToRead;ui32LoopCounter++){
    //    fFile.readStringUntil('\n');
    //}

    //sString = fFile.readStringUntil('\n');

    //fFile.close();

    return sString;
}

void METIFile::File_Test(String sFileName){
    MUtils.println("File_Test");
    MUtils.println("Writing to file : Test Test Test");
    
    if(Write_Data_To_File(sFileName,"Test Test Test") == FILE_STORE_SUCCESS){
        delay(1000);
        MUtils.println("Read data");
        MUtils.println(Read_Data_From_File(sFileName));
    }
}

// Preinstantiate Objects //////////////////////////////////////////////////////
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
METIFile MFile;
#endif
