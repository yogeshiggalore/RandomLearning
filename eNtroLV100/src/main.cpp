/*******************************************************************************
* Project Name: eNtrolV100
*
* Version: 1.0
*
* Description:
* In this project eNtroL, energy meter puls mqtt example
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2020-21) , eNtroL
*******************************************************************************/

/* include headers */
#include <Arduino.h>
#include <ESPUtils.h>
#include <ModbusMaster.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <ModbusUtils.h>
#include <EnergyMeter.h>
#include <ESPFile.h>
#include <MqttFile.h>
#include <AsyncMqttClient.h>
#include <WifiControlV2.h>
#include <TaskScheduler.h>
#include <Application.h>
#include <ESPMemory.h>
#include <ESPAlexaV2.h>
#include <Thingspeak.h>

uint8_t ui8MemoryLED1Value=0;

/* led web page index */
const char* htmlfile = "/index.html";

/* Static i config for server */
IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,0);
IPAddress subnet(255,255,255,0);

/* webserver object on port 80 */
ESP8266WebServer ESPServer(80);

/* ESP Server functions*/
void ESPServer_HandleWebRequests(void);
void ESPServer_EntrolUpdate(void);
void ESPServer_HandleRoot(void);
void ESPServer_Handle_MqttParameter(void);
void ESPServer_Handle_WifiAdd(void);
void ESPServer_Handle_WifiHostname(void);
void ESPServer_Handle_WifiParameterAdd(void);
void ESPServer_Handle_TSParameter(void);
void ESPServer_Get_TSValues(void);
void ESPServer_RelayUpdate(void);
bool ESPServer_LoadFromSpiffs(String path);
void ESPServer_Handle_DeviceReset(void);
void ESPServer_Mqtt_Values(void);
void ESPServer_Wifi_Values(void);
void ESPServer_Alexa_Values(void);
void ESPServer_Handle_Alexa(void);

/* timer and intruppt function for 1 milisecond */
void Timer_1ms_Start(void);
void ICACHE_RAM_ATTR timercallback1ms(void);

/* callbacks */
void TestCallback(void);
void WifiControlV1Callback(void);
void InternetCallback(void);
void MqttCallback(void);
void ThingspeakCallback(void);

/* Mqtt functions */
void onMqttConnect(bool sessionPresent);
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
void onMqttSubscribe(uint16_t packetId, uint8_t qos);
void onMqttUnsubscribe(uint16_t packetId);
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
void onMqttPublish(uint16_t packetId);
void connectToMqtt(void);
void MQTT_Handle(void);
void Mqtt_Publish_All(void);

Task TestTask(1000, TASK_FOREVER, &TestCallback);
Task WifiControlTask(1000, TASK_FOREVER, &WifiControlV1Callback);
Task MqttControlTask(60000, TASK_FOREVER, &MqttCallback);
Task MqttReconnectTask(2000,TASK_ONCE,&connectToMqtt);
Task ThingspeakTask(600000, TASK_FOREVER, &ThingspeakCallback);
Scheduler TaskRunner;

/* timer flags */
uint8_t  ui81msFlag=0;
uint64_t ui64OnemsTimeCounter=0;
long lLastTime=0;
uint8_t ui8DeviceResetFlag=0;

// MQTT 
AsyncMqttClient mqttClient;
uint8_t ui8MqttSendFlag;
IPAddress IPAddr;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

void setup() {
    
    delay(1000);

    Serial1.begin(921600);
    Serial1.println("eNtroLV100");
    EUtils.Enable_Serial_Debug();
    
	EMemory.Init();
	EApp.Start();

	Modbus.Start();

    /* Initialize SPIFFS */
	if(!SPIFFS.begin()){
		Serial1.println("An Error has occurred while mounting SPIFFS");
		while(1);
	}

    /* start file system and read all files */
    EFile.Start();
	MqFile.Read_All_Mqtt_Parameters();

    Serial1.println("Connecting to wifi");

    EWifiControl.Start();
    IPAddr = WiFi.localIP();
    Serial1.println(IPAddr);

	/* Enable ESP Server function and start webserver */
    ESPServer.on("/",ESPServer_HandleRoot);
    ESPServer.on("/entrolupdate",ESPServer_EntrolUpdate);
    ESPServer.on("/relayupdate",ESPServer_RelayUpdate);
    ESPServer.on ("/addmqtt", ESPServer_Handle_MqttParameter);
    ESPServer.on ("/addwifi", ESPServer_Handle_WifiAdd);
    ESPServer.on ("/addwifihostname", ESPServer_Handle_WifiHostname);
    ESPServer.on ("/addwifiparameter", ESPServer_Handle_WifiParameterAdd);
    ESPServer.on ("/addtsparameter", ESPServer_Handle_TSParameter);
    ESPServer.on ("/gettsvalues",ESPServer_Get_TSValues);
	ESPServer.on ("/reset", ESPServer_Handle_DeviceReset);
    ESPServer.on ("/getmqttvalues", ESPServer_Mqtt_Values);
    ESPServer.on ("/getwifivalues",ESPServer_Wifi_Values);
    ESPServer.on ("/getalexavalues",ESPServer_Alexa_Values);
    ESPServer.on("/addalexa",ESPServer_Handle_Alexa);
    ESPServer.onNotFound(ESPServer_HandleWebRequests);
    ESPServer.begin();

    /* start soft access point connections */
	EUtils.print("Setting soft-AP configuration ... ");
    EUtils.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
    EUtils.print("Setting soft-AP ... ");
    String sAPName;
    String sAPPassword;
    if(EWifiControl.sHostName != "NOWIFI"){
        sAPName = "EN01" + EWifiControl.sHostName;
    }else{
        sAPName = "EN01" + EUtils.Get_MAC();
    }

    sAPPassword = EWifiControl.sHostPassword;
    EUtils.print("APName: ");
    EUtils.print(sAPName);
    EUtils.print("  APPassword: ");
    EUtils.print(sAPPassword);
    EUtils.print("  Status: ");
    EUtils.println(WiFi.softAP(sAPName,sAPPassword) ? "Ready" : "Failed!");
    EUtils.print("Soft-AP IP address = ");
    EUtils.println(WiFi.softAPIP());

    TaskRunner.init();
    EUtils.println("Initialized scheduler");

    TaskRunner.addTask(TestTask);
    EUtils.println("added TestTask");
    TestTask.enable();
    EUtils.println("Enabled TestTask");

    TaskRunner.addTask(WifiControlTask);
    EUtils.println("added WifiControlTask");
    WifiControlTask.enable();
    EUtils.println("Enabled WifiControlTask");

    TaskRunner.addTask(MqttControlTask);
    EUtils.println("added MqttControlTask");
    MqttControlTask.enable();
    EUtils.println("Enabled MqttControlTask");

    TaskRunner.addTask(ThingspeakTask);
    EUtils.println("added ThingspeakTask");
    ThingspeakTask.enable();
    EUtils.println("Enabled ThingspeakTask");

    TaskRunner.addTask(MqttReconnectTask);

	/* start timer module */
	noInterrupts();
    Timer_1ms_Start();
    interrupts();

    lLastTime = millis();

    mqttClient.onConnect(onMqttConnect);
    mqttClient.onDisconnect(onMqttDisconnect);
    mqttClient.onSubscribe(onMqttSubscribe);
    mqttClient.onUnsubscribe(onMqttUnsubscribe);
    mqttClient.onMessage(onMqttMessage);
    mqttClient.onPublish(onMqttPublish);
    connectToMqtt();

    MAlexa.Start();
    TS.Start();
}

