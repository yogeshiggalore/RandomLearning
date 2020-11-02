/*******************************************************************************
*File Name: METIUtils.h
*
* Version: 1.0
*
* Description:
* In this header for utility functions
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/
#include <METIUtils.h>
#include <WiFiUdp.h>
#include <NTPClient.h>


METIUtils::METIUtils(void):ntpUDP(),timeClient(ntpUDP, "in.pool.ntp.org", 19800, 60000){
}

void METIUtils::Start(void){
    //Serial1.begin(250000);
    //Serial1.setDebugOutput(false);
    timeClient.begin();
}

String METIUtils::GetTime(void){
    timeClient.update();

    unsigned long rawTime = timeClient.getEpochTime();

    unsigned long hours = (rawTime % 86400L) / 3600;
    String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

    unsigned long minutes = (rawTime % 3600) / 60;
    String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

    unsigned long seconds = rawTime % 60;
    String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

    String dateStr = String(Get_Date());

    String monthStr = String(Get_Month());

    String yearStr = String(Get_Year());

    return hoursStr + ":" + minuteStr + ":" + secondStr + " " + dateStr + "-" + monthStr + "-" + yearStr;
}

int METIUtils::Get_Day(void) {
  return (((timeClient.getEpochTime()  / 86400L) + 4 ) % 7); //0 is Sunday
}

int METIUtils::Get_Hours(void) {
  return ((timeClient.getEpochTime()  % 86400L) / 3600);
}
int METIUtils::Get_Minutes(void) {
  return ((timeClient.getEpochTime() % 3600) / 60);
}
int METIUtils::Get_Seconds(void) {
  return (timeClient.getEpochTime() % 60);
}

int METIUtils::Get_Year(void) {
  time_t rawtime = timeClient.getEpochTime();
  struct tm * ti;
  ti = localtime (&rawtime);
  int year = ti->tm_year + 1900;

  return year;
}

int METIUtils::Get_Month(void) {
  time_t rawtime = timeClient.getEpochTime();
  struct tm * ti;
  ti = localtime (&rawtime);
  int month = (ti->tm_mon + 1) < 10 ? 0 + (ti->tm_mon + 1) : (ti->tm_mon + 1);

  return month;
}

int METIUtils::Get_Date(void) {
  time_t rawtime = timeClient.getEpochTime();
  struct tm * ti;
  ti = localtime (&rawtime);
  int month = (ti->tm_mday) < 10 ? 0 + (ti->tm_mday) : (ti->tm_mday);

  return month;
}

String METIUtils::Get_MAC(void){
  uint8_t ui8LoopCounter=0;
  uint8_t ui8Length=0;

  sMACAddress = WiFi.macAddress();
  ui8Length = sMACAddress.length();
  for(ui8LoopCounter=0;ui8LoopCounter<ui8Length;ui8LoopCounter++){
    if(sMACAddress[ui8LoopCounter] == ':'){
      sMACAddress.remove(ui8LoopCounter,1);
    }
  }
  return sMACAddress;
}

byte METIUtils::DecToBcd(byte bValue){
  return( (bValue/10*16) + (bValue%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte METIUtils::BcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}

String METIUtils::Get_Device_Name(void){
  sDeviceName = "SS01" + Get_MAC();
  return sDeviceName;
}

void METIUtils::Set_Host_Name(void){
  sHostNameWifi = Get_Device_Name();
  sHostNameWifi.concat(".local");
  WiFi.hostname(sHostNameWifi);
}

/******************************************************************************
* Function Name: Convert_ASCIIHex_To_Hex
*******************************************************************************
*
* Summary:
*  This function convert the asciihex to hex
*
*
* Parameters:
*  ascii hex string
*
* Return:
* returns hex
*
*******************************************************************************/
uint8_t METIUtils::Convert_ASCIIHex_To_Hex(char sASCIIHex[]){
    char cCharLowerNibble=0;
    char cCharHigherNibble=0;
    uint8_t ui8HexByte=0;

    cCharHigherNibble = sASCIIHex[0];
    cCharLowerNibble = sASCIIHex[1];

    if(cCharHigherNibble > '9'){
        cCharHigherNibble = cCharHigherNibble - 7;
    }
    cCharHigherNibble = cCharHigherNibble - '0';

    ui8HexByte = cCharHigherNibble;
    ui8HexByte = ui8HexByte << 4;

    if(cCharLowerNibble > '9'){
        cCharLowerNibble = cCharLowerNibble - 7;
    }
    cCharLowerNibble = cCharLowerNibble - '0';

    ui8HexByte = ui8HexByte | cCharLowerNibble;

    return ui8HexByte;
}

