/*******************************************************************************
* Project Name: MQTTDemo
*
* Version: 1.0
*
* Description:
* In this project ESP32 handles Mqtt connections
*
* Written By:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , None
*******************************************************************************/

/* include wifi library for esp32 module */
#include <WiFi.h>

/* dht22 library */
#include <DHT.h>

/* include mqtt asynchronous client */
#include <AsyncMqttClient.h>

/* define dht22 pins */
#define DHT22_PIN	4

/* define type as DHT22 */
#define DHTTYPE 	DHT22 

/* wifi ssid and password */
#define WIFI_SSID 		"*****"
#define WIFI_PASSWORD 	"*****"

/* mqtt server/host */
#define MQTT_HOST         "*****"

/* mqtt port secure communication use 8883 */
#define MQTT_PORT         1883

/* mqtt credentails for connection username and password */
#define MQTT_USERNAME     "*****"
#define MQTT_PASSWORD     "****"

/* keep alive in second decides how long server should keep client connection */
#define MQTT_KEEPALIVE    30

/* mqtt Quality of Service 
  0 : At most once
  1 : At least once
  2 : Exactly once 
*/

#define MQTT_QOS          2

/* retain flag if client disconnect from server */
#define MQTT_RETAIN       1

/* onboard led pin number */
#define LED_PIN	2

/* create dht22 intence */
DHT dht(DHT22_PIN, DHTTYPE);

/* DHT22 veriables */
float fHumidity;
float fTemperature;
float fHeatIndex;

/* cleintid should be unique */
String sClientId;

/* will messege on client disconnection and connection to other client */
String sMqttWillTopic;

/* mqtt topic for demo */
String sMqttTemperatureTopic;
String sMqttHumidityTopic;
String sMqttLedTopic;

uint16_t ui16TempPacketId;
uint16_t ui16HumidityPacketId;
uint16_t ui16LedPacketId;
uint8_t  ui8MqttStatus=0;

char buff[10];

/* mqtt client instance */
AsyncMqttClient mqttClient;

/* timer handling for mqtt and wifi reconnection */
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

/* timer interval for reading temperature and hall sensor */
TimerHandle_t ReadPublishIntervalTimer;

/* function to read dht22 */
uint8_t Read_DHT22(void);
void Connect_To_Wifi(void);
void Read_Publish_Temp_Humidity(void);
void Connect_To_Mqtt(void);
void WiFiEvent(WiFiEvent_t event);
void On_Mqtt_Connect(bool sessionPresent);
void On_Mqtt_Disconnect(AsyncMqttClientDisconnectReason reason);
void On_Mqtt_Subscribe(uint16_t packetId, uint8_t qos);
void On_Mqtt_Unsubscribe(uint16_t packetId);
void On_Mqtt_Message(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
void On_Mqtt_Publish(uint16_t packetId);


/****************************************************************************** 
* Function Name: setup
*******************************************************************************
*
* Summary:
*  This function calls on start of program
*  
*
*  
*
* Parameters:  
*  None
*
* Return: 
*  None
*  
*******************************************************************************/
void setup() {
	
	/* set serial baudrate to 115200 and start serial port */
	Serial.begin(921600);
	Serial.println();
	Serial.println();
	
	/* start dht22 sensor */
	dht.begin();

	delay(1000);

	Read_DHT22();

	/* set mqtt timer with 2 second interval */
	mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(Connect_To_Mqtt));
	
	/* set wifi timer with 2 second interval */
	wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(Connect_To_Wifi));

	/* set timer interval to 60 second to read and publish data */
	ReadPublishIntervalTimer = xTimerCreate("ReadPublishIntervalTimer", pdMS_TO_TICKS(10000), pdTRUE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(Read_Publish_Temp_Humidity));


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
	
	/* connect to wifi */
	Connect_To_Wifi();

	/* set onbaord led pin as output */
	pinMode(LED_PIN, OUTPUT);

	/* start read publish interval timer */
	xTimerStart(ReadPublishIntervalTimer, 0);
}

void loop() {
		/* read DHT22 */
	Read_DHT22();

	/* delay for 5 seconds */
	delay(5000);
}


