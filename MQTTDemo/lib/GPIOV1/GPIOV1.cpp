/*******************************************************************************
*File Name: GPIOV1.c
*
* Version: 1.0
*
* Description:
* In this source code for GPIO usage
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/

#include <GPIOV1.h>

// Constructors ////////////////////////////////////////////////////////////////
GPIOV1::GPIOV1(){}

void GPIOV1::Start(uint8_t ui8GPIO,uint8_t ui8Type,uint8_t ui8Inverse){
    ui8GPIOPin = ui8GPIO;
    ui8GPIOType = ui8Type;
    ui8GPIOInverse = ui8Inverse;
    if(ui8Inverse){
      ui8OffCode = HIGH;
      ui8OnCode = LOW;
    }else{
      ui8OffCode = LOW;
      ui8OnCode = HIGH;
    }
    pinMode(ui8GPIO, ui8Type);
}

void GPIOV1::On(){
    digitalWrite(ui8GPIOPin,ui8OnCode);
    ui8PinStatus = HIGH;
}

void GPIOV1::Toggle(void){
    if(ui8PinStatus){
        Off();
    }else{
      On();
    }
}

void GPIOV1::Off(){
    digitalWrite(ui8GPIOPin,ui8OffCode);
    ui8PinStatus = LOW;
}

void GPIOV1::Read(void){
    ui8PinStatus = digitalRead(ui8GPIOPin);
}

void GPIOV1::Set(uint16_t ui16On, uint16_t ui16Off, uint16_t ui16Count){
    ui16GPIOOntime = ui16On;
    ui16GPIOOfftime = ui16Off;
    ui16GPIORepeatCount = ui16Count;
    ui8GPIOStartFlag = 1;
    ui16GPIOOnCounter=0;
    ui16GPIOOffCounter=0;
    ui16GPIORepeatCounter=0;
    this->On();
}

void GPIOV1::CheckTimeout(void){
    if(ui8GPIOType != OUTPUT){
        this->Read();
    }

    if(ui8GPIOStartFlag){
      if(ui8PinStatus){
          ui16GPIOOnCounter++;
      }else{
          ui16GPIOOffCounter++;
      }

      if(ui16GPIOOnCounter >= ui16GPIOOntime){
        ui16GPIOOnCounter = 0;
        this->Off();
      }

      if(ui16GPIOOffCounter >= ui16GPIOOfftime){
        ui16GPIOOffCounter = 0;
        ui16GPIORepeatCounter++;
        if(ui16GPIORepeatCounter > ui16GPIORepeatCount){
          ui16GPIORepeatCounter=0;
          ui8GPIOStartFlag=0;
          this->Off();
        }else{
          this->On();
        }
      }
    }
}

uint8_t GPIOV1::Get_PinNumber(void){
  return ui8GPIOPin;
}

uint8_t GPIOV1::Get_PinType(void){
  return ui8GPIOType;
}

uint8_t GPIOV1::Get_Inverse(void){
  return ui8GPIOInverse;
}

uint8_t GPIOV1::Get_Status(void){
  return ui8PinStatus;
}

uint16_t GPIOV1::Get_Ontime(void){
  return ui16GPIOOntime;
}

uint16_t GPIOV1::Get_Offtime(void){
  return ui16GPIOOfftime;
}

uint16_t GPIOV1::Get_RepeatCount(void){
  return ui16GPIORepeatCount;
}

uint16_t GPIOV1::Get_CurrentCount(void){
  return ui16GPIORepeatCounter;
}

uint16_t GPIOV1::Get_RemainCount(void){
  return ui16GPIORepeatCount - ui16GPIORepeatCounter;
}

uint8_t GPIOV1::Get_TaskStatus(void){
  return ui8GPIOStartFlag;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
GPIOV1 MGPIO;
#endif