void loop() {

    Modbus.Update();

    /* function to handle clients */
	ESPServer.handleClient();

    /* task manager */
    TaskRunner.execute();

    MAlexa.handleRequest();

	if(ui8DeviceResetFlag){
        delay(2000);
        ESP.restart();
    }
    
    yield();
	
}

/* This function sets 1 milisecond timer */
void Timer_1ms_Start(void){
    timer1_isr_init();
    timer1_attachInterrupt(timercallback1ms);
    timer1_enable(TIM_DIV1, TIM_EDGE, TIM_LOOP); //80MHZ
    timer1_write(80000);//1ms
}

/* callback function 1ms interrupt */
void ICACHE_RAM_ATTR timercallback1ms(void){
    ui81msFlag=1;
    ui64OnemsTimeCounter++;
	
	/* call modbus timer function */
	Modbus.Check_Modbus_Timer();
}

void TestCallback(void) {
    //Serial1.println("Working");
}   

void ThingspeakCallback (void) {
    TS.Update_Energy();
}

void WifiControlV1Callback(void){
    String sString;
    
    //Mqtt_Check_For_Alexa_Sync();
    sString = EWifiControl.Checkout_Connection();
    if(EWifiControl.Get_Status() != WL_CONNECTED){
       Serial1.println("no wifi");
    }
}

void InternetCallback(void){
    //EHttpClient.Check_ESP_Default_Ping(1000);
}

void MqttCallback (void){
    MQTT_Handle();
}

void ESPServer_Mqtt_Values(void){
    String sData;
    StaticJsonDocument<1048> doc;

    doc["server"] = MqFile.sMqttServer; 
    doc["port"]   = String(MqFile.ui16MqttPort);
    doc["id"] = MqFile.sMqttClientId;
    doc["username"] = MqFile.sMqttUsername;
    doc["pass"] = MqFile.sMqttPassword;
    doc["keep"] = String(MqFile.ui16MqttKeepalive);
    doc["qos"] = String(MqFile.ui8MqttQoS);
    doc["relayP"] = MqFile.sMqttPubRelayTopic;
    doc["relayS"] = MqFile.sMqttSubRelayTopic;
    doc["unitP"] = MqFile.sMqttPubUnitTopic;
    doc["CurrP"] = MqFile.sMqttPubCurrentTopic;
    doc["voltP"] = MqFile.sMqttPubVoltageTopic;
    doc["pfP"] = MqFile.sMqttPubpowerfactorTopic;
    doc["pwrP"] = MqFile.sMqttPubPowerTopic;
    doc["frqP"] = MqFile.sMqttPubFrequencyTopic;
    doc["get"] = MqFile.sMqttPubAllTopic;

    serializeJson(doc, sData);  

    ESPServer.send(200, "text/json",sData);
}   

void ESPServer_Alexa_Values(void){
    String sData;
    StaticJsonDocument<1048> doc;

    doc["token"] = MAlexa.sAlexaAPIKey;
    doc["id"]   = MAlexa.sEntrolRelayId;
    doc["name"] = MAlexa.sEntrolRelayName;
    serializeJson(doc, sData);  

    ESPServer.send(200, "text/json",sData);
}

