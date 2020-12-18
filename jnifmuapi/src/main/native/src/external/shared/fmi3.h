/* ------------------------------------------------------------------------- 
 * fmi.h
 * Struct with the corresponding function pointers for FMI 2.0.
 * Copyright QTronic GmbH. All rights reserved.
 * -------------------------------------------------------------------------*/

#ifndef FMI3_H
#define FMI3_H



#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_MAC
// Other kinds of Mac OS
#include <dlfcn.h>
#define HMODULE void*
#else
// Unsupported platform
#endif
#elif __linux
#include <limits.h>
#include <dlfcn.h>
#define MAX_PATH PATH_MAX
#define HMODULE void*
#endif


#include "fmi3Functions.h"


//#include "XmlParserCApi.h"

typedef struct {
    //ModelDescription* modelDescription;

    HMODULE dllHandle; // fmu.dll handle
    fmi3GetVersionTYPE *fmi3GetVersion;
    fmi3SetDebugLoggingTYPE *fmi3SetDebugLogging;
    fmi3InstantiateModelExchangeTYPE *fmi3InstantiateModelExchange;
    fmi3InstantiateCoSimulationTYPE *fmi3InstantiateCoSimulation;
    fmi3InstantiateScheduledExecutionTYPE *fmi3InstantiateScheduledExecution;
    fmi3FreeInstanceTYPE *fmi3FreeInstance;
    fmi3EnterInitializationModeTYPE *fmi3EnterInitializationMode;
    fmi3ExitInitializationModeTYPE *fmi3ExitInitializationMode;
    fmi3EnterEventModeTYPE *fmi3EnterEventMode;
    fmi3TerminateTYPE *fmi3Terminate;
    fmi3ResetTYPE *fmi3Reset;
    fmi3GetFloat32TYPE *fmi3GetFloat32;
    fmi3GetFloat64TYPE *fmi3GetFloat64;
    fmi3GetInt8TYPE *fmi3GetInt8;
    fmi3GetUInt8TYPE *fmi3GetUInt8;
    fmi3GetInt16TYPE *fmi3GetInt16;
    fmi3GetUInt16TYPE *fmi3GetUInt16;
    fmi3GetInt32TYPE *fmi3GetInt32;
    fmi3GetUInt32TYPE *fmi3GetUInt32;
    fmi3GetInt64TYPE *fmi3GetInt64;
    fmi3GetUInt64TYPE *fmi3GetUInt64;
    fmi3GetBooleanTYPE *fmi3GetBoolean;
    fmi3GetStringTYPE *fmi3GetString;
    fmi3GetBinaryTYPE *fmi3GetBinary;
    fmi3SetFloat32TYPE *fmi3SetFloat32;
    fmi3SetFloat64TYPE *fmi3SetFloat64;
    fmi3SetInt8TYPE *fmi3SetInt8;
    fmi3SetUInt8TYPE *fmi3SetUInt8;
    fmi3SetInt16TYPE *fmi3SetInt16;
    fmi3SetUInt16TYPE *fmi3SetUInt16;
    fmi3SetInt32TYPE *fmi3SetInt32;
    fmi3SetUInt32TYPE *fmi3SetUInt32;
    fmi3SetInt64TYPE *fmi3SetInt64;
    fmi3SetUInt64TYPE *fmi3SetUInt64;
    fmi3SetBooleanTYPE *fmi3SetBoolean;
    fmi3SetStringTYPE *fmi3SetString;
    fmi3SetBinaryTYPE *fmi3SetBinary;
    fmi3GetNumberOfVariableDependenciesTYPE *fmi3GetNumberOfVariableDependencies;
    fmi3GetVariableDependenciesTYPE *fmi3GetVariableDependencies;
    fmi3GetFMUStateTYPE *fmi3GetFMUState;
    fmi3SetFMUStateTYPE *fmi3SetFMUState;
    fmi3FreeFMUStateTYPE *fmi3FreeFMUState;
    fmi3SerializedFMUStateSizeTYPE *fmi3SerializedFMUStateSize;
    fmi3SerializeFMUStateTYPE *fmi3SerializeFMUState;
    fmi3DeSerializeFMUStateTYPE *fmi3DeSerializeFMUState;
    fmi3GetDirectionalDerivativeTYPE *fmi3GetDirectionalDerivative;
    fmi3GetAdjointDerivativeTYPE *fmi3GetAdjointDerivative;
    fmi3EnterConfigurationModeTYPE *fmi3EnterConfigurationMode;
    fmi3ExitConfigurationModeTYPE *fmi3ExitConfigurationMode;
    fmi3GetClockTYPE *fmi3GetClock;
    fmi3SetClockTYPE *fmi3SetClock;
    fmi3GetIntervalDecimalTYPE *fmi3GetIntervalDecimal;
    fmi3GetIntervalFractionTYPE *fmi3GetIntervalFraction;
    fmi3SetIntervalDecimalTYPE *fmi3SetIntervalDecimal;
    fmi3SetIntervalFractionTYPE *fmi3SetIntervalFraction;
    fmi3UpdateDiscreteStatesTYPE *fmi3UpdateDiscreteStates;
    fmi3EnterContinuousTimeModeTYPE *fmi3EnterContinuousTimeMode;
    fmi3CompletedIntegratorStepTYPE *fmi3CompletedIntegratorStep;
    fmi3SetTimeTYPE *fmi3SetTime;
    fmi3SetContinuousStatesTYPE *fmi3SetContinuousStates;
    fmi3GetDerivativesTYPE *fmi3GetDerivatives;
    fmi3GetEventIndicatorsTYPE *fmi3GetEventIndicators;
    fmi3GetContinuousStatesTYPE *fmi3GetContinuousStates;
    fmi3GetNominalsOfContinuousStatesTYPE *fmi3GetNominalsOfContinuousStates;
    fmi3GetNumberOfEventIndicatorsTYPE *fmi3GetNumberOfEventIndicators;
    fmi3GetNumberOfContinuousStatesTYPE *fmi3GetNumberOfContinuousStates;
    fmi3EnterStepModeTYPE *fmi3EnterStepMode;
    fmi3GetOutputDerivativesTYPE *fmi3GetOutputDerivatives;
    fmi3DoStepTYPE *fmi3DoStep;
    fmi3ActivateModelPartitionTYPE *fmi3ActivateModelPartition;

} FMU3;

#endif // FMI_H

