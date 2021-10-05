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

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fmi3.h"
#include "sim_support3.h"

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
int loadDll3(const char *dllPath, FMU3 *fmu) {
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

  fmu->fmi3GetVersion = (fmi3GetVersionTYPE *)getAdr(&s, h, "fmi3GetVersion");
  fmu->fmi3SetDebugLogging =
      (fmi3SetDebugLoggingTYPE *)getAdr(&s, h, "fmi3SetDebugLogging");
  fmu->fmi3InstantiateModelExchange =
      (fmi3InstantiateModelExchangeTYPE *)getAdr(
          &s, h, "fmi3InstantiateModelExchange");
  fmu->fmi3InstantiateCoSimulation = (fmi3InstantiateCoSimulationTYPE *)getAdr(
      &s, h, "fmi3InstantiateCoSimulation");
  fmu->fmi3InstantiateScheduledExecution =
      (fmi3InstantiateScheduledExecutionTYPE *)getAdr(
          &s, h, "fmi3InstantiateScheduledExecution");
  fmu->fmi3FreeInstance =
      (fmi3FreeInstanceTYPE *)getAdr(&s, h, "fmi3FreeInstance");
  fmu->fmi3EnterInitializationMode = (fmi3EnterInitializationModeTYPE *)getAdr(
      &s, h, "fmi3EnterInitializationMode");
  fmu->fmi3ExitInitializationMode = (fmi3ExitInitializationModeTYPE *)getAdr(
      &s, h, "fmi3ExitInitializationMode");
  fmu->fmi3EnterEventMode =
      (fmi3EnterEventModeTYPE *)getAdr(&s, h, "fmi3EnterEventMode");
  fmu->fmi3Terminate = (fmi3TerminateTYPE *)getAdr(&s, h, "fmi3Terminate");
  fmu->fmi3Reset = (fmi3ResetTYPE *)getAdr(&s, h, "fmi3Reset");
  fmu->fmi3GetFloat32 = (fmi3GetFloat32TYPE *)getAdr(&s, h, "fmi3GetFloat32");
  fmu->fmi3GetFloat64 = (fmi3GetFloat64TYPE *)getAdr(&s, h, "fmi3GetFloat64");
  fmu->fmi3GetInt8 = (fmi3GetInt8TYPE *)getAdr(&s, h, "fmi3GetInt8");
  fmu->fmi3GetUInt8 = (fmi3GetUInt8TYPE *)getAdr(&s, h, "fmi3GetUInt8");
  fmu->fmi3GetInt16 = (fmi3GetInt16TYPE *)getAdr(&s, h, "fmi3GetInt16");
  fmu->fmi3GetUInt16 = (fmi3GetUInt16TYPE *)getAdr(&s, h, "fmi3GetUInt16");
  fmu->fmi3GetInt32 = (fmi3GetInt32TYPE *)getAdr(&s, h, "fmi3GetInt32");
  fmu->fmi3GetUInt32 = (fmi3GetUInt32TYPE *)getAdr(&s, h, "fmi3GetUInt32");
  fmu->fmi3GetInt64 = (fmi3GetInt64TYPE *)getAdr(&s, h, "fmi3GetInt64");
  fmu->fmi3GetUInt64 = (fmi3GetUInt64TYPE *)getAdr(&s, h, "fmi3GetUInt64");
  fmu->fmi3GetBoolean = (fmi3GetBooleanTYPE *)getAdr(&s, h, "fmi3GetBoolean");
  fmu->fmi3GetString = (fmi3GetStringTYPE *)getAdr(&s, h, "fmi3GetString");
  fmu->fmi3GetBinary = (fmi3GetBinaryTYPE *)getAdr(&s, h, "fmi3GetBinary");
  fmu->fmi3SetFloat32 = (fmi3SetFloat32TYPE *)getAdr(&s, h, "fmi3SetFloat32");
  fmu->fmi3SetFloat64 = (fmi3SetFloat64TYPE *)getAdr(&s, h, "fmi3SetFloat64");
  fmu->fmi3SetInt8 = (fmi3SetInt8TYPE *)getAdr(&s, h, "fmi3SetInt8");
  fmu->fmi3SetUInt8 = (fmi3SetUInt8TYPE *)getAdr(&s, h, "fmi3SetUInt8");
  fmu->fmi3SetInt16 = (fmi3SetInt16TYPE *)getAdr(&s, h, "fmi3SetInt16");
  fmu->fmi3SetUInt16 = (fmi3SetUInt16TYPE *)getAdr(&s, h, "fmi3SetUInt16");
  fmu->fmi3SetInt32 = (fmi3SetInt32TYPE *)getAdr(&s, h, "fmi3SetInt32");
  fmu->fmi3SetUInt32 = (fmi3SetUInt32TYPE *)getAdr(&s, h, "fmi3SetUInt32");
  fmu->fmi3SetInt64 = (fmi3SetInt64TYPE *)getAdr(&s, h, "fmi3SetInt64");
  fmu->fmi3SetUInt64 = (fmi3SetUInt64TYPE *)getAdr(&s, h, "fmi3SetUInt64");
  fmu->fmi3SetBoolean = (fmi3SetBooleanTYPE *)getAdr(&s, h, "fmi3SetBoolean");
  fmu->fmi3SetString = (fmi3SetStringTYPE *)getAdr(&s, h, "fmi3SetString");
  fmu->fmi3SetBinary = (fmi3SetBinaryTYPE *)getAdr(&s, h, "fmi3SetBinary");
  fmu->fmi3GetNumberOfVariableDependencies =
      (fmi3GetNumberOfVariableDependenciesTYPE *)getAdr(
          &s, h, "fmi3GetNumberOfVariableDependencies");
  fmu->fmi3GetVariableDependencies = (fmi3GetVariableDependenciesTYPE *)getAdr(
      &s, h, "fmi3GetVariableDependencies");
  fmu->fmi3GetFMUState =
      (fmi3GetFMUStateTYPE *)getAdr(&s, h, "fmi3GetFMUState");
  fmu->fmi3SetFMUState =
      (fmi3SetFMUStateTYPE *)getAdr(&s, h, "fmi3SetFMUState");
  fmu->fmi3FreeFMUState =
      (fmi3FreeFMUStateTYPE *)getAdr(&s, h, "fmi3FreeFMUState");
  fmu->fmi3SerializedFMUStateSize = (fmi3SerializedFMUStateSizeTYPE *)getAdr(
      &s, h, "fmi3SerializedFMUStateSize");
  fmu->fmi3SerializeFMUState =
      (fmi3SerializeFMUStateTYPE *)getAdr(&s, h, "fmi3SerializeFMUState");
  fmu->fmi3DeSerializeFMUState =
      (fmi3DeSerializeFMUStateTYPE *)getAdr(&s, h, "fmi3DeSerializeFMUState");
  fmu->fmi3GetDirectionalDerivative =
      (fmi3GetDirectionalDerivativeTYPE *)getAdr(
          &s, h, "fmi3GetDirectionalDerivative");
  fmu->fmi3GetAdjointDerivative =
      (fmi3GetAdjointDerivativeTYPE *)getAdr(&s, h, "fmi3GetAdjointDerivative");
  fmu->fmi3EnterConfigurationMode = (fmi3EnterConfigurationModeTYPE *)getAdr(
      &s, h, "fmi3EnterConfigurationMode");
  fmu->fmi3ExitConfigurationMode = (fmi3ExitConfigurationModeTYPE *)getAdr(
      &s, h, "fmi3ExitConfigurationMode");
  fmu->fmi3GetClock = (fmi3GetClockTYPE *)getAdr(&s, h, "fmi3GetClock");
  fmu->fmi3SetClock = (fmi3SetClockTYPE *)getAdr(&s, h, "fmi3SetClock");
  fmu->fmi3GetIntervalDecimal =
      (fmi3GetIntervalDecimalTYPE *)getAdr(&s, h, "fmi3GetIntervalDecimal");
  fmu->fmi3GetIntervalFraction =
      (fmi3GetIntervalFractionTYPE *)getAdr(&s, h, "fmi3GetIntervalFraction");
  fmu->fmi3SetIntervalDecimal =
      (fmi3SetIntervalDecimalTYPE *)getAdr(&s, h, "fmi3SetIntervalDecimal");
  fmu->fmi3SetIntervalFraction =
      (fmi3SetIntervalFractionTYPE *)getAdr(&s, h, "fmi3SetIntervalFraction");
  fmu->fmi3UpdateDiscreteStates =
      (fmi3UpdateDiscreteStatesTYPE *)getAdr(&s, h, "fmi3UpdateDiscreteStates");
  fmu->fmi3EnterContinuousTimeMode = (fmi3EnterContinuousTimeModeTYPE *)getAdr(
      &s, h, "fmi3EnterContinuousTimeMode");
  fmu->fmi3CompletedIntegratorStep = (fmi3CompletedIntegratorStepTYPE *)getAdr(
      &s, h, "fmi3CompletedIntegratorStep");
  fmu->fmi3SetTime = (fmi3SetTimeTYPE *)getAdr(&s, h, "fmi3SetTime");
  fmu->fmi3SetContinuousStates =
      (fmi3SetContinuousStatesTYPE *)getAdr(&s, h, "fmi3SetContinuousStates");
  fmu->fmi3GetEventIndicators =
      (fmi3GetEventIndicatorsTYPE *)getAdr(&s, h, "fmi3GetEventIndicators");
  fmu->fmi3GetContinuousStates =
      (fmi3GetContinuousStatesTYPE *)getAdr(&s, h, "fmi3GetContinuousStates");
  fmu->fmi3GetNominalsOfContinuousStates =
      (fmi3GetNominalsOfContinuousStatesTYPE *)getAdr(
          &s, h, "fmi3GetNominalsOfContinuousStates");
  fmu->fmi3GetNumberOfEventIndicators =
      (fmi3GetNumberOfEventIndicatorsTYPE *)getAdr(
          &s, h, "fmi3GetNumberOfEventIndicators");
  fmu->fmi3GetNumberOfContinuousStates =
      (fmi3GetNumberOfContinuousStatesTYPE *)getAdr(
          &s, h, "fmi3GetNumberOfContinuousStates");
  fmu->fmi3EnterStepMode =
      (fmi3EnterStepModeTYPE *)getAdr(&s, h, "fmi3EnterStepMode");
  fmu->fmi3GetOutputDerivatives =
      (fmi3GetOutputDerivativesTYPE *)getAdr(&s, h, "fmi3GetOutputDerivatives");
  fmu->fmi3DoStep = (fmi3DoStepTYPE *)getAdr(&s, h, "fmi3DoStep");
  fmu->fmi3ActivateModelPartition = (fmi3ActivateModelPartitionTYPE *)getAdr(
      &s, h, "fmi3ActivateModelPartition");

  return s;
}