void ESPServer_Wifi_Values(void){
    String sData;
    StaticJsonDocument<2048> doc;
    long lRSSI;

    // Measure Signal Strength (RSSI) of Wi-Fi connection
    lRSSI = WiFi.RSSI();
    if(lRSSI <= -100){
        doc["rssi"] = 0;
    }else if(lRSSI >= -50){
        doc["rssi"] = 100;
    }else{
        doc["rssi"] = 2 * (lRSSI + 100);
    }

    doc["MAC"] = EUtils.Get_MAC();

    if(EWifiControl.Get_Status() != WL_CONNECTED){
        doc["WifiStatus"] = "ERROR";
    }else{
        doc["WifiStatus"] = "CONNECTED";
    }
    
    doc["CSSID"] = WiFi.SSID();
    doc["CIP"] = WiFi.localIP().toString();
    doc["CGW"] = WiFi.gatewayIP().toString();
    doc["CSN"] = WiFi.subnetMask().toString();
    doc["CDNS"] = WiFi.dnsIP().toString();

    if(EWifiControl.sHostName == "NOWIFI"){
        doc["HOST Name"]  = "SB01" + EUtils.Get_MAC();
    }else{
        doc["HOST Name"]  = "SB01" + EWifiControl.sHostName;
    }    

    doc["Hostpassword"] = EWifiControl.sHostPassword;
    
    doc["AWN1"] = EWifiControl.sAddWifiName1;
    doc["AWP1"] = EWifiControl.sAddWifiPassword1;

    doc["AWN2"] = EWifiControl.sAddWifiName2;
    doc["AWP2"] = EWifiControl.sAddWifiPassword2;

    doc["AWN3"] = EWifiControl.sAddWifiName3;
    doc["AWP3"] = EWifiControl.sAddWifiPassword3;

    doc["AWN4"] = EWifiControl.sAddWifiName4;
    doc["AWP4"] = EWifiControl.sAddWifiPassword4;

    doc["IP1"] = EWifiControl.AddWifiIP1.toString();
    doc["GW1"] = EWifiControl.AddWifiGateway1.toString();
    doc["SN1"] = EWifiControl.AddWifiSubnet1.toString();
    doc["DN1"] = EWifiControl.AddWifiDNS1.toString();
    doc["CE1"] = EWifiControl.AddWifiConfig1.toString();
    doc["SE1"] = EWifiControl.ui8AddWifiConfigEnable1;

    doc["IP2"] = EWifiControl.AddWifiIP2.toString();
    doc["GW2"] = EWifiControl.AddWifiGateway2.toString();
    doc["SN2"] = EWifiControl.AddWifiSubnet2.toString();
    doc["DN2"] = EWifiControl.AddWifiDNS2.toString();
    doc["CE2"] = EWifiControl.AddWifiConfig2.toString();
    doc["SE2"] = EWifiControl.ui8AddWifiConfigEnable2;

    doc["IP3"] = EWifiControl.AddWifiIP3.toString();
    doc["GW3"] = EWifiControl.AddWifiGateway3.toString();
    doc["SN3"] = EWifiControl.AddWifiSubnet3.toString();
    doc["DN3"] = EWifiControl.AddWifiDNS3.toString();
    doc["CE3"] = EWifiControl.AddWifiConfig3.toString();
    doc["SE3"] = EWifiControl.ui8AddWifiConfigEnable3;

    doc["IP4"] = EWifiControl.AddWifiIP4.toString();
    doc["GW4"] = EWifiControl.AddWifiGateway4.toString();
    doc["SN4"] = EWifiControl.AddWifiSubnet4.toString();
    doc["DN4"] = EWifiControl.AddWifiDNS4.toString();
    doc["CE4"] = EWifiControl.AddWifiConfig4.toString();
    doc["SE4"] = EWifiControl.ui8AddWifiConfigEnable4;

    serializeJson(doc, sData);

    ESPServer.send(200, "text/json",sData);
}

/* this function used to load all web server related files from spiffs */
bool ESPServer_LoadFromSpiffs(String path){
    String dataType = "text/plain";
    if(path.endsWith("/")) path += "index.htm";
    if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
    else if(path.endsWith(".html")) dataType = "text/html";
    else if(path.endsWith(".htm")) dataType = "text/html";
    else if(path.endsWith(".css")) dataType = "text/css";
    else if(path.endsWith(".js")) dataType = "application/javascript";
    else if(path.endsWith(".png")) dataType = "image/png";
    else if(path.endsWith(".gif")) dataType = "image/gif";
    else if(path.endsWith(".jpg")) dataType = "image/jpeg";
    else if(path.endsWith(".ico")) dataType = "image/x-icon";
    else if(path.endsWith(".xml")) dataType = "text/xml";
    else if(path.endsWith(".pdf")) dataType = "application/pdf";
    else if(path.endsWith(".zip")) dataType = "application/zip";
    File dataFile = SPIFFS.open(path.c_str(), "r");
    if (ESPServer.hasArg("download")) dataType = "application/octet-stream";
    if (ESPServer.streamFile(dataFile, dataType) != dataFile.size()) {}

    dataFile.close();
    return true;
}

/* this function updates the modbus on client request */
void ESPServer_EntrolUpdate(void){
   	String sData;
    StaticJsonDocument<1048> doc;
        
    /* Measure Signal Strength (RSSI) of Wi-Fi connection */
    if(WiFi.RSSI() <= -100){
        doc["rssi"] = 0;
    }else if(WiFi.RSSI() >= -50){
        doc["rssi"] = 100;
    }else{
        doc["rssi"] = 2 * (WiFi.RSSI() + 100);
    }

    doc["ledg"] = EApp.Application_Pin_Read(GREEN_LED);
    doc["ledy"] = EApp.Application_Pin_Read(YELLOW_LED);
    doc["alexaS"] = EApp.ui8AlexaStatus;
    doc["mqttS"] = EApp.ui8MqttStatus;
	if(EApp.Application_Pin_Read(RELAY)){
		doc["relayS"] = "Relay On";
	}else{
		doc["relayS"] = "Relay Off";
	}
    doc["MAC"] = WiFi.macAddress();
    
    doc["Voltage"] = EMeter.fVoltage;

    doc["Current"] = EMeter.fCurrent;

    doc["Power"] = EMeter.fPower;

    doc["Unit"] = EMeter.fUnit;

    doc["Frequency"] = EMeter.fFrequency;

    doc["Powerfactor"] = EMeter.fPowerfactor;

    doc["Energyflag"] = EMeter.ui8EnergyFlag;

    serializeJson(doc, sData);  

    ESPServer.send(200, "text/json",sData);
}

void ESPServer_Get_TSValues(void){
    String sData;
    StaticJsonDocument<256> doc;

    doc["TSAPI"]    = TS.sThingspeakChAPIKey;
    doc["TSEnable"] = TS.ui8ThingspeakEnable;

    serializeJson(doc, sData);  

    ESPServer.send(200, "text/json",sData);
}

/* This handling root file */
void ESPServer_HandleRoot(void){
  ESPServer.sendHeader("Location", "/index.html",true);   //Redirect to our html web page
  ESPServer.send(302, "text/plane","");
}

