/*******************************************************************************
* File Name: WiFiControl.h
*
* Version: 1.0
*
* Description:
* This is header file for wifi modbus. All the functions related to
* wifi are here.
*
* Owner:
* Yogesh M Iggalore
*
* Code Tested With:
* 1. platformio and esp32
*
********************************************************************************
* Copyright (2021-22) ,
********************************************************************************/
#ifndef WiFiControl_h
#define WiFiControl_h
#include <Arduino.h>
#include <WiFiMulti.h>

#define ENABLE_WIFI

#define WIFI_STATE_AP_MODE          0
#define WIFI_STATE_NOT_CONNECTED    1
#define WIFI_STATE_CONNECTED        2

typedef struct{
    bool bEnableStatic;
    bool bIPEnable;
    bool bGatewayEnable;
    bool bSubnetEnable;
    bool bDNSEnable;
    IPAddress IPDeviceAddr;
    IPAddress IPGateway;
    IPAddress IPSubnet;
    IPAddress IPDNS; 
}WIFI_STATIC_CONFIG;

typedef struct{
    String  sName;
    String  sPass;
    WIFI_STATIC_CONFIG tdfWifiStatic;     
}WIFI_CONFIG;

typedef struct{
    bool bWifiFileReadSucess;
    uint8_t ui8StationStatus;
    bool bChangeInWifi;
    bool bWiFiFlag=0;
    bool bWiFiGotIP=0;
    uint8_t ui8WiFiStatus=0;
    uint8_t ui8Mode=0;
    WIFI_CONFIG tdfWifi[4];
    String sHostName;
    String sHostPassword;
    String sFixName;
    String sFixPass;
    String sLastWifi;
    WIFI_CONFIG tdfConnectedWifi;
}WIFI_PARAMETER;

class WiFiControl{
    public:
		WIFI_PARAMETER tdfwifi;
        WiFiControl();
		void start(void);
        void read_parameter_from_file(void);
        void connect_to_wifi_in_ap_mode(void);
        void connect_to_wifi_in_station_mode(void);
        void print_wifi_parameters(void);
	private:
		WiFiMulti wifiMulti;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_WIFICONTROL)
extern WiFiControl ewifi;
#endif
#endif