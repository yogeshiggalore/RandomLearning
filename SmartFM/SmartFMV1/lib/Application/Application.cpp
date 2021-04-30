/*******************************************************************************
*File Name: Application.cpp
*
* Version: 1.0
*
* Description:
* In this source file for smart fm application
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2021-22)
*******************************************************************************/

#include<Arduino.h>
#include<Application.h>
#include<Wire.h>
#include <LogLevel.h>
#include <logger.h>

// Constructors ////////////////////////////////////////////////////////////////
Application::Application(void):fm(){}

void Application::start(void){
    Wire.begin();
    fm.setup(RESET_PIN, ESP32_I2C_SDA);
    elog.Write(LogLevel::Info,"app", "FM module started");
}

void Application::test_fm(void){
    elog.Write(LogLevel::Info,"app", "FM volume set to 15");
    fm.setVolume(15);
    elog.Write(LogLevel::Info,"app", "FM channel 92.7");
    fm.setFrequency(9270);
    fm.setRds(true);
    fm.setRdsMode(0); 
    fm.setSeekThreshold(30);
}

// Preinstantiate Objects //////////////////////////////////////////////////////
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_APPLICATION)
Application app;
#endif