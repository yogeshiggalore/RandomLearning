/*******************************************************************************
*File Name: METIAlexa.cpp
*
* Version: 1.0
*
* Description:
* In this source code for alexa usage
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/

#include <METIAlexaV2.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h> 
#include <StreamString.h>
#include <METIUtils.h>
#include <FS.h>
#include <METIFile.h>
#include <Application.h>
#include <ACControl.h>

uint8_t ui8IsConnected=0;
uint64_t heartbeatTimestamp = 0;
uint8_t ui8HallLightValue=0;
uint8_t ui8RoomLightValue=0;
uint8_t ui8KitchenLightValue=0;
uint8_t ui8ChangeValueFlag=0;

METIAlexa::METIAlexa(void):webSocket(),client(){

}

String Get_StringValue(uint8_t ui8Device, uint8_t ui8Value){
    unsigned char ui8dig1,ui8dig2,ui8dig3,sDig[3];
    unsigned char ui8Temp;
    String sStringValue;

    ui8Temp=ui8Value;
    ui8Temp=ui8Temp/10;
    ui8dig1=ui8Value%10;
    ui8dig2=ui8Temp%10;
    ui8dig3=ui8Temp/10;

    sDig[0]=ui8dig3;
    sDig[1]=ui8dig2;
    sDig[2]=ui8dig1;

    MUtils.println(sDig[0]);
    MUtils.println(sDig[1]);
    MUtils.println(sDig[2]);

    if(ui8Device == HALL_LIGHT_FLAG){
        sStringValue = "A" ;
        sStringValue = sStringValue + sDig[0];
        sStringValue = sStringValue + sDig[1];
        sStringValue = sStringValue + sDig[2];
        MUtils.println("HALL_LIGHT_FLAG");
        MUtils.println(sStringValue);
    }else if(ui8Device == ROOM_LIGHT_FLAG){
        sStringValue = "B" ;
        sStringValue = sStringValue + sDig[0];
        sStringValue = sStringValue + sDig[1];
        sStringValue = sStringValue + sDig[2];
        MUtils.println("ROOM_LIGHT_FLAG");
        MUtils.println(sStringValue);
    }else if(ui8Device == KITCHEN_LIGHT_FLAG){
        sStringValue = "C" ;
        sStringValue = sStringValue + sDig[0];
        sStringValue = sStringValue + sDig[1];
        sStringValue = sStringValue + sDig[2];
        MUtils.println("KITCHEN_LIGHT_FLAG");
        MUtils.println(sStringValue);
    }else{
        //do nothing
    }
    MUtils.println(sStringValue);
    return sStringValue;
}

void handleSerialDimmer(void){
    String sSerialValue;

    if(ui8ChangeValueFlag){
        MUtils.println(ui8ChangeValueFlag);
        MUtils.println(ui8HallLightValue);
        MUtils.println(ui8RoomLightValue);
        MUtils.println(ui8KitchenLightValue);
        if(ui8ChangeValueFlag == HALL_LIGHT_FLAG){
            sSerialValue = Get_StringValue(HALL_LIGHT_FLAG,ui8HallLightValue);
        }else if(ui8ChangeValueFlag == ROOM_LIGHT_FLAG){
            sSerialValue = Get_StringValue(ROOM_LIGHT_FLAG,ui8RoomLightValue);
        }else if(ui8ChangeValueFlag == KITCHEN_LIGHT_FLAG){
            sSerialValue = Get_StringValue(KITCHEN_LIGHT_FLAG,ui8KitchenLightValue);
        }else{
            // do nothing
        }
        ui8ChangeValueFlag = 0;
        if(sSerialValue){
            Serial.print(sSerialValue);
        }
    }
}

void Action_Selector(uint8_t * payload){
    DynamicJsonDocument json(1024);
    deserializeJson(json, payload);

    String sDeviceId = json ["deviceId"];
    String sAction = json ["action"];
    String sValue = json["value"];
    int i16Value = json["value"];

    MUtils.println(sAction);
    MUtils.println(sDeviceId);
    MUtils.println(sValue);
    if(sAction == "setPowerState"){
        if(sDeviceId == HALL_LIGHT_ID){
            ui8ChangeValueFlag = HALL_LIGHT_FLAG;
            if(sValue == DEVICE_ON){
                ui8HallLightValue = 100;
                MUtils.println("Turning on hall light");
            }else{
                ui8HallLightValue = 0;
                MUtils.println("Turning off hall light");
            }
        }else if(sDeviceId == ROOM_LIGHT_ID){
            ui8ChangeValueFlag = ROOM_LIGHT_FLAG;
            if(sValue == DEVICE_ON){
                ui8RoomLightValue = 100;
                MUtils.println("Turning on room light");
            }else{
                ui8RoomLightValue = 0;
                MUtils.println("Turning off room light");
            }
        }else if(sDeviceId == KITCHEN_LIGHT_ID){
            ui8ChangeValueFlag = KITCHEN_LIGHT_FLAG;
            if(sValue == DEVICE_ON){
                ui8KitchenLightValue = 100;
                MUtils.println("Turning on kitchen light");
            }else{
                ui8KitchenLightValue = 0;
                MUtils.println("Turning off kitchen light");
            }
        }else{
            MUtils.println("No device specified in code");
        }
    }else if(sAction == "SetBrightness"){
        if(sDeviceId == HALL_LIGHT_ID){
            ui8ChangeValueFlag = HALL_LIGHT_FLAG;
            ui8HallLightValue = i16Value;
            MUtils.println("setting hall light brightness");
        }else if(sDeviceId == ROOM_LIGHT_ID){
            ui8ChangeValueFlag = ROOM_LIGHT_FLAG;
            ui8RoomLightValue = i16Value;
            MUtils.println("setting room light brightness");
        }else if(sDeviceId == KITCHEN_LIGHT_ID){
            ui8ChangeValueFlag = KITCHEN_LIGHT_FLAG;
            ui8KitchenLightValue = i16Value;
            MUtils.println("setting kitchen light brightness");
        }else{
            MUtils.println("No device specified in code");
        }
    }else if(sAction == "AdjustBrightness"){
        if(sDeviceId == HALL_LIGHT_ID){
            if(i16Value < 0){
                if(ui8HallLightValue >= ALEXA_BRIGHTNESS_LEVEL){
                    ui8HallLightValue = ui8HallLightValue - ALEXA_BRIGHTNESS_LEVEL;
                    ui8ChangeValueFlag = HALL_LIGHT_FLAG;
                    MUtils.println("Adjusting brightness of hall light");
                }
            }else{
                if(ui8HallLightValue < 100){
                    ui8HallLightValue = ui8HallLightValue + ALEXA_BRIGHTNESS_LEVEL;
                    ui8ChangeValueFlag = HALL_LIGHT_FLAG;
                    MUtils.println("Adjusting brightness of hall light");
                }
            }
        }else if(sDeviceId == ROOM_LIGHT_ID){
            if(i16Value < 0){
                if(ui8RoomLightValue >= ALEXA_BRIGHTNESS_LEVEL){
                    ui8RoomLightValue = ui8RoomLightValue - ALEXA_BRIGHTNESS_LEVEL;
                    ui8ChangeValueFlag = ROOM_LIGHT_FLAG;
                    MUtils.println("Adjusting brightness of room light");
                }
            }else{
                if(ui8RoomLightValue < 100){
                    ui8RoomLightValue = ui8RoomLightValue + ALEXA_BRIGHTNESS_LEVEL;
                    ui8ChangeValueFlag = ROOM_LIGHT_FLAG;
                    MUtils.println("Adjusting brightness of room light");
                }
            }
        }else if(sDeviceId == KITCHEN_LIGHT_ID){
            if(i16Value < 0){
                if(ui8KitchenLightValue >= ALEXA_BRIGHTNESS_LEVEL){
                    ui8KitchenLightValue = ui8KitchenLightValue - ALEXA_BRIGHTNESS_LEVEL;
                    ui8ChangeValueFlag = KITCHEN_LIGHT_FLAG;
                    MUtils.println("Adjusting brightness of kitchen light");
                }
            }else{
                if(ui8KitchenLightValue < 100){
                    ui8KitchenLightValue = ui8KitchenLightValue + ALEXA_BRIGHTNESS_LEVEL;
                    ui8ChangeValueFlag = KITCHEN_LIGHT_FLAG;
                    MUtils.println("Adjusting brightness of kitchen light");
                }
            }
        }else{
            MUtils.println("No device specified in code");
        }
    }else{
        // do nothing
    }

    handleSerialDimmer();
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length){
    switch(type) {
        case WStype_DISCONNECTED:
            ui8IsConnected = false;
            MUtils.println("[WSc] Webservice disconnected from sinric.com!\n");
            break;
        case WStype_CONNECTED:
            ui8IsConnected = true;
            MUtils.println("[WSc] Service connected to sinric.com at url: " );
            //MUtils.println(String(payload).);
            MUtils.println("Waiting for commands from sinric.com ...\n");
            break;
        //case WStype_ERROR:
        //break;
        //case WStype_FRAGMENT_TEXT_START:
        ///break;
        //case WStype_FRAGMENT_BIN_START:
        //break;
        //case WStype_FRAGMENT:
        //break;
        //case WStype_FRAGMENT_FIN:
        //break;
        case WStype_TEXT:
            //Action_Selector(payload);
            MUtils.println("Data received...");
            MApp.HandleAlexa(payload);
            break;
        case WStype_BIN:
            MUtils.print("[WSc] get binary length: ");
            MUtils.println(length);
            break;
    }
}

void METIAlexa::Start(void){

    Update_All_Key_And_ID();

    // server address, port and URL
    webSocket.begin("iot.sinric.com", 80, "/");
    // event handler
    webSocket.onEvent(webSocketEvent);
    //webSocket.setAuthorization("apikey", MyApiKey);
    
    webSocket.setAuthorization("apikey", sAlexaAPIKey.c_str());
    
    // try again every 5000ms if connection has failed
    webSocket.setReconnectInterval(5000);
}

void METIAlexa::handleRequest(void){
    webSocket.loop();
    if(ui8IsConnected) {
        uint64_t now = millis();
        // Send heartbeat in order to avoid disconnections during ISP resetting IPs over night. Thanks @MacSass
        if((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL) {
            heartbeatTimestamp = now;
            webSocket.sendTXT("H");
        }
    }
}

void METIAlexa::Update_Specified_File(uint8_t ui8ID){
    String sString;

    sString = MFile.Read_Data_From_File(sMETIFiles[ui8ID]);
    MUtils.println(sString);

    if(ui8ID == ALEXA_TOKEN_INDEX){
        sAlexaAPIKey = sString;
    }else if(ui8ID == ALEXA_AC_1_ID_INDEX){
        sAlexaACInput1ID = sString;
    }else if(ui8ID == ALEXA_AC_2_ID_INDEX){
        sAlexaACInput2ID = sString;
    }else if(ui8ID == ALEXA_AC_3_ID_INDEX){
        sAlexaACInput3ID = sString;
    }else if(ui8ID == ALEXA_AC_4_ID_INDEX){
        sAlexaACInput4ID = sString;
    }else if(ui8ID == ALEXA_AC_1_NAME_INDEX){
        sAlexaACInput1Name = sString;
    }else if(ui8ID == ALEXA_AC_2_NAME_INDEX){
        sAlexaACInput2Name = sString;
    }else if(ui8ID == ALEXA_AC_3_NAME_INDEX){
        sAlexaACInput3Name = sString;
    }else if(ui8ID == ALEXA_AC_4_NAME_INDEX){
        sAlexaACInput4Name = sString;
    }else{
        MUtils.println("file error");
    }
}

void METIAlexa::Update_All_Key_And_ID(void){
    
    sAlexaAPIKey = MFile.Read_Data_From_File(sMETIFiles[ALEXA_TOKEN_INDEX]);
    MUtils.print("API Key:");
    MUtils.println(sAlexaAPIKey);
    

    sAlexaACInput1ID = MFile.Read_Data_From_File(sMETIFiles[ALEXA_AC_1_ID_INDEX]);
    MUtils.print("ACin1:");    
    MUtils.println(sAlexaACInput1ID);


    sAlexaACInput2ID = MFile.Read_Data_From_File(sMETIFiles[ALEXA_AC_2_ID_INDEX]);
    MUtils.print("ACin2:"); 
    MUtils.println(sAlexaACInput2ID);


    sAlexaACInput3ID = MFile.Read_Data_From_File(sMETIFiles[ALEXA_AC_3_ID_INDEX]);
    MUtils.print("ACin3:"); 
    MUtils.println(sAlexaACInput3ID);


    sAlexaACInput4ID = MFile.Read_Data_From_File(sMETIFiles[ALEXA_AC_4_ID_INDEX]);
    MUtils.print("ACin4:"); 
    MUtils.println(sAlexaACInput4ID);


    sAlexaACInput1Name = MFile.Read_Data_From_File(sMETIFiles[ALEXA_AC_1_NAME_INDEX]);
    MUtils.print("ACName1:"); 
    MUtils.println(sAlexaACInput1Name);


    sAlexaACInput2Name = MFile.Read_Data_From_File(sMETIFiles[ALEXA_AC_2_NAME_INDEX]);
    MUtils.print("ACName2:");
    MUtils.println(sAlexaACInput2Name);


    sAlexaACInput3Name = MFile.Read_Data_From_File(sMETIFiles[ALEXA_AC_3_NAME_INDEX]);
    MUtils.print("ACName3:");
    MUtils.println(sAlexaACInput3Name);


    sAlexaACInput4Name = MFile.Read_Data_From_File(sMETIFiles[ALEXA_AC_4_NAME_INDEX]);
    MUtils.print("ACName4:");
    MUtils.println(sAlexaACInput4Name);

}

String METIAlexa::Get_Value_Of_File(uint8_t ui8Index){
    String sString;

    if(ui8Index == ALEXA_TOKEN_INDEX){
        sString= sAlexaAPIKey;
    }else if(ui8Index == ALEXA_AC_1_ID_INDEX){
        sString= sAlexaACInput1ID;
    }else if(ui8Index == ALEXA_AC_2_ID_INDEX){
        sString= sAlexaACInput2ID;
    }else if(ui8Index == ALEXA_AC_3_ID_INDEX){
        sString= sAlexaACInput3ID;
    }else if(ui8Index == ALEXA_AC_4_ID_INDEX){
        sString= sAlexaACInput4ID;
    }else if(ui8Index == ALEXA_AC_1_NAME_INDEX){
        sString= sAlexaACInput1Name;
    }else if(ui8Index == ALEXA_AC_2_NAME_INDEX){
        sString= sAlexaACInput2Name;
    }else if(ui8Index == ALEXA_AC_3_NAME_INDEX){
        sString= sAlexaACInput3Name;
    }else if(ui8Index == ALEXA_AC_4_NAME_INDEX){
        sString= sAlexaACInput4Name;
    }else{
        sString= "  ";
    }

    return sString;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
METIAlexa MAlexa;
#endif
