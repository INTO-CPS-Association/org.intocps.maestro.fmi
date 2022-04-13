/*
 * This file is part of the INTO-CPS toolchain.
 *
 * Copyright (c) 2017-CurrentYear, INTO-CPS Association,
 * c/o Professor Peter Gorm Larsen, Department of Engineering
 * Finlandsgade 22, 8200 Aarhus N.
 *
 * All rights reserved.
 *
 * THIS PROGRAM IS PROVIDED UNDER THE TERMS OF GPL VERSION 3 LICENSE OR
 * THIS INTO-CPS ASSOCIATION PUBLIC LICENSE VERSION 1.0.
 * ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS PROGRAM CONSTITUTES
 * RECIPIENT'S ACCEPTANCE OF THE OSMC PUBLIC LICENSE OR THE GPL
 * VERSION 3, ACCORDING TO RECIPIENTS CHOICE.
 *
 * The INTO-CPS toolchain  and the INTO-CPS Association Public License
 * are obtained from the INTO-CPS Association, either from the above address,
 * from the URLs: http://www.into-cps.org, and in the INTO-CPS toolchain
 * distribution. GNU version 3 is obtained from:
 * http://www.gnu.org/copyleft/gpl.html.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without
 * even the implied warranty of  MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE, EXCEPT AS EXPRESSLY SET FORTH IN THE
 * BY RECIPIENT SELECTED SUBSIDIARY LICENSE CONDITIONS OF
 * THE INTO-CPS ASSOCIATION.
 *
 * See the full INTO-CPS Association Public License conditions for more details.
 */

/*
 * Author:
 *		Kenneth Lausdahl
 */

#include "sim_support.h"

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fmi2.h"

static void *getAdrOptional(int *success, HMODULE dllHandle,
                            const char *functionName, bool optional) {
#ifdef _WIN32
  void *fp = (void *)GetProcAddress(dllHandle, functionName);
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_MAC
  // Other kinds of Mac OS
  void *fp = (void *)dlsym(dllHandle, functionName);
#else
// Unsupported platform
#endif
#elif __linux
  // linux
  void *fp = (void *)dlsym(dllHandle, functionName);
#endif

  if (!fp) {
    if (!optional) {
      printf("warning: Function %s not found in library\n", functionName);
      *success = 0;
    }
  }
  return fp;
}

static void *getAdr(int *success, HMODULE dllHandle, const char *functionName) {
  return getAdrOptional(success, dllHandle, functionName, false);
}

