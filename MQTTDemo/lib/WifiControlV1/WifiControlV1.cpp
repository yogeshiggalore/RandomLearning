/*******************************************************************************
*File Name: WifiControlV1.c
*
* Version: 1.0
*
* Description:
* In this source code for wifi connection related functions
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/
#include <WifiControlV1.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <METIUtils.h>
#include <METIMemory.h>
#include <METIUtils.h>

// Constructors ////////////////////////////////////////////////////////////////
WifiControlV1::WifiControlV1(){}

void WifiControlV1::Start(void){
  uint8_t ui8WifiCounter=0;

  MSSID = MMemory.Read_SSID();
  MPassword = MMemory.Read_Password();


  if(MSSID != ""){
        if(MPassword != ""){
          if(Check_Wifi_Parameters()){
            Serial1.println("wifi parameters configured ");
            WiFi.config(MWifiIPAddr,MWifiGateway,MWifiSubnet,MWifiDNS);
          }
            WiFi.begin(MSSID, MPassword);
            for(ui8WifiCounter=0;ui8WifiCounter<180;ui8WifiCounter++){
              if(WiFi.status() != WL_CONNECTED){
                delay(100);
                //digitalWrite(D6,HIGH);
                delay(100);
                //digitalWrite(D6,LOW);
              }else{
                ui8WifiCounter = 181;
                Serial1.print("Connected to wifi ");
                Serial1.println(MSSID);
                CWifiIPAddr = WiFi.localIP();
                CWifiGateway = WiFi.gatewayIP();
                CWifiSubnet = WiFi.subnetMask();
                CWifiDNS    = WiFi.dnsIP();
                CSSID = WiFi.SSID();
                Update_Wifi_Parameters();
              }
            }
        }else{
            ui8WifiStatus = 0xFF;
            Serial1.println("No wifi configured in memory");
        }
  }else{
        ui8WifiStatus = 0xFF;
        Serial1.println("No password configured in memory");
  }

}

uint8_t WifiControlV1::Get_Status(void){
  return ui8WifiStatus;
}

void WifiControlV1::Set_Status(uint8_t ui8Status){
  ui8WifiStatus = ui8Status;

}

String WifiControlV1::Checkout_Connection(void){
  Set_Status(WiFi.status());
  String sString;
  if(Get_Status() != WL_CONNECTED){
    if(Get_Status() == WL_NO_SHIELD){
      sString = "\r\nwifi error:WL_NO_SHIELD";
    }else if(Get_Status() == WL_IDLE_STATUS){
      sString = "\r\nwifi error:WL_IDLE_STATUS";
    }else if(Get_Status() == WL_NO_SSID_AVAIL){
      sString = "\r\nwifi error:WL_NO_SSID_AVAIL";
    }else if(Get_Status() == WL_SCAN_COMPLETED){
      sString = "\r\nwifi error:WL_SCAN_COMPLETED";
    }else if(Get_Status() == WL_CONNECT_FAILED){
      sString = "\r\nwifi error:WL_CONNECT_FAILED";
    }else if(Get_Status() == WL_CONNECTION_LOST){
      sString = "\r\nwifi error:WL_CONNECTION_LOST";
    }else if(Get_Status() == WL_DISCONNECTED){
      sString = "\r\nwifi error:WL_DISCONNECTED";
    }
  }else{
    sString = "\r\nConnected to wifi ";
  }

  if(ui8WifiStartFlag == 0){
    ui16BootWifiCounter++;
    if(ui16BootWifiCounter > BOOT_WIFI_COUNT_MAX){
      ui16BootWifiCounter = 0;
      ui8WifiStartFlag = 1;
    }
  }
  return sString;
}

uint8_t WifiControlV1::Check_Wifi_Parameters(void){

  MMemory.Read_Wifi_Config_Status(aui8WifiConfigStatus);
  MMemory.Read_Wifi_Config_Parameters(aui8WifiConfigParameters);

  if(aui8WifiConfigStatus[WIFI_USE_CONFIG_PARA_INDEX]== 1){
    if(aui8WifiConfigStatus[WIFI_USE_IP_ADDR_INDEX]){
      MWifiIPAddr[0] = aui8WifiConfigParameters[0];
      MWifiIPAddr[1] = aui8WifiConfigParameters[1];
      MWifiIPAddr[2] = aui8WifiConfigParameters[2];
      MWifiIPAddr[3] = aui8WifiConfigParameters[3];
    }

    if(aui8WifiConfigStatus[WIFI_USE_GATWAY_ADDR_INDEX]){
      MWifiGateway[0] = aui8WifiConfigParameters[4];
      MWifiGateway[1] = aui8WifiConfigParameters[5];
      MWifiGateway[2] = aui8WifiConfigParameters[6];
      MWifiGateway[3] = aui8WifiConfigParameters[7];
    }

    if(aui8WifiConfigStatus[WIFI_USE_SUBNET_ADDR_INDEX]){
      MWifiSubnet[0] = aui8WifiConfigParameters[8];
      MWifiSubnet[1] = aui8WifiConfigParameters[9];
      MWifiSubnet[2] = aui8WifiConfigParameters[10];
      MWifiSubnet[3] = aui8WifiConfigParameters[11];
    }

    if(aui8WifiConfigStatus[WIFI_USE_DNS_ADDR_INDEX]){
      MWifiDNS[0] = aui8WifiConfigParameters[12];
      MWifiDNS[1] = aui8WifiConfigParameters[13];
      MWifiDNS[2] = aui8WifiConfigParameters[14];
      MWifiDNS[3] = aui8WifiConfigParameters[15];
    }

    return 1;
  }

  return 0;
}

void WifiControlV1::Update_Wifi_Parameters(void){

  MMemory.Read_Wifi_Config_Status(aui8WifiConfigStatus);
  MMemory.Read_Wifi_Config_Parameters(aui8WifiConfigParameters);

  MWifiIPAddr[0] = aui8WifiConfigParameters[0];
  MWifiIPAddr[1] = aui8WifiConfigParameters[1];
  MWifiIPAddr[2] = aui8WifiConfigParameters[2];
  MWifiIPAddr[3] = aui8WifiConfigParameters[3];
  
  MWifiGateway[0] = aui8WifiConfigParameters[4];
  MWifiGateway[1] = aui8WifiConfigParameters[5];
  MWifiGateway[2] = aui8WifiConfigParameters[6];
  MWifiGateway[3] = aui8WifiConfigParameters[7];

  MWifiSubnet[0] = aui8WifiConfigParameters[8];
  MWifiSubnet[1] = aui8WifiConfigParameters[9];
  MWifiSubnet[2] = aui8WifiConfigParameters[10];
  MWifiSubnet[3] = aui8WifiConfigParameters[11];

  MWifiDNS[0] = aui8WifiConfigParameters[12];
  MWifiDNS[1] = aui8WifiConfigParameters[13];
  MWifiDNS[2] = aui8WifiConfigParameters[14];
  MWifiDNS[3] = aui8WifiConfigParameters[15];

}

void WifiControlV1::Update_Wifi_ssid_password(void){
  MSSID = MMemory.Read_SSID();
  MPassword = MMemory.Read_Password();
}

void WifiControlV1::Test_WifiConfigParameters(void){
  uint8_t ui8LoopCounter;

  MMemory.Read_Wifi_Config_Parameters(aui8WifiConfigParameters);
  Serial1.println("aui8WifiConfigParameters");
  for(ui8LoopCounter=0;ui8LoopCounter<16;ui8LoopCounter++){
    Serial1.print(aui8WifiConfigParameters[ui8LoopCounter]);
    Serial1.print(" ");
  } 
  
  Serial1.println("");
}

void WifiControlV1::Test_WifiConfigStatus(void){
  uint8_t ui8LoopCounter;

  MMemory.Read_Wifi_Config_Status(aui8WifiConfigStatus);
  Serial1.println("aui8WifiConfigStatus");
  for(ui8LoopCounter=0;ui8LoopCounter<16;ui8LoopCounter++){
    Serial1.print(aui8WifiConfigStatus[ui8LoopCounter]);
    Serial1.print(" ");
  } 
  Serial1.println("");
}

// Preinstantiate Objects //////////////////////////////////////////////////////

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
WifiControlV1 MWifiControl;
#endif
