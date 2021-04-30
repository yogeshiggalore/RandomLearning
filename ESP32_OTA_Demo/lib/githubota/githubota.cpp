/*******************************************************************************
*File Name: githubota.cpp
*
* Version: 1.0
*
* Description:
* In this source file for githubota application
*
*
* Owner:
* Yogesh M Iggalore
*
********************************************************************************
* Copyright (2021-22), RandomLearning
*******************************************************************************/

#include<Arduino.h>
#include<githubota.h>

// Constructors ////////////////////////////////////////////////////////////////
githubota::githubota(){}

// Preinstantiate Objects //////////////////////////////////////////////////////
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_GITHUBOTA)
githubota ghota;
#endif