// Load the given dll and set function pointers in fmu
// Return 0 to indicate failure
int loadDll(const char *dllPath, FMU *fmu) {
  int x = 1, s = 1;

#ifdef _WIN32
  HMODULE h = LoadLibraryEx(
      dllPath, NULL,
      LOAD_WITH_ALTERED_SEARCH_PATH);  // LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_MAC
  // Other kinds of Mac OS
  HMODULE h = dlopen(dllPath, RTLD_LAZY | RTLD_LOCAL | RTLD_FIRST);
#else
// Unsupported platform
#endif
#elif __linux
  // linux
  HMODULE h = dlopen(dllPath, RTLD_LAZY | RTLD_LOCAL | RTLD_DEEPBIND);
#endif

  if (!h) {
    printf("error: Could not load %s\n", dllPath);

#ifdef _WIN32
// TODO figure out how to format GetLastError() in C
#elif __APPLE__
#if TARGET_OS_MAC
    // Other kinds of Mac OS
    fputs(dlerror(), stderr);
    fflush(stderr);
#else
// Unsupported platform
#endif
#elif __linux
    // linux
    fputs(dlerror(), stderr);
    fflush(stderr);
#endif
    return 0;  // failure
  }
  fmu->dllHandle = h;
  fmu->getTypesPlatform =
      (fmi2GetTypesPlatformTYPE *)getAdr(&s, h, "fmi2GetTypesPlatform");
  fmu->getVersion = (fmi2GetVersionTYPE *)getAdr(&s, h, "fmi2GetVersion");
  fmu->setDebugLogging =
      (fmi2SetDebugLoggingTYPE *)getAdr(&s, h, "fmi2SetDebugLogging");
  fmu->instantiate = (fmi2InstantiateTYPE *)getAdr(&s, h, "fmi2Instantiate");
  fmu->freeInstance = (fmi2FreeInstanceTYPE *)getAdr(&s, h, "fmi2FreeInstance");
  fmu->setupExperiment =
      (fmi2SetupExperimentTYPE *)getAdr(&s, h, "fmi2SetupExperiment");
  fmu->enterInitializationMode = (fmi2EnterInitializationModeTYPE *)getAdr(
      &s, h, "fmi2EnterInitializationMode");
  fmu->exitInitializationMode = (fmi2ExitInitializationModeTYPE *)getAdr(
      &s, h, "fmi2ExitInitializationMode");
  fmu->terminate = (fmi2TerminateTYPE *)getAdr(&s, h, "fmi2Terminate");
  fmu->reset = (fmi2ResetTYPE *)getAdr(&s, h, "fmi2Reset");
  fmu->getReal = (fmi2GetRealTYPE *)getAdr(&s, h, "fmi2GetReal");
  fmu->getInteger = (fmi2GetIntegerTYPE *)getAdr(&s, h, "fmi2GetInteger");
  fmu->getBoolean = (fmi2GetBooleanTYPE *)getAdr(&s, h, "fmi2GetBoolean");
  fmu->getString = (fmi2GetStringTYPE *)getAdr(&s, h, "fmi2GetString");
  fmu->setReal = (fmi2SetRealTYPE *)getAdr(&s, h, "fmi2SetReal");
  fmu->setInteger = (fmi2SetIntegerTYPE *)getAdr(&s, h, "fmi2SetInteger");
  fmu->setBoolean = (fmi2SetBooleanTYPE *)getAdr(&s, h, "fmi2SetBoolean");
  fmu->setString = (fmi2SetStringTYPE *)getAdr(&s, h, "fmi2SetString");
  fmu->getFMUstate = (fmi2GetFMUstateTYPE *)getAdr(&s, h, "fmi2GetFMUstate");
  fmu->setFMUstate = (fmi2SetFMUstateTYPE *)getAdr(&s, h, "fmi2SetFMUstate");
  fmu->freeFMUstate = (fmi2FreeFMUstateTYPE *)getAdr(&s, h, "fmi2FreeFMUstate");
  fmu->serializedFMUstateSize = (fmi2SerializedFMUstateSizeTYPE *)getAdr(
      &s, h, "fmi2SerializedFMUstateSize");
  fmu->serializeFMUstate =
      (fmi2SerializeFMUstateTYPE *)getAdr(&s, h, "fmi2SerializeFMUstate");
  fmu->deSerializeFMUstate =
      (fmi2DeSerializeFMUstateTYPE *)getAdr(&s, h, "fmi2DeSerializeFMUstate");
  fmu->getDirectionalDerivative = (fmi2GetDirectionalDerivativeTYPE *)getAdr(
      &s, h, "fmi2GetDirectionalDerivative");
#ifdef FMI_COSIMULATION
  fmu->setRealInputDerivatives = (fmi2SetRealInputDerivativesTYPE *)getAdr(
      &s, h, "fmi2SetRealInputDerivatives");
  fmu->getRealOutputDerivatives = (fmi2GetRealOutputDerivativesTYPE *)getAdr(
      &s, h, "fmi2GetRealOutputDerivatives");
  fmu->doStep = (fmi2DoStepTYPE *)getAdr(&s, h, "fmi2DoStep");
  fmu->cancelStep = (fmi2CancelStepTYPE *)getAdr(&s, h, "fmi2CancelStep");
  fmu->getStatus = (fmi2GetStatusTYPE *)getAdr(&s, h, "fmi2GetStatus");
  fmu->getRealStatus =
      (fmi2GetRealStatusTYPE *)getAdr(&s, h, "fmi2GetRealStatus");
  fmu->getIntegerStatus =
      (fmi2GetIntegerStatusTYPE *)getAdr(&s, h, "fmi2GetIntegerStatus");
  fmu->getBooleanStatus =
      (fmi2GetBooleanStatusTYPE *)getAdr(&s, h, "fmi2GetBooleanStatus");
  fmu->getStringStatus =
      (fmi2GetStringStatusTYPE *)getAdr(&s, h, "fmi2GetStringStatus");

  fmu->getMaxStepsize = (fmi2GetMaxStepsizeTYPE *)getAdrOptional(
      &s, h, " fmiGetMaxStepSize", true);
  if (fmu->getMaxStepsize == NULL) {
    fmu->getMaxStepsize = (fmi2GetMaxStepsizeTYPE *)getAdrOptional(
        &s, h, "fmi2GetMaxStepsize", true);
  }

#else  // FMI2 for Model Exchange
  fmu->enterEventMode =
      (fmi2EnterEventModeTYPE *)getAdr(&s, h, "fmi2EnterEventMode");
  fmu->newDiscreteStates =
      (fmi2NewDiscreteStatesTYPE *)getAdr(&s, h, "fmi2NewDiscreteStates");
  fmu->enterContinuousTimeMode = (fmi2EnterContinuousTimeModeTYPE *)getAdr(
      &s, h, "fmi2EnterContinuousTimeMode");
  fmu->completedIntegratorStep = (fmi2CompletedIntegratorStepTYPE *)getAdr(
      &s, h, "fmi2CompletedIntegratorStep");
  fmu->setTime = (fmi2SetTimeTYPE *)getAdr(&s, h, "fmi2SetTime");
  fmu->setContinuousStates =
      (fmi2SetContinuousStatesTYPE *)getAdr(&s, h, "fmi2SetContinuousStates");
  fmu->getDerivatives =
      (fmi2GetDerivativesTYPE *)getAdr(&s, h, "fmi2GetDerivatives");
  fmu->getEventIndicators =
      (fmi2GetEventIndicatorsTYPE *)getAdr(&s, h, "fmi2GetEventIndicators");
  fmu->getContinuousStates =
      (fmi2GetContinuousStatesTYPE *)getAdr(&s, h, "fmi2GetContinuousStates");
  fmu->getNominalsOfContinuousStates =
      (fmi2GetNominalsOfContinuousStatesTYPE *)getAdr(
          &s, h, "fmi2GetNominalsOfContinuousStates");
#endif

  if (fmu->getVersion == NULL && fmu->instantiate == NULL) {
    printf("warning: Functions from FMI 2.0 could not be found in %s\n",
           dllPath);
    printf("warning: Simulator will look for FMI 2.0 RC1 functions names...\n");
    fmu->getTypesPlatform =
        (fmi2GetTypesPlatformTYPE *)getAdr(&s, h, "fmiGetTypesPlatform");
    fmu->getVersion = (fmi2GetVersionTYPE *)getAdr(&s, h, "fmiGetVersion");
    fmu->setDebugLogging =
        (fmi2SetDebugLoggingTYPE *)getAdr(&s, h, "fmiSetDebugLogging");
    fmu->instantiate = (fmi2InstantiateTYPE *)getAdr(&s, h, "fmiInstantiate");
    fmu->freeInstance =
        (fmi2FreeInstanceTYPE *)getAdr(&s, h, "fmiFreeInstance");
    fmu->setupExperiment =
        (fmi2SetupExperimentTYPE *)getAdr(&s, h, "fmiSetupExperiment");
    fmu->enterInitializationMode = (fmi2EnterInitializationModeTYPE *)getAdr(
        &s, h, "fmiEnterInitializationMode");
    fmu->exitInitializationMode = (fmi2ExitInitializationModeTYPE *)getAdr(
        &s, h, "fmiExitInitializationMode");
    fmu->terminate = (fmi2TerminateTYPE *)getAdr(&s, h, "fmiTerminate");
    fmu->reset = (fmi2ResetTYPE *)getAdr(&s, h, "fmiReset");
    fmu->getReal = (fmi2GetRealTYPE *)getAdr(&s, h, "fmiGetReal");
    fmu->getInteger = (fmi2GetIntegerTYPE *)getAdr(&s, h, "fmiGetInteger");
    fmu->getBoolean = (fmi2GetBooleanTYPE *)getAdr(&s, h, "fmiGetBoolean");
    fmu->getString = (fmi2GetStringTYPE *)getAdr(&s, h, "fmiGetString");
    fmu->setReal = (fmi2SetRealTYPE *)getAdr(&s, h, "fmiSetReal");
    fmu->setInteger = (fmi2SetIntegerTYPE *)getAdr(&s, h, "fmiSetInteger");
    fmu->setBoolean = (fmi2SetBooleanTYPE *)getAdr(&s, h, "fmiSetBoolean");
    fmu->setString = (fmi2SetStringTYPE *)getAdr(&s, h, "fmiSetString");
    fmu->getFMUstate = (fmi2GetFMUstateTYPE *)getAdr(&s, h, "fmiGetFMUstate");
    fmu->setFMUstate = (fmi2SetFMUstateTYPE *)getAdr(&s, h, "fmiSetFMUstate");
    fmu->freeFMUstate =
        (fmi2FreeFMUstateTYPE *)getAdr(&s, h, "fmiFreeFMUstate");
    fmu->serializedFMUstateSize = (fmi2SerializedFMUstateSizeTYPE *)getAdr(
        &s, h, "fmiSerializedFMUstateSize");
    fmu->serializeFMUstate =
        (fmi2SerializeFMUstateTYPE *)getAdr(&s, h, "fmiSerializeFMUstate");
    fmu->deSerializeFMUstate =
        (fmi2DeSerializeFMUstateTYPE *)getAdr(&s, h, "fmiDeSerializeFMUstate");
    fmu->getDirectionalDerivative = (fmi2GetDirectionalDerivativeTYPE *)getAdr(
        &s, h, "fmiGetDirectionalDerivative");
#ifdef FMI_COSIMULATION
    fmu->setRealInputDerivatives = (fmi2SetRealInputDerivativesTYPE *)getAdr(
        &s, h, "fmiSetRealInputDerivatives");
    fmu->getRealOutputDerivatives = (fmi2GetRealOutputDerivativesTYPE *)getAdr(
        &s, h, "fmiGetRealOutputDerivatives");
    fmu->doStep = (fmi2DoStepTYPE *)getAdr(&s, h, "fmiDoStep");
    fmu->cancelStep = (fmi2CancelStepTYPE *)getAdr(&s, h, "fmiCancelStep");
    fmu->getStatus = (fmi2GetStatusTYPE *)getAdr(&s, h, "fmiGetStatus");
    fmu->getRealStatus =
        (fmi2GetRealStatusTYPE *)getAdr(&s, h, "fmiGetRealStatus");
    fmu->getIntegerStatus =
        (fmi2GetIntegerStatusTYPE *)getAdr(&s, h, "fmiGetIntegerStatus");
    fmu->getBooleanStatus =
        (fmi2GetBooleanStatusTYPE *)getAdr(&s, h, "fmiGetBooleanStatus");
    fmu->getStringStatus =
        (fmi2GetStringStatusTYPE *)getAdr(&s, h, "fmiGetStringStatus");
#else  // FMI2 for Model Exchange
    fmu->enterEventMode =
        (fmi2EnterEventModeTYPE *)getAdr(&s, h, "fmiEnterEventMode");
    fmu->newDiscreteStates =
        (fmi2NewDiscreteStatesTYPE *)getAdr(&s, h, "fmiNewDiscreteStates");
    fmu->enterContinuousTimeMode = (fmi2EnterContinuousTimeModeTYPE *)getAdr(
        &s, h, "fmiEnterContinuousTimeMode");
    fmu->completedIntegratorStep = (fmi2CompletedIntegratorStepTYPE *)getAdr(
        &s, h, "fmiCompletedIntegratorStep");
    fmu->setTime = (fmi2SetTimeTYPE *)getAdr(&s, h, "fmiSetTime");
    fmu->setContinuousStates =
        (fmi2SetContinuousStatesTYPE *)getAdr(&s, h, "fmiSetContinuousStates");
    fmu->getDerivatives =
        (fmi2GetDerivativesTYPE *)getAdr(&s, h, "fmiGetDerivatives");
    fmu->getEventIndicators =
        (fmi2GetEventIndicatorsTYPE *)getAdr(&s, h, "fmiGetEventIndicators");
    fmu->getContinuousStates =
        (fmi2GetContinuousStatesTYPE *)getAdr(&s, h, "fmiGetContinuousStates");
    fmu->getNominalsOfContinuousStates =
        (fmi2GetNominalsOfContinuousStatesTYPE *)getAdr(
            &s, h, "fmiGetNominalsOfContinuousStates");
#endif
  }
  return s;
}