/* this function handle when wrong url path requented */
void ESPServer_HandleWebRequests(void){
    if(ESPServer_LoadFromSpiffs(ESPServer.uri())) return;
    String message = "File Not Detected\n\n";
    message += "URI: ";
    message += ESPServer.uri();
    message += "\nMethod: ";
    message += (ESPServer.method() == HTTP_GET)?"GET":"POST";
    message += "\nArguments: ";
    message += ESPServer.args();
    message += "\n";
    for (uint8_t i=0; i<ESPServer.args(); i++){
        message += " NAME:"+ESPServer.argName(i) + "\n VALUE:" + ESPServer.arg(i) + "\n";
    }

    ESPServer.send(404, "text/plain", message);
}


void ESPServer_Handle_MqttParameter (void){
    String sMqttServer;
	String sMqttPort;
	String sMqttClientId;
	String sMqttUsername;
	String sMqttPassword;
	String sMqttVoltageTopic;
	String sMqttCurrentTopic;
	String sMqttPowerTopic;
    String sMqttUnitTopic;
	String sMqttFrequencyTopic;
    String sMqttPowerfactorTopic;
    String sMqttRelayTopic;
	String sMqttGetAllTopic;
    String sQos;
    String sKeepalive;
    String  sMsg = " ";
    
    sMqttServer             = ESPServer.arg("sMqttServer");
    sMqttPort               = ESPServer.arg("sMqttPort");
    sMqttClientId           = ESPServer.arg("sMqttClientId");
    sMqttUsername           = ESPServer.arg("sMqttUsername");
    sMqttPassword           = ESPServer.arg("sMqttPassword");
    sMqttVoltageTopic       =  ESPServer.arg("sMqttVoltageTopic");
    sMqttCurrentTopic       = ESPServer.arg("sMqttCurrentTopic");
    sMqttPowerTopic         = ESPServer.arg("sMqttPowerTopic");
    sMqttUnitTopic          = ESPServer.arg("sMqttUnitTopic");
    sMqttPowerfactorTopic   = ESPServer.arg("sMqttPowerfactorTopic");
    sMqttFrequencyTopic     = ESPServer.arg("sMqttFrequencyTopic");
    sMqttRelayTopic         = ESPServer.arg("sMqttRelayTopic");
    sMqttGetAllTopic        = ESPServer.arg("sMqttGetAllTopic");
    sQos                    = ESPServer.arg("sQos");
    sKeepalive              = ESPServer.arg("sMqttKeepalive");
    
    EUtils.println(sMqttServer);
    EUtils.println(sMqttPort);
    EUtils.println(sMqttClientId);
    EUtils.println(sMqttUsername);
    EUtils.println(sMqttPassword);
    EUtils.println(sMqttVoltageTopic);
    EUtils.println(sMqttCurrentTopic);
    EUtils.println(sMqttPowerTopic);
    EUtils.println(sMqttUnitTopic);
    EUtils.println(sMqttPowerfactorTopic);
    EUtils.println(sMqttFrequencyTopic);
    EUtils.println(sMqttRelayTopic);
    EUtils.println(sMqttGetAllTopic);
    EUtils.println(sQos);
    EUtils.println(sKeepalive);
    
    if(sMqttServer.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[MQTT_SERVER_INDEX],&EFile.aui32ESPFileSize[MQTT_SERVER_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[MQTT_SERVER_INDEX],sMqttServer) == FILE_STORE_SUCCESS){
                sMsg = "mqtt server to file sucess";
                MqFile.Read_Mqtt_Parameter(MQTT_SERVER_INDEX);
            }else{
                sMsg = "mqtt server store to file fail";
            }
       }else{
           sMsg = "Fail to delete mqtt server file";
       }
    }

    if(sMqttPort.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[MQTT_PORT_INDEX],&EFile.aui32ESPFileSize[MQTT_PORT_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[MQTT_PORT_INDEX],sMqttPort) == FILE_STORE_SUCCESS){
                sMsg = "mqtt port to file sucess";
                MqFile.Read_Mqtt_Parameter(MQTT_PORT_INDEX);
            }else{
                sMsg = "mqtt port store to file fail";
            }
       }else{
           sMsg = "Fail to delete mqtt port file";
       }
    }

    if(sMqttClientId.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[MQTT_CLIENTID_INDEX],&EFile.aui32ESPFileSize[MQTT_CLIENTID_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[MQTT_CLIENTID_INDEX],sMqttClientId) == FILE_STORE_SUCCESS){
                sMsg = "mqtt clientid to file sucess";
                MqFile.Read_Mqtt_Parameter(MQTT_CLIENTID_INDEX);
            }else{
                sMsg = "mqtt clientid store to file fail";
            }
       }else{
           sMsg = "Fail to delete mqtt clientid file";
       }
    }

    if(sMqttUsername.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[MQTT_USERNAME_INDEX],&EFile.aui32ESPFileSize[MQTT_USERNAME_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[MQTT_USERNAME_INDEX],sMqttUsername) == FILE_STORE_SUCCESS){
                sMsg = "mqtt username to file sucess";
                MqFile.Read_Mqtt_Parameter(MQTT_USERNAME_INDEX);
            }else{
                sMsg = "mqtt username store to file fail";
            }
       }else{
           sMsg = "Fail to delete mqtt username file";
       }
    }


    if(sMqttPassword.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[MQTT_PASSWORD_INDEX],&EFile.aui32ESPFileSize[MQTT_PASSWORD_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[MQTT_PASSWORD_INDEX],sMqttPassword) == FILE_STORE_SUCCESS){
                sMsg = "mqtt password to file sucess";
                MqFile.Read_Mqtt_Parameter(MQTT_PASSWORD_INDEX);
            }else{
                sMsg = "mqtt password store to file fail";
            }
       }else{
           sMsg = "Fail to delete mqtt password file";
       }
    }

    if(sMqttVoltageTopic.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[MQTT_PUB_VOLTAGE_TOPIC_INDEX],&EFile.aui32ESPFileSize[MQTT_PUB_VOLTAGE_TOPIC_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[MQTT_PUB_VOLTAGE_TOPIC_INDEX],sMqttVoltageTopic) == FILE_STORE_SUCCESS){
                sMsg = "mqtt voltage to file sucess";
                MqFile.Read_Mqtt_Parameter(MQTT_PUB_VOLTAGE_TOPIC_INDEX);
            }else{
                sMsg = "mqtt voltage store to file fail";
            }
       }else{
           sMsg = "Fail to delete mqtt voltage file";
       }
    }

    if(sMqttCurrentTopic.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[MQTT_PUB_CURRENT_TOPIC_INDEX],&EFile.aui32ESPFileSize[MQTT_PUB_CURRENT_TOPIC_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[MQTT_PUB_CURRENT_TOPIC_INDEX],sMqttCurrentTopic) == FILE_STORE_SUCCESS){
                sMsg = "mqtt current to file sucess";
                MqFile.Read_Mqtt_Parameter(MQTT_PUB_CURRENT_TOPIC_INDEX);
            }else{
                sMsg = "mqtt current store to file fail";
            }
       }else{
           sMsg = "Fail to delete mqtt current file";
       }
    }

    if(sMqttPowerTopic.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[MQTT_PUB_POWER_TOPIC_INDEX],&EFile.aui32ESPFileSize[MQTT_PUB_POWER_TOPIC_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[MQTT_PUB_POWER_TOPIC_INDEX],sMqttPowerTopic) == FILE_STORE_SUCCESS){
                sMsg = "mqtt power to file sucess";
                MqFile.Read_Mqtt_Parameter(MQTT_PUB_POWER_TOPIC_INDEX);
            }else{
                sMsg = "mqtt power store to file fail";
            }
       }else{
           sMsg = "Fail to delete mqtt power file";
       }
    }

    if(sMqttUnitTopic.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[MQTT_PUB_UNIT_TOPIC_INDEX],&EFile.aui32ESPFileSize[MQTT_PUB_UNIT_TOPIC_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[MQTT_PUB_UNIT_TOPIC_INDEX],sMqttUnitTopic) == FILE_STORE_SUCCESS){
                sMsg = " sMqttUnitTopic to file sucess";
                MqFile.Read_Mqtt_Parameter(MQTT_PUB_UNIT_TOPIC_INDEX);
            }else{
                sMsg = " sMqttUnitTopic store to file fail";
            }
       }else{
           sMsg = "Fail to delete sMqttUnitTopic file";
       }
    }

    if(sMqttFrequencyTopic.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[MQTT_PUB_FREQUENCY_TOPIC_INDEX],&EFile.aui32ESPFileSize[MQTT_PUB_FREQUENCY_TOPIC_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[MQTT_PUB_FREQUENCY_TOPIC_INDEX],sMqttFrequencyTopic) == FILE_STORE_SUCCESS){
                sMsg = "mqtt sMqttFrequencyTopic to file sucess";
                MqFile.Read_Mqtt_Parameter(MQTT_PUB_FREQUENCY_TOPIC_INDEX);
            }else{
                sMsg = "mqtt sMqttFrequencyTopic store to file fail";
            }
       }else{
           sMsg = "Fail to delete mqtt sMqttFrequencyTopic file";
       }
    }

    if(sMqttPowerfactorTopic.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[MQTT_PUB_PF_TOPIC_INDEX],&EFile.aui32ESPFileSize[MQTT_PUB_PF_TOPIC_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[MQTT_PUB_PF_TOPIC_INDEX],sMqttPowerfactorTopic) == FILE_STORE_SUCCESS){
                sMsg = "mqtt sMqttPowerfactorTopic to file sucess";
                MqFile.Read_Mqtt_Parameter(MQTT_PUB_PF_TOPIC_INDEX);
            }else{
                sMsg = "mqtt sMqttPowerfactorTopic store to file fail";
            }
       }else{
           sMsg = "Fail to delete mqtt sMqttPowerfactorTopic file";
       }
    }

    if(sMqttRelayTopic.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[MQTT_PUB_RELAY_TOPIC_INDEX],&EFile.aui32ESPFileSize[MQTT_PUB_RELAY_TOPIC_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[MQTT_PUB_RELAY_TOPIC_INDEX],sMqttRelayTopic) == FILE_STORE_SUCCESS){
                sMsg = "mqtt sMqttRelayTopic to file sucess";
                MqFile.Read_Mqtt_Parameter(MQTT_PUB_RELAY_TOPIC_INDEX);
            }else{
                sMsg = "mqtt sMqttRelayTopic store to file fail";
            }
       }else{
           sMsg = "Fail to delete mqtt sMqttRelayTopic file";
       }
    }

    if(sMqttRelayTopic.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[MQTT_SUB_RELAY_TOPIC_INDEX],&EFile.aui32ESPFileSize[MQTT_SUB_RELAY_TOPIC_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[MQTT_SUB_RELAY_TOPIC_INDEX],sMqttRelayTopic) == FILE_STORE_SUCCESS){
                sMsg = "mqtt sMqttRelayTopic to file sucess";
                MqFile.Read_Mqtt_Parameter(MQTT_SUB_RELAY_TOPIC_INDEX);
            }else{
                sMsg = "mqtt sMqttRelayTopic store to file fail";
            }
       }else{
           sMsg = "Fail to delete mqtt sMqttRelayTopic file";
       }
    }

    if(sMqttGetAllTopic.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[MQTT_SUB_GETALL_TOPIC_INDEX],&EFile.aui32ESPFileSize[MQTT_SUB_GETALL_TOPIC_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[MQTT_SUB_GETALL_TOPIC_INDEX],sMqttGetAllTopic) == FILE_STORE_SUCCESS){
                sMsg = "mqtt sMqttGetAllTopic to file sucess";
                MqFile.Read_Mqtt_Parameter(MQTT_SUB_GETALL_TOPIC_INDEX);
            }else{
                sMsg = "mqtt sMqttGetAllTopic store to file fail";
            }
       }else{
           sMsg = "Fail to delete mqtt sMqttGetAllTopic file";
       }
    }

    if((sQos.equals("")== false) and (sQos.equals("4")== false)){
        
       if(EFile.DeleteFile(sESPFiles[MQTT_QOS_INDEX],&EFile.aui32ESPFileSize[MQTT_QOS_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[MQTT_QOS_INDEX],sQos) == FILE_STORE_SUCCESS){
                sMsg = "mqtt qos to file sucess";
                MqFile.Read_Mqtt_Parameter(MQTT_QOS_INDEX);
            }else{
                sMsg = "mqtt qos store to file fail";
            }
       }else{
           sMsg = "Fail to delete mqtt qos file";
       }
    }


    if(sKeepalive.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[MQTT_KEEPALIVE_INDEX],&EFile.aui32ESPFileSize[MQTT_KEEPALIVE_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[MQTT_KEEPALIVE_INDEX],sKeepalive) == FILE_STORE_SUCCESS){
                sMsg = "mqtt keepalive to file sucess";
                MqFile.Read_Mqtt_Parameter(MQTT_KEEPALIVE_INDEX);
            }else{
                sMsg = "mqtt keepalive store to file fail";
            }
       }else{
           sMsg = "Fail to delete mqtt keepalive file";
       }
    }

    if(sMsg.equals(" ")){
        sMsg = "no action taken ";
    }

    EUtils.println(sMsg);

    ESPServer.send(200, "text/plain",sMsg);
}