void METIUtils::Parse_String_To_Bytes(const char* str, char sep, uint8_t* bytes, int maxBytes, int base) {
    for (int i = 0; i < maxBytes; i++) {
        bytes[i] = strtoul(str, NULL, base);  // Convert byte
        str = strchr(str, sep);               // Find next separator
        if (str == NULL || *str == '\0') {
            break;                            // No more separators, exit
        }
        str++;                                // Point to next character after separator
    }
}


void METIUtils::Enable_Serial_Debug(void){
    ui8DebugEnable = 1;
}

void METIUtils::Disable_Serial_Debug(void){
    ui8DebugEnable = 0;
}

uint8_t METIUtils::Get_Serial_Debug(void){
    return ui8DebugEnable;
}

size_t METIUtils::printf(const char *format, ...) {
    if(ui8DebugEnable){
        return Serial1.printf(format);
    }return 0;
}

size_t METIUtils::printf_P(PGM_P format, ...) {
    if(ui8DebugEnable){
        return Serial1.printf_P(format);
    }return 0;
}

size_t METIUtils::print(const __FlashStringHelper *ifsh) {
    if(ui8DebugEnable){
        return Serial1.print(ifsh);
    }return 0;
}

size_t METIUtils::print(const String &s) {
    if(ui8DebugEnable){
        return Serial1.print(s);
    }return 0;
}

size_t METIUtils::print(const char str[]) {
    if(ui8DebugEnable){
        return Serial1.print(str);
    }return 0;
}

size_t METIUtils::print(char c) {
    if(ui8DebugEnable){
        return Serial1.print(c);
    }return 0;
}

size_t METIUtils::print(unsigned char b, int base) {
    if(ui8DebugEnable){
        return Serial1.print(b,base);
    }return 0;
}

size_t METIUtils::print(int n, int base) {
    if(ui8DebugEnable){
        return Serial1.print(n,base);
    }return 0;
}

size_t METIUtils::print(unsigned int n, int base) {
    if(ui8DebugEnable){
        return Serial1.print(n,base);
    }return 0;
}

size_t METIUtils::print(long n, int base) {
    if(ui8DebugEnable){
        return Serial1.print(n,base);
    }return 0;
}

size_t METIUtils::print(unsigned long n, int base) {
    if(ui8DebugEnable){
        return Serial1.print(n,base);
    }return 0;
}

size_t METIUtils::print(double n, int digits) {
    if(ui8DebugEnable){
        return Serial1.print(n,digits);
    }return 0;
}

size_t METIUtils::println(const __FlashStringHelper *ifsh) {

    if(ui8DebugEnable){
        return Serial1.print(ifsh);
    }return 0;
}

size_t METIUtils::print(const Printable& x) {
    if(ui8DebugEnable){
        return Serial1.print(x);
    }
    return 0;

}

size_t METIUtils::println(void) {
    if(ui8DebugEnable){
        return Serial1.println("\r\n");
    }
    return 0;

}

size_t METIUtils::println(const String &s) {
    if(ui8DebugEnable){
        return Serial1.println(s);
    }
    return 0;
}

size_t METIUtils::println(const char c[]) {
    if(ui8DebugEnable){
        return Serial1.println(c);
    }
    return 0;
}

size_t METIUtils::println(char c) {
    if(ui8DebugEnable){
        return Serial1.println(c);
    }
    return 0;
}

size_t METIUtils::println(unsigned char b, int base) {
    if(ui8DebugEnable){
        return Serial1.println(b, base);
    }
    return 0;
}

size_t METIUtils::println(int num, int base) {
    if(ui8DebugEnable){
        return Serial1.println(num, base);
    }
    return 0;
}

size_t METIUtils::println(unsigned int num, int base) {
    if(ui8DebugEnable){
        return Serial1.println(num, base);
    }
    return 0;
}

size_t METIUtils::println(long num, int base) {
    if(ui8DebugEnable){
        return Serial1.println(num, base);
    }
    return 0;
}

size_t METIUtils::println(unsigned long num, int base) {
    if(ui8DebugEnable){
        return Serial1.println(num, base);
    }
    return 0;
}

size_t METIUtils::println(double num, int digits) {
    if(ui8DebugEnable){
        return Serial1.print(num, digits);
    }return 0;
}

size_t METIUtils::println(const Printable& x) {
    if(ui8DebugEnable){
        return Serial1.println(x);
    }return 0;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
METIUtils MUtils;
#endif
