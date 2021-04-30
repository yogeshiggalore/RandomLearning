/*******************************************************************************
*File Name: WiFiControl.cpp
*
* Version: 1.0
*
* Description:
* In this source file for wifi application
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2021-22)
*******************************************************************************/

#include<Arduino.h>
#include<WiFiControl.h>
#include<ESPGPIO.h>
#include<ArduinoJson.h>
#include<SPIFFS.h>
#include<FS.h>
#include<LogLevel.h>
#include<logger.h>
#include<ESPUtils.h>

// Constructors ////////////////////////////////////////////////////////////////
WiFiControl::WiFiControl(void):wifiMulti(){}

static void outsideClassHandler(WiFiEvent_t event){
   switch (event){
		case SYSTEM_EVENT_STA_CONNECTED:
			eio.led_set_mode(LED_WIFI,LED_MODE_BLINK); 
			ewifi.tdfwifi.bWiFiGotIP = false;
			elog.Write(LogLevel::Info, "wifi", "Device WiFi got connected to external Router");
			break;
		case SYSTEM_EVENT_STA_DISCONNECTED:
			eio.led_set_mode(LED_WIFI,LED_MODE_BLINK); 
			ewifi.tdfwifi.bWiFiGotIP = false;
			elog.Write(LogLevel::Warn, "wifi", "Device lost WiFi connection");
			WiFi.reconnect();
			break;
		case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
			elog.Write(LogLevel::Info, "wifi", "Auth mode of exernal Router changed");
			break;
		case SYSTEM_EVENT_STA_GOT_IP:
			eio.led_set_mode(LED_WIFI,LED_MODE_ON); 
			ewifi.tdfwifi.bWiFiGotIP = true;
			elog.Write(LogLevel::Info, "wifi", "Device WiFi got IP from external Router : %s", WiFi.localIP().toString().c_str());
			break;
		case SYSTEM_EVENT_AP_STACONNECTED:
			elog.Write(LogLevel::Info, "wifi", "External device connected to this Access point");
			break;
		case SYSTEM_EVENT_AP_STADISCONNECTED:
			elog.Write(LogLevel::Info, "wifi", "External device disconnected from this Access point");
			break;
		case SYSTEM_EVENT_SCAN_DONE:
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_SCAN_DONE");
			break;
		case SYSTEM_EVENT_MAX:
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_MAX");
			break;
		case SYSTEM_EVENT_WIFI_READY:
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_WIFI_READY");
			break;           
		case SYSTEM_EVENT_STA_START:
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_STA_START");
			break;
		case SYSTEM_EVENT_STA_STOP:            
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_STA_STOP");
			break;
		case SYSTEM_EVENT_STA_LOST_IP:
			eio.led_set_mode(LED_WIFI,LED_MODE_ON); 
			ewifi.tdfwifi.bWiFiGotIP = false;         
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_STA_LOST_IP");
			break;
		case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_STA_WPS_ER_SUCCESS");
			break;
		case SYSTEM_EVENT_STA_WPS_ER_FAILED:
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_STA_WPS_ER_FAILED");
			break;
		case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_STA_WPS_ER_TIMEOUT");
			break;
		case SYSTEM_EVENT_STA_WPS_ER_PIN:
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_STA_WPS_ER_PIN");
			break;
		case SYSTEM_EVENT_STA_WPS_ER_PBC_OVERLAP:
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_STA_WPS_ER_PBC_OVERLAP");
			break;
		case SYSTEM_EVENT_AP_START:
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_AP_START");
			break;
		case SYSTEM_EVENT_AP_STOP:                
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_AP_STOP");
			break;
		case SYSTEM_EVENT_AP_STAIPASSIGNED:       
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_AP_STAIPASSIGNED");
			break;
		case SYSTEM_EVENT_AP_PROBEREQRECVED:      
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_AP_PROBEREQRECVED");
			break;
		case SYSTEM_EVENT_GOT_IP6:                
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_GOT_IP6");
			break;
		case SYSTEM_EVENT_ETH_START:              
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_ETH_START");
			break;
		case SYSTEM_EVENT_ETH_STOP:               
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_ETH_STOP");
			break;
		case SYSTEM_EVENT_ETH_CONNECTED:          
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_ETH_CONNECTED");
			break;
		case SYSTEM_EVENT_ETH_DISCONNECTED:       
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_ETH_DISCONNECTED");
			break;
		case SYSTEM_EVENT_ETH_GOT_IP:
			elog.Write(LogLevel::Info, "wifi", "SYSTEM_EVENT_ETH_GOT_IP");
			break;
	}
}

void WiFiControl::start(void){
	eio.led_set_mode(LED_WIFI, LED_MODE_FAST_BLINK);
	read_parameter_from_file();
	print_wifi_parameters();
	
    WiFi.disconnect();
    delay(1000);

    WiFi.onEvent(outsideClassHandler);
	
	//connect_to_wifi_in_ap_mode();
	connect_to_wifi_in_station_mode();
}

void WiFiControl::connect_to_wifi_in_ap_mode(void){
	String sHost;
	String sPass;

	sHost = tdfwifi.sHostName;
	sPass = tdfwifi.sHostPassword;

	if(sHost.equals("NOWIFI")){
		sHost = EUtils.Get_Device_Name();
	}

	WiFi.mode(WIFI_AP);


	WiFi.softAP(sHost.c_str(), sPass.c_str());

    /* Important after softAPConfig, a delay is required, else system will crash */
    delay(50);

    WiFi.softAPConfig({192,168,4,2}, {192,168,4,1}, {255,255,255,0});

    elog.Write(LogLevel::Info, "wifi", "Setting up AP mode : %s", WiFi.softAPIP().toString().c_str());

    delay(10);
}