/****************************************************************************** 
* Function Name: Connect_To_Wifi
*******************************************************************************
*
* Summary:
*  This function connects esp32 module to wifi with given ssid and password
*  
*
*  
*
* Parameters:  
*  None
*
* Return: 
*  None
*  
*******************************************************************************/
void Connect_To_Wifi(void) {
	Serial.println("Connecting to Wi-Fi...");
	
	/* this line begins the wifi connction */
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	
	/* for client id we are using mac address */
	sClientId = WiFi.macAddress();
	
	/* will message topic  */
	sMqttWillTopic = WiFi.macAddress() + "/status";
	Serial.println(sMqttWillTopic);

	/* temperature read topic */
	sMqttTemperatureTopic = WiFi.macAddress() + "/temperature";
	Serial.println(sMqttTemperatureTopic);

	/* humidity sensor read topic */
	sMqttHumidityTopic = WiFi.macAddress() + "/humidity";
	Serial.println(sMqttHumidityTopic);

	/* led control topic */
	sMqttLedTopic = WiFi.macAddress() + "/led";
	Serial.println(sMqttLedTopic);
}

/****************************************************************************** 
* Function Name: Read_Publish_Temp_Hall
*******************************************************************************
*
* Summary:
*  This function reads temperature and hall values and publish to server
*  
*
*  
*
* Parameters:  
*  None
*
* Return: 
*  None
*  
*******************************************************************************/
void Read_Publish_Temp_Humidity(void){
	uint16_t ui16UnScb;
	static long time;

	Serial.println(millis() - time);
	time = millis();

	if(ui8MqttStatus){
		/* publish temperature topic to server */
		snprintf (buff, sizeof(buff), "%f", fTemperature);
		ui16TempPacketId = mqttClient.publish(sMqttTemperatureTopic.c_str(),MQTT_QOS, MQTT_RETAIN, buff);
		Serial.printf("Publishing at QoS %d, packetId: %d\n",MQTT_QOS,ui16TempPacketId);

		/* publish hall sensor topic to server */
		snprintf (buff, sizeof(buff), "%f", fHumidity);
		ui16HumidityPacketId = mqttClient.publish(sMqttHumidityTopic.c_str(),2, MQTT_RETAIN, buff);
		Serial.printf("Publishing at QoS %d, packetId: %d\n",MQTT_QOS,ui16HumidityPacketId);
	}
}

/****************************************************************************** 
* Function Name: Connect_To_Mqtt
*******************************************************************************
*
* Summary:
*  This function connects esp32 module to wifi with given ssid and password
*  
*
*  
*
* Parameters:  
*  None
*
* Return: 
*  None
*  
*******************************************************************************/
void Connect_To_Mqtt(void) {
	Serial.println("Connecting to MQTT...");

	/* set mqtt client id */
	mqttClient.setClientId(sClientId.c_str());
	
	/* set username and password */
	mqttClient.setCredentials(MQTT_USERNAME,MQTT_PASSWORD);
	
	/* set server and port */
	mqttClient.setServer(MQTT_HOST, MQTT_PORT);
	
	/* set keepalive value */
	mqttClient.setKeepAlive(MQTT_KEEPALIVE);
	
	/* set will message */
	mqttClient.setWill(sMqttWillTopic.c_str(),MQTT_QOS,MQTT_RETAIN,"offline",strlen("offline"));
	
	/* connect to mqtt */
	mqttClient.connect();
}

/****************************************************************************** 
* Function Name: WiFiEvent
*******************************************************************************
*
* Summary:
*  This function calls when any wifi event like connection and disconnection happens
*  
*
*  
*
* Parameters:  
*  None
*
* Return: 
*  None
*  
*******************************************************************************/
void WiFiEvent(WiFiEvent_t event) {
	Serial.printf("[WiFi-event] event: %d\n", event);
	switch(event){
		/* If modules got IP from router */
		case SYSTEM_EVENT_STA_GOT_IP:
			Serial.println("WiFi connected");
        	Serial.println("IP address: ");
        	Serial.println(WiFi.localIP());
			/* connect to mqtt */
        	Connect_To_Mqtt();
        	break;
		/* if module disconnects from router */	
		case SYSTEM_EVENT_STA_DISCONNECTED:
        	Serial.println("WiFi lost connection");
			/* stop mqqt reconnect timer */
        	xTimerStop(mqttReconnectTimer, 0);

			/* start wifi re connect timer */
			xTimerStart(wifiReconnectTimer, 0);
        	break;
		default:
			break;
    }
}