static const char *fmi3StatusToString(fmi3Status status) {
  switch (status) {
    case fmi3OK:
      return "ok";
    case fmi3Warning:
      return "warning";
    case fmi3Discard:
      return "discard";
    case fmi3Error:
      return "error";
    case fmi3Fatal:
      return "fatal";
    default:
      return "?";
  }
}

#define MAX_MSG_SIZE 1000

void fmu3Logger(fmi3InstanceEnvironment instanceEnvironment,
                fmi3String instanceName, fmi3Status status, fmi3String category,
                fmi3String message, ...) {
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
  printf("%s %s (%s): %s\n", fmi3StatusToString(status), instanceName, category,
         msg);
}

void fmu3CallbackIntermediateUpdate(
    fmi3InstanceEnvironment instanceEnvironment,
    fmi3Float64 intermediateUpdateTime, fmi3Boolean clocksTicked,
    fmi3Boolean intermediateVariableSetRequested,
    fmi3Boolean intermediateVariableGetAllowed,
    fmi3Boolean intermediateStepFinished, fmi3Boolean canReturnEarly,
    fmi3Boolean *earlyReturnRequested, fmi3Float64 *earlyReturnTime) {}

void fmu3CallbackLockPreemption() {}

void fmu3CallbackUnlockPreemption() {}

int error3(const char *message) {
  printf("%s\n", message);
  return 0;
}