void ESPServer_Handle_Alexa(void){
    String sAlexaAPI;
    String senRelayID;
    String senRelayName;

    String  sMsg = " ";
    
    sAlexaAPI   = ESPServer.arg("sAlexaAPI");
    senRelayID = ESPServer.arg("sAlexarelayid");
    senRelayName = ESPServer.arg("sAlexarelayname");

    EUtils.println(sAlexaAPI);
    EUtils.println(senRelayID);
    EUtils.println(senRelayName);
     
   if(sAlexaAPI.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[ALEXA_TOKEN_INDEX],&EFile.aui32ESPFileSize[ALEXA_TOKEN_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[ALEXA_TOKEN_INDEX],sAlexaAPI) == FILE_STORE_SUCCESS){
                sMsg = "Alexa API keys store to file sucess";
                MAlexa.Update_Specified_File(ALEXA_TOKEN_INDEX);
            }else{
                sMsg = "Alexa API keys store to file fail";
            }
       }else{
           sMsg = "Fail to delete Alexa API file";
       }
       
       //EUtils.println("File read"); 
       //EUtils.println(EFile.Read_Data_From_File(sESPFiles[ALEXA_TOKEN_INDEX]));
    }

    if(senRelayID.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[ALEXA_ENTROL_ID_INDEX],&EFile.aui32ESPFileSize[ALEXA_ENTROL_ID_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[ALEXA_ENTROL_ID_INDEX],senRelayID) == FILE_STORE_SUCCESS){
                sMsg = sMsg + " senRelayID store to file sucess";
                MAlexa.Update_Specified_File(ALEXA_ENTROL_ID_INDEX);
            }else{
                sMsg = sMsg + " senRelayID keys store to file fail";
            }
       }else{
           sMsg = sMsg + " Fail to delete sSBRelayID file";
       }
       
       //EUtils.println("File read"); 
       //EUtils.println(EFile.Read_Data_From_File(sESPFiles[sACInput1ID]));
    }

    if(senRelayName.equals("")== false){

       if(EFile.DeleteFile(sESPFiles[ALEXA_ENTROL_NAME_INDEX],&EFile.aui32ESPFileSize[ALEXA_ENTROL_NAME_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[ALEXA_ENTROL_NAME_INDEX],senRelayName) == FILE_STORE_SUCCESS){
                sMsg = sMsg + " senRelayName store to file sucess";
                MAlexa.Update_Specified_File(ALEXA_ENTROL_NAME_INDEX);
            }else{
                sMsg = sMsg + " senRelayName keys store to file fail";
            }
       }else{
           sMsg = sMsg + " Fail to delete senRelayName file";
       }
       
       //EUtils.println("File read"); 
       //EUtils.println(EFile.Read_Data_From_File(sESPFiles[sACInput1ID]));
    }

    if(sMsg.equals(" ")){
        sMsg = "no action taken ";
    }

    EUtils.println(sMsg);

    ESPServer.send(200, "text/plain",sMsg);
}

