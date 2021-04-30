/*******************************************************************************
* File Name: githubota.h
*
* Version: 1.0
*
* Description:
* This is header file for ESP githubota. All the functions related to
* githubota are here.
*
* Owner:
* Yogesh M Iggalore
*
* Code Tested With:
* 1. platformio and esp32
*
********************************************************************************
* Copyright (2020-21) , RandomLearning
********************************************************************************/
#ifndef githubota_h
#define githubota_h
#include <Arduino.h>

class githubota{
    public:
        githubota();
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_GITHUBOTA)
extern githubota ghota;
#endif
#endif