/****************************************************************************** 
* Function Name: On_Mqtt_Connect
*******************************************************************************
*
* Summary:
*  This function calls when mqtt connects to server
*  
*
*  
*
* Parameters:  
*  None
*
* Return: 
*  None
*  
*******************************************************************************/
void On_Mqtt_Connect(bool sessionPresent) {
	uint16_t ui16LoopCounter=0;

	Serial.println("Connected to MQTT.");
	
	/* print session */
	Serial.print("Session present: ");
	Serial.println(sessionPresent);
	
	/* publish temperature topic to server */
	snprintf (buff, sizeof(buff), "%f", fTemperature);
	ui16TempPacketId = mqttClient.publish(sMqttTemperatureTopic.c_str(),MQTT_QOS, MQTT_RETAIN, buff);
	Serial.printf("Publishing at QoS %d, packetId: %d\n",MQTT_QOS,ui16TempPacketId);

	/* publish hall sensor topic to server */
	snprintf (buff, sizeof(buff), "%f", fHumidity);
	ui16HumidityPacketId = mqttClient.publish(sMqttHumidityTopic.c_str(),MQTT_QOS, MQTT_RETAIN,buff);
	Serial.printf("Publishing at QoS %d, packetId: %d\n",2,ui16HumidityPacketId);

	/* publish will topic to server */
	mqttClient.publish(sMqttWillTopic.c_str(),MQTT_QOS, MQTT_RETAIN, "online");
	Serial.printf("Publishing will topic\n");

	/* subscribe to led control packet */
	uint16_t ui16LedPacketId = mqttClient.subscribe(sMqttLedTopic.c_str(),MQTT_QOS);
	Serial.printf("Subscribing to led topic QoS:%d\n", ui16LedPacketId);

	ui8MqttStatus = 1;
}

/****************************************************************************** 
* Function Name: On_Mqtt_Disconnect
*******************************************************************************
*
* Summary:
*  This function calls when mqtt disconnects to server
*  
*
*  
*
* Parameters:  
*  None
*
* Return: 
*  None
*  
*******************************************************************************/
void On_Mqtt_Disconnect(AsyncMqttClientDisconnectReason reason) {
	Serial.println("Disconnected from MQTT.");
	
	/* if wifi is connected then reconnect to mqtt */
	if (WiFi.isConnected()) {
		xTimerStart(mqttReconnectTimer, 0);
	}

	ui8MqttStatus = 0;
}

/****************************************************************************** 
* Function Name: On_Mqtt_Subscribe
*******************************************************************************
*
* Summary:
*  This function calls when client subscribe to topic
*  
*
*  
*
* Parameters:  
*  None
*
* Return: 
*  None
*  
*******************************************************************************/
void On_Mqtt_Subscribe(uint16_t packetId, uint8_t qos) {
	uint16_t ui16LoopCounter=0;

	Serial.print("Mqtt subscribe pkt: ");
  	for(ui16LoopCounter=0;ui16LoopCounter<mqttClient.ui16DebugPacketCountLength;ui16LoopCounter++){
    	Serial.print(mqttClient.aui8DebugPacket[ui16LoopCounter],HEX);
    	Serial.print(" ");
  	}
  	Serial.println(" ");

	Serial.println("Subscribe acknowledged.");
	Serial.print("  packetId: ");
	Serial.println(packetId);
	Serial.print("  qos: ");
	Serial.println(qos);
}

