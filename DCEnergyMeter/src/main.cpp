#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

Adafruit_INA219 ina219;

unsigned long previousMillis = 0;
unsigned long interval = 300;
const int chipSelect = 10;
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float energy = 0;

/* OLED display width, in pixels */
#define SCREEN_WIDTH 128 

/* OLED display height, in pixels */
#define SCREEN_HEIGHT 64 

/* Declaration for an SSD1306 display connected to I2C (SDA, SCL pins) */
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void displaydata(void);
void ina219values(void);

#define ACCESS_POINT_NAME     "myesp1"
#define ACCESS_POINT_PASSWORD "test1234"

uint8_t ui8MemoryLED1Value=0;

/* led web page index */
const char* htmlfile = "/modbusweb.html";

/* Static i config for server */
IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,0);
IPAddress subnet(255,255,255,0);

/* webserver object on port 80 */
ESP8266WebServer ESPServer(80);

/* ESP Server functions*/
void ESPServer_HandleWebRequests(void);
void ESPServer_ModbusUpdate(void);
void ESPServer_HandleRoot(void);
bool ESPServer_LoadFromSpiffs(String path);

/* timer and intruppt function for 1 milisecond */
void Timer_1ms_Start(void);
void ICACHE_RAM_ATTR timercallback1ms(void);

/* timer flags */
uint8_t  ui81msFlag=0;
uint64_t ui64OnemsTimeCounter=0;
long lLastTime=0;

void setup() {
	Wire.begin();
  	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    	Serial.println(F("SSD1306 allocation failed"));
    	for(;;);
 	}
  	ina219.begin();

	delay(5000);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    ina219values();
    displaydata();
  }
}

void displaydata() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(busvoltage);
  display.setCursor(75, 0);
  display.println("V");
  display.setCursor(0, 20);
  display.println(current_mA);
  display.setCursor(75, 20);
  display.println("mA");
  display.setCursor(0, 40);
  display.println(energy);
  display.setCursor(75, 40);
  display.println("mWh");
  /*display.setCursor(0, 60);
  display.println(loadvoltage * current_mA);
  display.setCursor(75, 60);
  display.println("mW");*/
  
  display.display();
}

void ina219values() {
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  energy = energy + loadvoltage * current_mA / 3600;
}




