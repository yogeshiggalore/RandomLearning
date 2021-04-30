#include <Arduino.h>
#include <ESPMqtt.h>
#include <ESPUtils.h>
#include <ArduinoJson.h>
#include <MqttFile.h>
#include <LogLevel.h>
#include <logger.h>

void ESPMqtt_Start(void){

	pinMode(MQTT_LED_PIN, OUTPUT);
	digitalWrite(MQTT_LED_PIN, LOW);

    FMqtt.Start();

    /* set callback function on wifi event */
	WiFi.onEvent(WiFiEvent);

	/* set callback function on mqtt connect */
	mqttClient.onConnect(On_Mqtt_Connect);

	/* set callback function on mqtt disconnect */
	mqttClient.onDisconnect(On_Mqtt_Disconnect);

	/* set callback function on client subscribe to topic */
	mqttClient.onSubscribe(On_Mqtt_Subscribe);

	/* set callback function on client unsubscribe to topic */
	mqttClient.onUnsubscribe(On_Mqtt_Unsubscribe);

	/* set callback function client on received mqtt messege */
	mqttClient.onMessage(On_Mqtt_Message);

	/* set callback function on client publish topic */
	mqttClient.onPublish(On_Mqtt_Publish);

    Connect_To_Mqtt();
}

void Connect_To_Mqtt(void) {

	elog.Write(LogLevel::Info,"MQTT", "Connecting to MQTT...");
    
    //FMqtt.Print_Mqtt_Parameters();
	
    /* set mqtt client id */
	mqttClient.setClientId(FMqtt.tdfMqttPar.sClientId.c_str());
	
	/* set username and password */
	mqttClient.setCredentials(FMqtt.tdfMqttPar.sUserName.c_str(), FMqtt.tdfMqttPar.sPassword.c_str());
	
	/* set server and port */
	mqttClient.setServer(FMqtt.tdfMqttPar.sServer.c_str(), FMqtt.tdfMqttPar.ui16Port);
	
	/* set keepalive value */
	mqttClient.setKeepAlive(FMqtt.tdfMqttPar.ui16KeepAlive);
	
	/* set will message */
	mqttClient.setWill(FMqtt.tdfMqttPar.tdfTopic.sWill.c_str(),FMqtt.tdfMqttPar.ui8QoS,FMqtt.tdfMqttPar.ui8Retain,"offline",strlen("offline"));
	
	/* connect to mqtt */
	mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {

	String sPrintString;
	sPrintString = "[WiFi-event] event: " + String(event); 
	elog.Write(LogLevel::Info,"MQTT", sPrintString.c_str());

	switch(event){
		/* If modules got IP from router */
		case SYSTEM_EVENT_STA_GOT_IP:
			sPrintString = "WiFi connected IP: " + WiFi.localIP().toString(); 
			elog.Write(LogLevel::Info,"MQTT", sPrintString.c_str());
			/* connect to mqtt */
        	Connect_To_Mqtt();
        	break;
		/* if module disconnects from router */	
		case SYSTEM_EVENT_STA_DISCONNECTED:
            FMqtt.tdfMqttPar.bStatus = 0;
        	break;
		default:
			break;
    }
}

void On_Mqtt_Connect(bool sessionPresent) {
	uint16_t ui16LoopCounter=0;
	String sPrintString;

	/* publish will topic to server */
	mqttClient.publish(FMqtt.tdfMqttPar.tdfTopic.sWill.c_str(),FMqtt.tdfMqttPar.ui8QoS, FMqtt.tdfMqttPar.ui8Retain, "online");
	
	FMqtt.tdfMqttPar.bStatus = 1;

	sPrintString = "Connected to MQTT, Publishing will topic "; 
	elog.Write(LogLevel::Info,"MQTT", sPrintString.c_str());

	digitalWrite(MQTT_LED_PIN, HIGH);

    //Mqtt_Pusblish_Data();
}

void On_Mqtt_Disconnect(AsyncMqttClientDisconnectReason reason) {
	
	FMqtt.tdfMqttPar.bStatus = 0;
    FMqtt.tdfMqttPar.bStartInitiated = 0;

	String sPrintString;
	sPrintString = "Disconnected from MQTT. reason " + String((uint8_t)reason); 
	elog.Write(LogLevel::Info,"MQTT", sPrintString.c_str());

	digitalWrite(MQTT_LED_PIN, LOW);
}

void On_Mqtt_Subscribe(uint16_t packetId, uint8_t qos) {
	String sPrintString;
	sPrintString = "Subscribe acknowledged packetId: " + String(packetId) + " QoS:" + String(qos); 
	elog.Write(LogLevel::Info,"MQTT", sPrintString.c_str());
}

void On_Mqtt_Unsubscribe(uint16_t packetId) {
	String sPrintString;
	sPrintString = "Unsubscribe acknowledged packetId " + String(packetId); 
	elog.Write(LogLevel::Info,"MQTT", sPrintString.c_str());
}

void On_Mqtt_Message(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total){

	String sPrintString;
	sPrintString = "Publish received.   topic: " + String(topic) + " qos: " + String(properties.qos) + " dup: " + String(properties.dup) + " retain: " + String(properties.retain) + " len:" + String(len) + " index: " + String(index) + " total: " +String(total) + " payload: " + String(payload); 
	elog.Write(LogLevel::Info,"MQTT", sPrintString.c_str());

}

void On_Mqtt_Publish(uint16_t packetId) {
	elog.Write(LogLevel::Info,"MQTT", "Publish acknowledged");
}

void Check_Mqtt_Connection(void){
    if(FMqtt.tdfMqttPar.bStatus == 0){
        if(FMqtt.tdfMqttPar.bStartInitiated == 0){
            FMqtt.tdfMqttPar.bStartInitiated = 1;
            Connect_To_Mqtt();
        }
    }
}

void Mqtt_Send_Dispenser_Data(String sString){
	uint8_t ui8Index;
	String sSubString;
	String sRFID;
	String sAmount;
	String sExpiryDate;
	String sPhoneNumber;
	String sCurrentTime;
	String sData;
    StaticJsonDocument<1024> doc;

	ui8Index = sString.indexOf("RNFC:") + 5;
	sSubString = sString.substring(ui8Index);
	sRFID = sSubString.substring(0,sSubString.indexOf(','));
	ui8Index = sRFID.length() + 1;
	sSubString = sSubString.substring(ui8Index);
	sAmount = sSubString.substring(0,sSubString.indexOf(','));
	ui8Index = sAmount.length() + 1;
	sSubString = sSubString.substring(ui8Index);
	sExpiryDate = sSubString.substring(0,sSubString.indexOf(','));
	ui8Index = sExpiryDate.length() + 1;
	sSubString = sSubString.substring(ui8Index);
	sPhoneNumber = sSubString.substring(0,sSubString.indexOf(','));
	ui8Index = sPhoneNumber.length() + 1;
	sSubString = sSubString.substring(ui8Index);
	sCurrentTime = sSubString.substring(0,sSubString.indexOf(';'));

	Serial.println(sRFID);
	Serial.println(sAmount);
	Serial.println(sExpiryDate);
	Serial.println(sPhoneNumber);
	Serial.println(sCurrentTime);

	doc["RFID"] = sRFID;
	doc["Amount"] = sAmount;
	doc["ExpiryDate"] = sExpiryDate;
	doc["Phonenumber"] = sPhoneNumber;
	doc["datetime"] = sCurrentTime;

	serializeJson(doc, sData);
	//Serial.println(sData);

	if(FMqtt.tdfMqttPar.bStatus){
		mqttClient.publish(FMqtt.tdfMqttPar.tdfTopic.sPubJson.c_str(),FMqtt.tdfMqttPar.ui8QoS,true,(sData).c_str());
	}
}

void Mqtt_Publish_Content(String sTopic, String sPayload, uint8_t ui8QoS){
	if(FMqtt.tdfMqttPar.bStatus){
		elog.Write(LogLevel::Info,"MQTT","Sending MQTT data to server");
		mqttClient.publish(sTopic.c_str(),ui8QoS,true,sPayload.c_str());
	}else{
		elog.Write(LogLevel::Info,"MQTT","Internet error to send data");
	}
}

/*void Mqtt_Pusblish_Data(void){
    Serial.println("Publishing mqtt topics");

    // publishing RTC data
    mqttClient.publish(FMqtt.tdfMqttPar.tdfTopic.sPubTime.c_str(),FMqtt.tdfMqttPar.ui8QoS,true,(ERTC.Read()).c_str());


}*/

