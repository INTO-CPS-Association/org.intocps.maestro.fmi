/* ------------------------------------------------------------------------- 
 * fmi.h
 * Struct with the corresponding function pointers for FMI 2.0.
 * Copyright QTronic GmbH. All rights reserved.
 * -------------------------------------------------------------------------*/

#ifndef FMI2_H
#define FMI2_H

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



#include "fmi2Functions.h"



/* into-cps specific*/
typedef fmi2Status fmi2GetMaxStepsizeTYPE(fmi2Component, fmi2Real*);

/* custom INTO-CPS*/
FMI2_Export fmi2GetMaxStepsizeTYPE    fmi2GetMaxStepsize;

//#include "XmlParserCApi.h"

typedef struct {
    //ModelDescription* modelDescription;

    HMODULE dllHandle; // fmu.dll handle
    /***************************************************
    Common Functions
    ****************************************************/
    fmi2GetTypesPlatformTYPE         *getTypesPlatform;
    fmi2GetVersionTYPE               *getVersion;
    fmi2SetDebugLoggingTYPE          *setDebugLogging;
    fmi2InstantiateTYPE              *instantiate;
    fmi2FreeInstanceTYPE             *freeInstance;
    fmi2SetupExperimentTYPE          *setupExperiment;
    fmi2EnterInitializationModeTYPE  *enterInitializationMode;
    fmi2ExitInitializationModeTYPE   *exitInitializationMode;
    fmi2TerminateTYPE                *terminate;
    fmi2ResetTYPE                    *reset;
    fmi2GetRealTYPE                  *getReal;
    fmi2GetIntegerTYPE               *getInteger;
    fmi2GetBooleanTYPE               *getBoolean;
    fmi2GetStringTYPE                *getString;
    fmi2SetRealTYPE                  *setReal;
    fmi2SetIntegerTYPE               *setInteger;
    fmi2SetBooleanTYPE               *setBoolean;
    fmi2SetStringTYPE                *setString;
    fmi2GetFMUstateTYPE              *getFMUstate;
    fmi2SetFMUstateTYPE              *setFMUstate;
    fmi2FreeFMUstateTYPE             *freeFMUstate;
    fmi2SerializedFMUstateSizeTYPE   *serializedFMUstateSize;
    fmi2SerializeFMUstateTYPE        *serializeFMUstate;
    fmi2DeSerializeFMUstateTYPE      *deSerializeFMUstate;
    fmi2GetDirectionalDerivativeTYPE *getDirectionalDerivative;
    /***************************************************
    Functions for FMI2 for Co-Simulation
    ****************************************************/
    fmi2SetRealInputDerivativesTYPE  *setRealInputDerivatives;
    fmi2GetRealOutputDerivativesTYPE *getRealOutputDerivatives;
    fmi2DoStepTYPE                   *doStep;
    fmi2CancelStepTYPE               *cancelStep;
    fmi2GetStatusTYPE                *getStatus;
    fmi2GetRealStatusTYPE            *getRealStatus;
    fmi2GetIntegerStatusTYPE         *getIntegerStatus;
    fmi2GetBooleanStatusTYPE         *getBooleanStatus;
    fmi2GetStringStatusTYPE          *getStringStatus;

    //INTO CPS specific
    fmi2GetMaxStepsizeTYPE 			*getMaxStepsize;
    /***************************************************
    Functions for FMI2 for Model Exchange
    ****************************************************/
    fmi2EnterEventModeTYPE                *enterEventMode;
    fmi2NewDiscreteStatesTYPE             *newDiscreteStates;
    fmi2EnterContinuousTimeModeTYPE       *enterContinuousTimeMode;
    fmi2CompletedIntegratorStepTYPE       *completedIntegratorStep;
    fmi2SetTimeTYPE                       *setTime;
    fmi2SetContinuousStatesTYPE           *setContinuousStates;
    fmi2GetDerivativesTYPE                *getDerivatives;
    fmi2GetEventIndicatorsTYPE            *getEventIndicators;
    fmi2GetContinuousStatesTYPE           *getContinuousStates;
    fmi2GetNominalsOfContinuousStatesTYPE *getNominalsOfContinuousStates;
} FMU;

#endif // FMI_H

