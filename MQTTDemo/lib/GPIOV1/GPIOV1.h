/*******************************************************************************
*File Name: GPIOV1.h
*
* Version: 1.0
*
* Description:
* In this header used for gpio functions
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/
#ifndef GPIOV1_h
#define GPIOV1_h

#include <Arduino.h>
#include <ESP8266WiFi.h>

#define LED_PIN_2 2

class GPIOV1{
public:
  uint8_t ui8GPIOPin=0;
  uint8_t ui8GPIOType=0;
  uint8_t ui8GPIOInverse=0;
  uint8_t ui8OffCode=0;
  uint8_t ui8OnCode=0;
  uint8_t ui8PinStatus=0;
  uint16_t ui16GPIOOntime=0;
  uint16_t ui16GPIOOfftime=0;
  uint16_t ui16GPIORepeatCount=0;
  uint8_t  ui8GPIOStartFlag=0;
  uint16_t ui16GPIOOnCounter=0;
  uint16_t ui16GPIOOffCounter=0;
  uint16_t ui16GPIORepeatCounter=0;
  GPIOV1();
  void Start(uint8_t ui8GPIO,uint8_t ui8Type,uint8_t ui8Inverse);
  void On();
  void Off();
  void Toggle(void);
  void Set(uint16_t ui16On, uint16_t ui16Off, uint16_t ui16Count);
  void CheckTimeout(void);
  uint8_t Get_PinNumber(void);
  uint8_t Get_PinType(void);
  uint8_t Get_Inverse(void);
  uint8_t Get_Status(void);
  uint16_t Get_Ontime(void);
  uint16_t Get_Offtime(void);
  uint16_t Get_RepeatCount(void);
  uint16_t Get_CurrentCount(void);
  uint16_t Get_RemainCount(void);
  uint8_t Get_TaskStatus(void);
  void Read(void);
};
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
extern GPIOV1 MGPIO;
#endif
#endif
