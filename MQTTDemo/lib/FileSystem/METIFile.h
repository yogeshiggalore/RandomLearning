/*******************************************************************************
*File Name: METIFile.h
*
* Version: 1.0
*
* Description:
* In this header used for managing files in spiffs system
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/
#ifndef METIFILE_h
#define METIFILE_h
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FS.h>

#define FILE_NONE                0
#define FILE_NOT_EXIST           1
#define FILE_EXIST               2
#define FILE_ALREADY_CREATED     3
#define FILE_CREATE_SUCCESS      4
#define FILE_CREATE_FAIL         5
#define FILE_READ_FAIL           6
#define FILE_READ_SUCCESS        7
#define FILE_WRITE_FAIL          8
#define FILE_WRITE_SUCCESS       9
#define FILE_APPEND_FAIL         10
#define FILE_APPEND_SUCCESS      11
#define FILE_DELETE_FAIL         12
#define FILE_DELETE_SUCCESS      13
#define FILE_STORE_FAIL          14
#define FILE_STORE_SUCCESS       15
#define FILE_NAME_ERROR          16
#define FILE_LOG_FAIL            17
#define FILE_LOG_SUCESS          18


#define MAX_FILE_COUNT   21

#define ALEXA_TOKEN_INDEX      0
#define ALEXA_AC_1_ID_INDEX    1
#define ALEXA_AC_2_ID_INDEX    2
#define ALEXA_AC_3_ID_INDEX    3
#define ALEXA_AC_4_ID_INDEX    4
#define ALEXA_AC_1_NAME_INDEX  5
#define ALEXA_AC_2_NAME_INDEX  6
#define ALEXA_AC_3_NAME_INDEX  7
#define ALEXA_AC_4_NAME_INDEX  8

#define MQTT_PUB_AC_1_TOPIC_INDEX 9
#define MQTT_PUB_AC_2_TOPIC_INDEX 10
#define MQTT_PUB_AC_3_TOPIC_INDEX 11
#define MQTT_PUB_AC_4_TOPIC_INDEX 12

#define MQTT_SUB_AC_TOPIC_INDEX   13

#define MQTT_SERVER_INDEX   14
#define MQTT_PORT_INDEX     15
#define MQTT_USERNAME_INDEX 16
#define MQTT_PASSWORD_INDEX 17
#define MQTT_CLIENTID_INDEX 18

#define MQTT_QOS_INDEX       19
#define MQTT_KEEPALIVE_INDEX 20

static const char *sMETIFiles[] = {
    "/AlexaToken.txt",
    "/AlexaAC1Id.txt",
    "/AlexaAC2Id.txt",
    "/AlexaAC3Id.txt",
    "/AlexaAC4Id.txt",
    "/AlexaAC1Name.txt",
    "/AlexaAC2Name.txt",
    "/AlexaAC3Name.txt",
    "/AlexaAC4Name.txt",
    "/MqttPubAC1Topic.txt",
    "/MqttPubAC2Topic.txt",
    "/MqttPubAC3Topic.txt",
    "/MqttPubAC4Topic.txt",
    "/MqttSubACTopic.txt",
    "/MqttServer.txt",
    "/MqttPort.txt",
    "/MqttUsername.txt",
    "/MqttPassword.txt",
    "/MqttClientid.txt",
    "/MqttQos.txt",
    "/MqttKeepalive.txt"
};

class METIFile{
public:
    uint32_t ui32LogFileSize;
    uint32_t aui32METIFileSize[MAX_FILE_COUNT];
    FSInfo fFileInfo;

    METIFile();
    void Start(void);
    uint8_t CreateFile(String sFileName,uint32_t *ui32FileSize);
    uint8_t DeleteFile(String sFileName,uint32_t *ui32FileSize);
    uint8_t CheckFile(String sFileName,uint32_t *ui32FileSize);
    uint8_t Store_To_File(String sData);
    void Store_To_LogFile(String sData);
    uint32_t Get_File_Size(uint8_t ui8Index);
    uint32_t Get_LogFile_Size(void);
    void Delete_All_Files(void);
    void Read_FileInfo(void);
    String ReStore_From_File(uint32_t ui32LineToRead);
    uint8_t Write_Data_To_File(String sFileName, String sData);
    String Read_Data_From_File(String sFileName);

    void File_Test(String sFileName);
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
extern METIFile MFile;
#endif
#endif