void ESPServer_Handle_WifiAdd(void){
    String sWifiName;
    String sWifiPassword;
    uint8_t ui8WifiNumber;
    String  sMsg;

    if (ESPServer.arg("username")!= ""){
        sWifiName = ESPServer.arg("username");
        if (ESPServer.arg("password")!= ""){
            sWifiPassword = ESPServer.arg("password");
        }else{
            sWifiPassword = "NoPassword";
        }

        if (ESPServer.arg("number")!= ""){
            ui8WifiNumber = ESPServer.arg("number").toInt();
            if(ui8WifiNumber > 3){
                ui8WifiNumber = 3;
            }

            if(ui8WifiNumber < 0){
                ui8WifiNumber = 0;
            }

        }else{
            ui8WifiNumber = 0;
        }

        if(EFile.DeleteFile(sESPFiles[WIFI_1_SSID_INDEX + ui8WifiNumber],&EFile.aui32ESPFileSize[WIFI_1_SSID_INDEX + ui8WifiNumber]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[WIFI_1_SSID_INDEX+ ui8WifiNumber],sWifiName) == FILE_STORE_SUCCESS){
                sMsg = "Add wifi sucess";
                if(EFile.DeleteFile(sESPFiles[WIFI_1_PASSWORD_INDEX + ui8WifiNumber],&EFile.aui32ESPFileSize[WIFI_1_PASSWORD_INDEX + ui8WifiNumber]) == FILE_DELETE_SUCCESS){
                    if(EFile.Write_Data_To_File(sESPFiles[WIFI_1_PASSWORD_INDEX + ui8WifiNumber],sWifiPassword) == FILE_STORE_SUCCESS){
                        sMsg = "Add wifi sucess";
                        EWifiControl.Update_Wifi_Add_File(ui8WifiNumber);
                    }else{
                        sMsg = "Add wifi fail";
                    }
                }else{
               
                    sMsg = "Fail to delete file";
                }
            }else{
                sMsg = "Add wifi fail";
            }
        }else{
           sMsg = "Fail to delete file";
        }
    }

    ESPServer.send(200, "text/plain",sMsg);
}

