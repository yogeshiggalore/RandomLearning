/*******************************************************************************
*File Name: METITimer.cpp
*
* Version: 1.0
*
* Description:
* In this source code for handling 1ms time
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2019-20) , METI M2M India Pvt Ltd
*******************************************************************************/
#include <Arduino.h>
#include <METITimer.h>

uint8_t ui8Timer1msFlag;

// Constructors ////////////////////////////////////////////////////////////////
METITimer::METITimer(){}

void METITimer::Start(void){
    
}


// Preinstantiate Objects //////////////////////////////////////////////////////
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
METITimer MTimer;
#endif