static const char *fmi2StatusToString(fmi2Status status) {
  switch (status) {
    case fmi2OK:
      return "ok";
    case fmi2Warning:
      return "warning";
    case fmi2Discard:
      return "discard";
    case fmi2Error:
      return "error";
    case fmi2Fatal:
      return "fatal";
#ifdef FMI_COSIMULATION
    case fmi2Pending:
      return "fmi2Pending";
#endif
    default:
      return "?";
  }
}

#define MAX_MSG_SIZE 1000
void fmuLogger(void *componentEnvironment, fmi2String instanceName,
               fmi2Status status, fmi2String category, fmi2String message,
               ...) {
  char msg[MAX_MSG_SIZE];
  char *copy;
  va_list argp;

  // replace C format strings
  va_start(argp, message);
  vsprintf(msg, message, argp);
  va_end(argp);

  // replace e.g. ## and #r12#
  copy = strdup(msg);
  // FIXME replaceRefsInMessage(copy, msg, MAX_MSG_SIZE, &fmu);
  free(copy);

  // print the final message
  if (!instanceName) instanceName = "?";
  if (!category) category = "?";
  printf("%s %s (%s): %s\n", fmi2StatusToString(status), instanceName, category,
         msg);
}

int error(const char *message) {
  printf("%s\n", message);
  return 0;
}