void ESPServer_Handle_WifiHostname(void){
    String sMsg;

    if (ESPServer.arg("hostname")!= ""){
        if(EFile.DeleteFile(sESPFiles[WIFI_HOSTNAME_INDEX],&EFile.aui32ESPFileSize[WIFI_HOSTNAME_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[WIFI_HOSTNAME_INDEX],ESPServer.arg("hostname")) == FILE_STORE_SUCCESS){
                sMsg = "wifi hostname received";
            }else{
                sMsg = "wifi hostname failed";
            }
        }else{
           sMsg = "Fail to delete file";
        }
    }

    if (ESPServer.arg("hostpassword")!= ""){
        if(EFile.DeleteFile(sESPFiles[HOST_PASSWORD_INDEX],&EFile.aui32ESPFileSize[HOST_PASSWORD_INDEX]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[HOST_PASSWORD_INDEX],ESPServer.arg("hostpassword")) == FILE_STORE_SUCCESS){
                sMsg = "wifi hostpassword received";
            }else{
                sMsg = "wifi hostpassword failed";
            }
        }else{
           sMsg = "Fail to delete file";
        }
    }

    EWifiControl.Update_Host_Name();

    ESPServer.send(200, "text/plain",sMsg);
}


void ESPServer_Handle_WifiParameterAdd(void){
    String sIpAddr;
    String sGateway;
    String sSubnet;
    String sDNSIP;
    String sConfigEnable;
    String sMsg;
    String sWifiParamter;
    String sSubString;
    uint8_t ui8WifiIndex;
    
    sIpAddr = ESPServer.arg("ipaddr");
    sGateway = ESPServer.arg("gateway");
    sSubnet = ESPServer.arg("subnet");
    sDNSIP = ESPServer.arg("dnsip");
    sConfigEnable = ESPServer.arg("staticconfig");
    ui8WifiIndex  = ESPServer.arg("wifinumber").toInt();

    Serial1.println("file index");
    Serial1.println(ui8WifiIndex);

    if(sIpAddr.equals("A.B.C.D") == false){
        if(EWifiControl.Update_Wifi_Add_Static_Config(ui8WifiIndex,2,sIpAddr)){
            EWifiControl.Get_Wifi_Add_Parameter(ui8WifiIndex);
            sMsg  = "Wifi IP add sucess";
        }else{
            sMsg  = "Wifi IP add failed";
        }
    }

    if(sGateway.equals("A.B.C.D") == false){
        if(EWifiControl.Update_Wifi_Add_Static_Config(ui8WifiIndex,3,sGateway)){
            EWifiControl.Get_Wifi_Add_Parameter(ui8WifiIndex);
            sMsg  = "Wifi Gateway add sucess";
        }else{
            sMsg  = "Wifi Gateway add failed";
        }
    }
    
    if(sSubnet.equals("A.B.C.D") == false){
        if(EWifiControl.Update_Wifi_Add_Static_Config(ui8WifiIndex,4,sSubnet)){
            EWifiControl.Get_Wifi_Add_Parameter(ui8WifiIndex);
            sMsg  = "Wifi add sucess";
        }else{
            sMsg  = "Wifi add failed";
        }
    }

    if(sDNSIP.equals("A.B.C.D") == false){
        if(EWifiControl.Update_Wifi_Add_Static_Config(ui8WifiIndex,5,sDNSIP)){
            EWifiControl.Get_Wifi_Add_Parameter(ui8WifiIndex);
            sMsg  = "Wifi add sucess";
        }else{
            sMsg  = "Wifi add failed";
        }
    }

    if(sConfigEnable.equals("2") == false){
        if(EWifiControl.Update_Wifi_Add_Static_Config(ui8WifiIndex,1,sConfigEnable)){
            EWifiControl.Get_Wifi_Add_Parameter(ui8WifiIndex);
            sMsg  = "Wifi add sucess";
        }else{
            sMsg  = "Wifi add failed";
        }
    }

    ESPServer.send(200, "text/plain",sMsg);
    
}

void ESPServer_Handle_TSParameter(void){
    String sChannelapi;
    uint8_t ui8Enable;
    String  sMsg;


    if (ESPServer.arg("channelapi")!= "NOAPI"){
        sChannelapi = ESPServer.arg("channelapi");
        if(EFile.DeleteFile(sESPFiles[TS_CHANNEL_API],&EFile.aui32ESPFileSize[TS_CHANNEL_API]) == FILE_DELETE_SUCCESS){
            if(EFile.Write_Data_To_File(sESPFiles[TS_CHANNEL_API],ESPServer.arg("channelapi")) == FILE_STORE_SUCCESS){
                sMsg = " ts config received";
            }else{
                sMsg = "ts config failed";
            }
        }else{
           sMsg = "Fail to delete file";
        }
    }
        
    if (ESPServer.arg("enable")!= ""){
        ui8Enable = ESPServer.arg("enable").toInt();
        if(ui8Enable != 2){
            if(EFile.DeleteFile(sESPFiles[TS_ENABLE_FLAG],&EFile.aui32ESPFileSize[TS_ENABLE_FLAG]) == FILE_DELETE_SUCCESS){
                if(EFile.Write_Data_To_File(sESPFiles[TS_ENABLE_FLAG],ESPServer.arg("enable")) == FILE_STORE_SUCCESS){
                    sMsg = " ts config received";
                }else{
                    sMsg = "ts config failed";
                }
            }else{
                sMsg = "Fail to delete file";
            }
        }
    }

    TS.Read_All_Thingspeak_Parameters();

    ESPServer.send(200, "text/plain",sMsg);
}

void MQTT_Handle(void) {
    if(ui8MqttSendFlag){
        Mqtt_Publish_All();
    }
}

