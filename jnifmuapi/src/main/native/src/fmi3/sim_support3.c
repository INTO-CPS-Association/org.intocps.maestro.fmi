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

static void *getAdrOptional(int *success, HMODULE dllHandle, char **errorMessage,
                            const char *functionName, bool optional) {
#ifdef _WIN32
    void *fp = (void *)GetProcAddress(dllHandle, functionName);
#elif __APPLE__

#include "TargetConditionals.h"

#if TARGET_OS_MAC
    // Other kinds of Mac OS
    void *fp = (void *) dlsym(dllHandle, functionName);
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
            if (*errorMessage == NULL) {
                *errorMessage = strdup(functionName);
            }else{size_t newSize = strlen(*errorMessage)+1+ strlen(functionName);
                 char* m = malloc(newSize);
                strcat(m,*errorMessage);
                strcat(m,",");
                strcat(m,functionName);
                free(*errorMessage);
                *errorMessage=m;
            }
            *success = 0;
        }
    }
    return fp;
}

static void *getAdr(int *success, HMODULE dllHandle, char **errorMessage, const char *functionName) {
    return getAdrOptional(success, dllHandle, errorMessage, functionName, false);
}

// Load the given dll and set function pointers in fmu
// Return 0 to indicate failure
int loadDll3(const char *dllPath, FMU3 *fmu, char **errorMessage) {
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

    fmu->fmi3GetVersion = (fmi3GetVersionTYPE *) getAdr(&s, h, errorMessage, "fmi3GetVersion");
    fmu->fmi3SetDebugLogging =
            (fmi3SetDebugLoggingTYPE *) getAdr(&s, h, errorMessage, "fmi3SetDebugLogging");
    fmu->fmi3InstantiateModelExchange =
            (fmi3InstantiateModelExchangeTYPE *) getAdr(
                    &s, h, errorMessage, "fmi3InstantiateModelExchange");
    fmu->fmi3InstantiateCoSimulation = (fmi3InstantiateCoSimulationTYPE *) getAdr(
            &s, h, errorMessage, "fmi3InstantiateCoSimulation");
    fmu->fmi3InstantiateScheduledExecution =
            (fmi3InstantiateScheduledExecutionTYPE *) getAdr(
                    &s, h, errorMessage, "fmi3InstantiateScheduledExecution");
    fmu->fmi3FreeInstance =
            (fmi3FreeInstanceTYPE *) getAdr(&s, h, errorMessage, "fmi3FreeInstance");
    fmu->fmi3EnterInitializationMode = (fmi3EnterInitializationModeTYPE *) getAdr(
            &s, h, errorMessage, "fmi3EnterInitializationMode");
    fmu->fmi3ExitInitializationMode = (fmi3ExitInitializationModeTYPE *) getAdr(
            &s, h, errorMessage, "fmi3ExitInitializationMode");
    fmu->fmi3EnterEventMode =
            (fmi3EnterEventModeTYPE *) getAdr(&s, h, errorMessage, "fmi3EnterEventMode");
    fmu->fmi3Terminate = (fmi3TerminateTYPE *) getAdr(&s, h, errorMessage, "fmi3Terminate");
    fmu->fmi3Reset = (fmi3ResetTYPE *) getAdr(&s, h, errorMessage, "fmi3Reset");
    fmu->fmi3GetFloat32 = (fmi3GetFloat32TYPE *) getAdr(&s, h, errorMessage, "fmi3GetFloat32");
    fmu->fmi3GetFloat64 = (fmi3GetFloat64TYPE *) getAdr(&s, h, errorMessage, "fmi3GetFloat64");
    fmu->fmi3GetInt8 = (fmi3GetInt8TYPE *) getAdr(&s, h, errorMessage, "fmi3GetInt8");
    fmu->fmi3GetUInt8 = (fmi3GetUInt8TYPE *) getAdr(&s, h, errorMessage, "fmi3GetUInt8");
    fmu->fmi3GetInt16 = (fmi3GetInt16TYPE *) getAdr(&s, h, errorMessage, "fmi3GetInt16");
    fmu->fmi3GetUInt16 = (fmi3GetUInt16TYPE *) getAdr(&s, h, errorMessage, "fmi3GetUInt16");
    fmu->fmi3GetInt32 = (fmi3GetInt32TYPE *) getAdr(&s, h, errorMessage, "fmi3GetInt32");
    fmu->fmi3GetUInt32 = (fmi3GetUInt32TYPE *) getAdr(&s, h, errorMessage, "fmi3GetUInt32");
    fmu->fmi3GetInt64 = (fmi3GetInt64TYPE *) getAdr(&s, h, errorMessage, "fmi3GetInt64");
    fmu->fmi3GetUInt64 = (fmi3GetUInt64TYPE *) getAdr(&s, h, errorMessage, "fmi3GetUInt64");
    fmu->fmi3GetBoolean = (fmi3GetBooleanTYPE *) getAdr(&s, h, errorMessage, "fmi3GetBoolean");
    fmu->fmi3GetString = (fmi3GetStringTYPE *) getAdr(&s, h, errorMessage, "fmi3GetString");
    fmu->fmi3GetBinary = (fmi3GetBinaryTYPE *) getAdr(&s, h, errorMessage, "fmi3GetBinary");
    fmu->fmi3SetFloat32 = (fmi3SetFloat32TYPE *) getAdr(&s, h, errorMessage, "fmi3SetFloat32");
    fmu->fmi3SetFloat64 = (fmi3SetFloat64TYPE *) getAdr(&s, h, errorMessage, "fmi3SetFloat64");
    fmu->fmi3SetInt8 = (fmi3SetInt8TYPE *) getAdr(&s, h, errorMessage, "fmi3SetInt8");
    fmu->fmi3SetUInt8 = (fmi3SetUInt8TYPE *) getAdr(&s, h, errorMessage, "fmi3SetUInt8");
    fmu->fmi3SetInt16 = (fmi3SetInt16TYPE *) getAdr(&s, h, errorMessage, "fmi3SetInt16");
    fmu->fmi3SetUInt16 = (fmi3SetUInt16TYPE *) getAdr(&s, h, errorMessage, "fmi3SetUInt16");
    fmu->fmi3SetInt32 = (fmi3SetInt32TYPE *) getAdr(&s, h, errorMessage, "fmi3SetInt32");
    fmu->fmi3SetUInt32 = (fmi3SetUInt32TYPE *) getAdr(&s, h, errorMessage, "fmi3SetUInt32");
    fmu->fmi3SetInt64 = (fmi3SetInt64TYPE *) getAdr(&s, h, errorMessage, "fmi3SetInt64");
    fmu->fmi3SetUInt64 = (fmi3SetUInt64TYPE *) getAdr(&s, h, errorMessage, "fmi3SetUInt64");
    fmu->fmi3SetBoolean = (fmi3SetBooleanTYPE *) getAdr(&s, h, errorMessage, "fmi3SetBoolean");
    fmu->fmi3SetString = (fmi3SetStringTYPE *) getAdr(&s, h, errorMessage, "fmi3SetString");
    fmu->fmi3SetBinary = (fmi3SetBinaryTYPE *) getAdr(&s, h, errorMessage, "fmi3SetBinary");
    fmu->fmi3GetNumberOfVariableDependencies =
            (fmi3GetNumberOfVariableDependenciesTYPE *) getAdr(
                    &s, h, errorMessage, "fmi3GetNumberOfVariableDependencies");
    fmu->fmi3GetVariableDependencies = (fmi3GetVariableDependenciesTYPE *) getAdr(
            &s, h, errorMessage, "fmi3GetVariableDependencies");
    fmu->fmi3GetFMUState =
            (fmi3GetFMUStateTYPE *) getAdr(&s, h, errorMessage, "fmi3GetFMUState");
    fmu->fmi3SetFMUState =
            (fmi3SetFMUStateTYPE *) getAdr(&s, h, errorMessage, "fmi3SetFMUState");
    fmu->fmi3FreeFMUState =
            (fmi3FreeFMUStateTYPE *) getAdr(&s, h, errorMessage, "fmi3FreeFMUState");
    fmu->fmi3SerializedFMUStateSize = (fmi3SerializedFMUStateSizeTYPE *) getAdr(
            &s, h, errorMessage, "fmi3SerializedFMUStateSize");
    fmu->fmi3SerializeFMUState =
            (fmi3SerializeFMUStateTYPE *) getAdr(&s, h, errorMessage, "fmi3SerializeFMUState");
    fmu->fmi3DeserializeFMUState =
            (fmi3DeserializeFMUStateTYPE *) getAdr(&s, h, errorMessage, "fmi3DeserializeFMUState");
    fmu->fmi3GetDirectionalDerivative =
            (fmi3GetDirectionalDerivativeTYPE *) getAdr(
                    &s, h, errorMessage, "fmi3GetDirectionalDerivative");
    fmu->fmi3GetAdjointDerivative =
            (fmi3GetAdjointDerivativeTYPE *) getAdr(&s, h, errorMessage, "fmi3GetAdjointDerivative");
    fmu->fmi3EnterConfigurationMode = (fmi3EnterConfigurationModeTYPE *) getAdr(
            &s, h, errorMessage, "fmi3EnterConfigurationMode");
    fmu->fmi3ExitConfigurationMode = (fmi3ExitConfigurationModeTYPE *) getAdr(
            &s, h, errorMessage, "fmi3ExitConfigurationMode");
    fmu->fmi3GetClock = (fmi3GetClockTYPE *) getAdr(&s, h, errorMessage, "fmi3GetClock");
    fmu->fmi3SetClock = (fmi3SetClockTYPE *) getAdr(&s, h, errorMessage, "fmi3SetClock");
    fmu->fmi3GetIntervalDecimal =
            (fmi3GetIntervalDecimalTYPE *) getAdr(&s, h, errorMessage, "fmi3GetIntervalDecimal");
    fmu->fmi3GetIntervalFraction =
            (fmi3GetIntervalFractionTYPE *) getAdr(&s, h, errorMessage, "fmi3GetIntervalFraction");

    fmu->fmi3GetShiftDecimal = (fmi3GetShiftDecimalTYPE *) getAdr(&s, h, errorMessage, "fmi3GetShiftDecimal");
    fmu->fmi3GetShiftFraction = (fmi3GetShiftFractionTYPE *) getAdr(&s, h, errorMessage, "fmi3GetShiftFraction");


    fmu->fmi3SetIntervalDecimal =
            (fmi3SetIntervalDecimalTYPE *) getAdr(&s, h, errorMessage, "fmi3SetIntervalDecimal");
    fmu->fmi3SetIntervalFraction =
            (fmi3SetIntervalFractionTYPE *) getAdr(&s, h, errorMessage, "fmi3SetIntervalFraction");

    fmu->fmi3SetShiftDecimal = (fmi3SetShiftDecimalTYPE *) getAdr(&s, h, errorMessage, "fmi3SetShiftDecimal");
    fmu->fmi3SetShiftFraction = (fmi3SetShiftFractionTYPE *) getAdr(&s, h, errorMessage, "fmi3SetShiftFraction");


    fmu->fmi3EvaluateDiscreteStates =
            (fmi3EvaluateDiscreteStatesTYPE *) getAdr(&s, h, errorMessage, "fmi3EvaluateDiscreteStates");

    fmu->fmi3UpdateDiscreteStates =
            (fmi3UpdateDiscreteStatesTYPE *) getAdr(&s, h, errorMessage, "fmi3UpdateDiscreteStates");
    fmu->fmi3EnterContinuousTimeMode = (fmi3EnterContinuousTimeModeTYPE *) getAdr(
            &s, h, errorMessage, "fmi3EnterContinuousTimeMode");
    fmu->fmi3CompletedIntegratorStep = (fmi3CompletedIntegratorStepTYPE *) getAdr(
            &s, h, errorMessage, "fmi3CompletedIntegratorStep");
    fmu->fmi3SetTime = (fmi3SetTimeTYPE *) getAdr(&s, h, errorMessage, "fmi3SetTime");
    fmu->fmi3SetContinuousStates =
            (fmi3SetContinuousStatesTYPE *) getAdr(&s, h, errorMessage, "fmi3SetContinuousStates");
    fmu->fmi3GetEventIndicators =
            (fmi3GetEventIndicatorsTYPE *) getAdr(&s, h, errorMessage, "fmi3GetEventIndicators");
    fmu->fmi3GetContinuousStates =
            (fmi3GetContinuousStatesTYPE *) getAdr(&s, h, errorMessage, "fmi3GetContinuousStates");
    fmu->fmi3GetNominalsOfContinuousStates =
            (fmi3GetNominalsOfContinuousStatesTYPE *) getAdr(
                    &s, h, errorMessage, "fmi3GetNominalsOfContinuousStates");
    fmu->fmi3GetNumberOfEventIndicators =
            (fmi3GetNumberOfEventIndicatorsTYPE *) getAdr(
                    &s, h, errorMessage, "fmi3GetNumberOfEventIndicators");
    fmu->fmi3GetNumberOfContinuousStates =
            (fmi3GetNumberOfContinuousStatesTYPE *) getAdr(
                    &s, h, errorMessage, "fmi3GetNumberOfContinuousStates");
    fmu->fmi3EnterStepMode =
            (fmi3EnterStepModeTYPE *) getAdr(&s, h, errorMessage, "fmi3EnterStepMode");
    fmu->fmi3GetOutputDerivatives =
            (fmi3GetOutputDerivativesTYPE *) getAdr(&s, h, errorMessage, "fmi3GetOutputDerivatives");
    fmu->fmi3DoStep = (fmi3DoStepTYPE *) getAdr(&s, h, errorMessage, "fmi3DoStep");
    fmu->fmi3ActivateModelPartition = (fmi3ActivateModelPartitionTYPE *) getAdr(
            &s, h, errorMessage, "fmi3ActivateModelPartition");

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
