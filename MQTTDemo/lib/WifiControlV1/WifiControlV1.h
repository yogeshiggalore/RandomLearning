/*******************************************************************************
*File Name: WifiControlV1.h
*
* Version: 1.0
*
* Description:
* In this header used for wifi connection related functions functions
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/
#ifndef WifiControlV1_h
#define WifiControlV1_h
#include <Arduino.h>
#include <ESP8266WiFi.h>

#define BOOT_WIFI_COUNT_MAX 180

#define WIFI_USE_CONFIG_PARA_INDEX  0
#define WIFI_USE_IP_ADDR_INDEX      1
#define WIFI_USE_GATWAY_ADDR_INDEX  2
#define WIFI_USE_SUBNET_ADDR_INDEX  3
#define WIFI_USE_DNS_ADDR_INDEX     4

class WifiControlV1{
public:
  uint8_t  ui8WifiStatus=0;
  uint16_t ui16BootWifiCounter=0;
  uint8_t  ui8WifiStartFlag=0;
  uint8_t  aui8WifiConfigStatus[16];
  uint8_t  aui8WifiConfigParameters[16];

  IPAddress MWifiIPAddr;
  IPAddress MWifiGateway;
  IPAddress MWifiSubnet;
  IPAddress MWifiDNS;

  IPAddress CWifiIPAddr;
  IPAddress CWifiGateway;
  IPAddress CWifiSubnet;
  IPAddress CWifiDNS;

  String MSSID;
  String MPassword;
  String CSSID;
  String CPassword;

  WifiControlV1();
  void Start(void);
  String Checkout_Connection(void);
  uint8_t Get_Status(void);
  void Set_Status(uint8_t ui8Status);
  uint8_t Check_Wifi_Parameters(void);
  void Update_Wifi_Parameters(void);
  void Update_Wifi_ssid_password(void);
  void Test_WifiConfigParameters(void);
  void Test_WifiConfigStatus(void);
};
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
extern WifiControlV1 MWifiControl;
#endif
#endif