/****************************************************************************** 
* Function Name: On_Mqtt_Unsubscribe
*******************************************************************************
*
* Summary:
*  This function calls when client unsubscribe to topic
*  
*
*  
*
* Parameters:  
*  None
*
* Return: 
*  None
*  
*******************************************************************************/
void On_Mqtt_Unsubscribe(uint16_t packetId) {
	uint16_t ui16LoopCounter=0;

	Serial.print("Mqtt unsubscribe pkt: ");
  	for(ui16LoopCounter=0;ui16LoopCounter<mqttClient.ui16DebugPacketCountLength;ui16LoopCounter++){
    	Serial.print(mqttClient.aui8DebugPacket[ui16LoopCounter],HEX);
    	Serial.print(" ");
  	}
  	Serial.println(" ");

	Serial.println("Unsubscribe acknowledged.");
	Serial.print("  packetId: ");
	Serial.println(packetId);
}


/****************************************************************************** 
* Function Name: On_Mqtt_Message
*******************************************************************************
*
* Summary:
*  This function calls when client received any message from server
*  
*
*  
*
* Parameters:  
*  None
*
* Return: 
*  None
*  
*******************************************************************************/
void On_Mqtt_Message(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total){

	uint16_t ui16LoopCounter=0;
	Serial.print("Mqtt onMessage pkt: ");
  	for(ui16LoopCounter=0;ui16LoopCounter<mqttClient.ui16DebugPacketCountLength;ui16LoopCounter++){
    	Serial.print(mqttClient.aui8DebugPacket[ui16LoopCounter],HEX);
    	Serial.print(" ");
  	}
  	Serial.println(" ");

	Serial.println("Publish received.");
	Serial.print("  topic: ");
	Serial.println(topic);
	Serial.print("  qos: ");
	Serial.println(properties.qos);
	Serial.print("  dup: ");
	Serial.println(properties.dup);
	Serial.print("  retain: ");
	Serial.println(properties.retain);
	Serial.print("  len: ");
	Serial.println(len);
	Serial.print("  index: ");
	Serial.println(index);
	Serial.print("  total: ");
	Serial.println(total);
	Serial.print("  payload: ");
	Serial.println(payload);
	

	/* check for topic received */
	if(strcmp(topic,sMqttLedTopic.c_str()) == 0){
		/* if payload is "on" then switch on led else switch off led */
        if(strcmp(payload,"on")){
			digitalWrite(LED_PIN,LOW);
		}else{
			digitalWrite(LED_PIN,HIGH);
		}
    }
}

/****************************************************************************** 
* Function Name: On_Mqtt_Publish
*******************************************************************************
*
* Summary:
*  This function calls on sucess of mqtt publish 
*  
*
*  
*
* Parameters:  
*  None
*
* Return: 
*  None
*  
*******************************************************************************/
void On_Mqtt_Publish(uint16_t packetId) {
	uint16_t ui16LoopCounter=0;

	Serial.print("Mqtt publish pkt: ");
  	for(ui16LoopCounter=0;ui16LoopCounter<mqttClient.ui16DebugPacketCountLength;ui16LoopCounter++){
    	Serial.print(mqttClient.aui8DebugPacket[ui16LoopCounter],HEX);
    	Serial.print(" ");
  	}
  	Serial.println(" ");

	Serial.println("Publish acknowledged.");
	Serial.print("  packetId: ");
	Serial.println(packetId);
	
}

/* function to read dht22 sensor */
uint8_t Read_DHT22(void){

	uint8_t ui8Response=0;

	/* read humidity */
	fHumidity = dht.readHumidity();

	/* read temperature */
	fTemperature = dht.readTemperature();

	/* check if data is valid */
	if(isnan(fHumidity) || isnan(fTemperature)){

		/* display read error in serial */
		fHeatIndex = 0;
		ui8Response = 0;
		Serial.println("DHT22 reading failure");

	}else{

		/* if data is valid read heat index */
		fHeatIndex = dht.computeHeatIndex(fHumidity, fTemperature, false);
		
		ui8Response = 1;

		/* display these values */
		Serial.printf("Temperature:%f degC Humidity:%f % Heat index:%f\n",fTemperature,fHumidity,fHeatIndex);
	}
	
	return ui8Response;
}