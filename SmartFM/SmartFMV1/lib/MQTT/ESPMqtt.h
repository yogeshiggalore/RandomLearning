#ifndef _HEADER_ESPMQTT_H_
#define _HEADER_ESPMQTT_H_

#include <Arduino.h>
#include <MqttFile.h>
#include <AsyncMqttClient.h>
#include <WiFi.h>

#define MQTT_LED_PIN   4

/* mqtt client instance */
extern AsyncMqttClient mqttClient;

/* timer handling for mqtt and wifi reconnection */
extern TimerHandle_t mqttReconnectTimer;
extern TimerHandle_t wifiReconnectTimer;

/* timer interval publishing data */
extern TimerHandle_t PublishIntervalTimer;


void ESPMqtt_Start(void);
void On_Mqtt_Connect(bool sessionPresent);
void On_Mqtt_Disconnect(AsyncMqttClientDisconnectReason reason);
void On_Mqtt_Subscribe(uint16_t packetId, uint8_t qos);
void On_Mqtt_Unsubscribe(uint16_t packetId);
void On_Mqtt_Message(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
void On_Mqtt_Publish(uint16_t packetId);
void WiFiEvent(WiFiEvent_t event);
void Check_Mqtt_Connection(void);
void Connect_To_Mqtt(void) ;
void Mqtt_Publish_Content(String sTopic, String sPayload, uint8_t ui8QoS);
#endif