void WiFiControl::connect_to_wifi_in_station_mode(void){
	WiFi.mode(WIFI_STA);

	wifiMulti.addAP(tdfwifi.tdfWifi[0].sName.c_str(), tdfwifi.tdfWifi[0].sPass.c_str());
    wifiMulti.addAP(tdfwifi.tdfWifi[1].sName.c_str(), tdfwifi.tdfWifi[1].sPass.c_str());
    wifiMulti.addAP(tdfwifi.tdfWifi[2].sName.c_str(), tdfwifi.tdfWifi[2].sPass.c_str());
    wifiMulti.addAP(tdfwifi.tdfWifi[3].sName.c_str(), tdfwifi.tdfWifi[3].sPass.c_str());
	wifiMulti.addAP(tdfwifi.sFixName.c_str(), tdfwifi.sFixPass.c_str());

	wifiMulti.run();

}
	            
void WiFiControl::read_parameter_from_file(void){
	const char* word;
    
    StaticJsonDocument<2048> doc;
    JsonObject jWifi;
    JsonObject jWifiStatic;
    JsonObject jIP;
    uint8_t ui8LoopCounter=0;

    File fFile = SPIFFS.open("/wificonfig.json","r");
    if (!fFile) {
        elog.Write(LogLevel::Error,"wifi","Failed to open config file");
		tdfwifi.bWifiFileReadSucess = 0;
    }else{
        deserializeJson(doc, fFile);
        tdfwifi.bWifiFileReadSucess = 1;
        word = doc["Host"];
        tdfwifi.sHostName = word;
        word = doc["HostPass"];
        tdfwifi.sHostPassword = word;
        word = doc["FixName"];
        tdfwifi.sFixName = word;
        word = doc["FixPass"];
        tdfwifi.sFixPass = word;

        for(ui8LoopCounter=0;ui8LoopCounter<4;ui8LoopCounter++){
            
            if(ui8LoopCounter == 0){
                jWifi = doc["W1"];
            }else if(ui8LoopCounter == 1){
                jWifi = doc["W2"];
            }else if(ui8LoopCounter == 2){
                jWifi = doc["W3"];
            }else if(ui8LoopCounter == 3){
                jWifi = doc["W4"];
            }else{
                // do nothing devare kapadbeku....
            }

            jWifiStatic = jWifi["S"];

            word = jWifi["N"];
            tdfwifi.tdfWifi[ui8LoopCounter].sName = word;

            word = jWifi["P"];
            tdfwifi.tdfWifi[ui8LoopCounter].sPass = word;
        
            tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.bEnableStatic = jWifiStatic["E"];
        
            jIP  = jWifiStatic["IP"];
            tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.bIPEnable = jIP["E"];
            word = jIP["V"];
            tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.IPDeviceAddr.fromString(word);
        
            jIP  = jWifiStatic["Gat"];
            tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.bGatewayEnable = jIP["E"];
            word = jIP["V"];
            tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.IPGateway.fromString(word);

            jIP  = jWifiStatic["Sub"];
            tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.bSubnetEnable = jIP["E"];
            word = jIP["V"];
            tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.IPSubnet.fromString(word);

            jIP  = jWifiStatic["DNS"];
            tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.bDNSEnable = jIP["E"];
            word = jIP["V"];
            tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.IPDNS.fromString(word);

			elog.Write(LogLevel::Info,"wifi","file read sucess ");
		}
    }

    fFile.close();
}

void WiFiControl::print_wifi_parameters(void){
	uint8_t ui8LoopCounter=0;
	String sData;


	for(ui8LoopCounter=0;ui8LoopCounter<4;ui8LoopCounter++){
		sData = "Wifi " + String(ui8LoopCounter) + " parameters ";
		sData +=tdfwifi.tdfWifi[ui8LoopCounter].sName + " ";
    	sData +=tdfwifi.tdfWifi[ui8LoopCounter].sPass + " ";
    	sData +=String(tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.bEnableStatic) + " ";
    	sData +=String(tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.bIPEnable) + " ";
    	sData +=tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.IPDeviceAddr.toString() + " ";
    	sData +=String(tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.bGatewayEnable) + " ";
    	sData +=tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.IPGateway.toString() + " ";
    	sData +=String(tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.bSubnetEnable) + " ";
    	sData +=tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.IPSubnet.toString()+ " ";
    	sData +=String(tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.bDNSEnable) + " ";
    	sData +=tdfwifi.tdfWifi[ui8LoopCounter].tdfWifiStatic.IPDNS.toString()+ " ";

		elog.Write(LogLevel::Trace,"wifi",sData.c_str());
	}
	
	sData = "Host:" + tdfwifi.sHostName + " HostPass:" + tdfwifi.sHostPassword + " FixName:" + tdfwifi.sFixName + " FixPass:" + tdfwifi.sFixPass;

	if(tdfwifi.ui8Mode == 0){
		sData += " mode: AP"; 
	}else if(tdfwifi.ui8Mode == 1){
		sData += " mode: Station"; 
	}

	elog.Write(LogLevel::Trace,"wifi",sData.c_str());
}

// Preinstantiate Objects //////////////////////////////////////////////////////
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_WIFICONTROL)
WiFiControl ewifi;
#endif