void onMqttPublish(uint16_t packetId) {
  Serial1.println("Publish acknowledged.");
  
  //Serial1.print("  packetId: ");
  //Serial1.println(packetId);
  //sEndTime = micros();
  //Serial1.println(sEndTime - sStratTime);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  uint8_t ui8Payload;

  Serial1.println("Publish received.");
  Serial1.print("  topic: ");
  Serial1.println(topic);
  Serial1.print("  qos: ");
  Serial1.println(properties.qos);
  Serial1.print("  dup: ");
  Serial1.println(properties.dup);
  Serial1.print("  retain: ");
  Serial1.println(properties.retain);
  Serial1.print("  len: ");
  Serial1.println(len);
  Serial1.print("  index: ");
  Serial1.println(index);
  Serial1.print("  total: ");
  Serial1.println(total);
  Serial1.println(payload);

    ui8Payload = String(payload).toInt();
    Serial1.print("Payload: ");
    Serial1.println(ui8Payload);

    if(strcmp(topic,MqFile.sMqttPubAllTopic.c_str()) == 0){
        Mqtt_Publish_All();
    }

    if(strcmp(topic,MqFile.sMqttSubRelayTopic.c_str()) == 0){
        if(ui8Payload){
            EApp.Application_Pin_Write(RELAY, HIGH);
        }else{
            EApp.Application_Pin_Write(RELAY, LOW);
        }
        Mqtt_Publish_All();
    }

}

void onMqttUnsubscribe(uint16_t packetId) {
  Serial1.println("Unsubscribe acknowledged.");
  Serial1.print("  packetId: ");
  Serial1.println(packetId);
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial1.println("Subscribe acknowledged.");
  Serial1.print("  packetId: ");
  Serial1.println(packetId);
  Serial1.print("  qos: ");
  Serial1.println(qos);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    Serial1.print("Disconnected from MQTT Reason: ");
    Serial1.printf("%d",reason);
    Serial1.println();
    
    ui8MqttSendFlag = 0;
    EApp.ui8MqttStatus = 0;

    if (WiFi.isConnected()) {
        connectToMqtt();
        //MqttReconnectTask.enable();
    }
}

void connectToMqtt(void) {
  Serial1.println("Connecting to MQTT...");
  mqttClient.setClientId(MqFile.sMqttClientId.c_str());
  mqttClient.setCredentials(MqFile.sMqttUsername.c_str(),MqFile.sMqttPassword.c_str());
  mqttClient.setServer(MqFile.sMqttServer.c_str(),MqFile.ui16MqttPort);
  mqttClient.setKeepAlive(MqFile.ui16MqttKeepalive);
  mqttClient.setWill(MqFile.sMqttDeviceStatus.c_str(),MqFile.ui8MqttQoS,1,"offline",strlen("offline"));
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent) {
    
    Serial1.println("Connected to MQTT.");
    Serial1.print("Session present: ");
    Serial1.println(sessionPresent);

    ui8MqttSendFlag = 1;
    EApp.ui8MqttStatus = 1;

    mqttClient.subscribe(MqFile.sMqttSubRelayTopic.c_str(),MqFile.ui8MqttQoS);
    mqttClient.subscribe(MqFile.sMqttPubAllTopic.c_str(),MqFile.ui8MqttQoS);

    Mqtt_Publish_All();
    mqttClient.publish(MqFile.sMqttDeviceStatus.c_str(), MqFile.ui8MqttQoS, true,"online");
}

void Mqtt_Publish_All(void){
    mqttClient.publish(MqFile.sMqttPubVoltageTopic.c_str(),MqFile.ui8MqttQoS, true,String(EMeter.fVoltage).c_str());

    mqttClient.publish(MqFile.sMqttPubCurrentTopic.c_str(),MqFile.ui8MqttQoS, true,String(EMeter.fCurrent).c_str());

    mqttClient.publish(MqFile.sMqttPubPowerTopic.c_str(),MqFile.ui8MqttQoS, true,String(EMeter.fPower).c_str());

    mqttClient.publish(MqFile.sMqttPubUnitTopic.c_str(),MqFile.ui8MqttQoS, true,String(EMeter.fUnit).c_str());

    mqttClient.publish(MqFile.sMqttPubFrequencyTopic.c_str(),MqFile.ui8MqttQoS, true,String(EMeter.fFrequency).c_str());

    mqttClient.publish(MqFile.sMqttPubpowerfactorTopic.c_str(),MqFile.ui8MqttQoS, true,String(EMeter.fPowerfactor).c_str());

    mqttClient.publish(MqFile.sMqttPubRelayTopic.c_str(),MqFile.ui8MqttQoS, true,String(EApp.ui8RelayStatus).c_str());

    Serial1.println("Publish all");
}

void Mqtt_Check_For_Alexa_Sync(void){
    if(EApp.ui8AlexaUpdated){
        EUtils.println("Alexa mqtt sync");
        EApp.ui8AlexaUpdated=0;
        MQTT_Handle();
    }
}

/* this function updates the relay status on client request */
void ESPServer_RelayUpdate(void){
   	String sData;
	
  	Serial1.print("Relay web request: ");
  	Serial1.println(ESPServer.arg("state"));
	if (ESPServer.arg("state")=="1"){
		sData = "Relay:on";
		EApp.Application_Pin_Write(RELAY, HIGH);
	}else{
		sData = "Relay:off";
		EApp.Application_Pin_Write(RELAY, LOW);
	}
	Serial1.println(sData);
	ESPServer.send(200, "text/json",sData);
}

void ESPServer_Handle_DeviceReset (void){
    String sReset;
   
    String  sMsg = " ";
    
    sReset   = ESPServer.arg("value");
    
    if(sReset.equals("1")){
        sMsg = "device will resetart now";
        ui8DeviceResetFlag=1;
    }

    if(sMsg.equals(" ")){
        sMsg = "no action taken ";
    }

    EUtils.println(sMsg);

    ESPServer.send(200, "text/plain",sMsg);
}