/* ---------------------------------------------------------------------------*
 * Sample implementation of an FMU - the Dahlquist test equation.
 *
 *   der(x) = - k * x and x(0) = 1. 
 *   Analytical solution: x(t) = exp(-k*t).
 * Copyright QTronic GmbH. All rights reserved.
 * ---------------------------------------------------------------------------*/


// define class name and unique id
#ifndef MODEL_GUID
	#define MODEL_GUID "{8c4e810f-3df3-4a00-8276-176fa3c9f000}"
#endif

#ifndef MODEL_IDENTIFIER
	#define MODEL_IDENTIFIER fmu2test1
#endif
//#define MODEL_IDENTIFIER fmu2test1
//#define MODEL_GUID "{8c4e810f-3df3-4a00-8276-176fa3c9f000}"

// define model size
#define NUMBER_OF_REALS 4
#define NUMBER_OF_INTEGERS 1
#define NUMBER_OF_BOOLEANS 5
#define NUMBER_OF_STRINGS 1
#define NUMBER_OF_STATES 1
#define NUMBER_OF_EVENT_INDICATORS 0

// include fmu header files, typedefs and macros
#include "fmuTemplate.h"


// define all model variables and their value references
// conventions used here:
// - if x is a variable, then macro x_ is its variable reference
// - the vr of a variable is its index in array  r, i, b or s
// - if k is the vr of a real state, then k+1 is the vr of its derivative
#define x_     0
#define der_x_ 1
#define k_     2

// define state vector as vector of value references
#define STATES { x_ }

// called by fmi2Instantiate
// Set values for all variables that define a start value
// Settings used unless changed by fmi2SetX before fmi2EnterInitializationMode
void setStartValues(ModelInstance *comp) {
  //  r(x_) = 1;
  //  r(k_) = 1;
}

// called by fmi2GetReal, fmi2GetInteger, fmi2GetBoolean, fmi2GetString, fmi2ExitInitialization
// if setStartValues or environment set new values through fmi2SetXXX.
// Lazy set values for all variable that are computed from other variables.
void calculateValues(ModelInstance *comp) {
    //if (comp->state == modelInitializationMode) {
    //  initialization code here
    //  set first time event, if any, using comp->eventInfo.nextEventTime
    //}
}

// called by fmi2GetReal, fmi2GetContinuousStates and fmi2GetDerivatives
fmi2Real getReal(ModelInstance* comp, fmi2ValueReference vr){
   /* switch (vr) {
        case x_     : return r(x_);
        case der_x_ : return - r(k_) * r(x_);
        case k_     : return r(k_);
        default: return 0;
    }*/
	return comp->r[vr];
}

// used to set the next time event, if any.
void eventUpdate(ModelInstance *comp, fmi2EventInfo *eventInfo, int isTimeEvent) {
}

#define VALVE_ID 4
#define LEVEL_ID 3
#define LEVEL_MAX 8
#define LEVEL_MAX_ID 0
#define LEVEL_MIN 2.

fmi2Status fmi2DoStepInternal(ModelInstance *comp, fmi2Real currentCommunicationPoint,
	fmi2Real communicationStepSize, fmi2Boolean noSetFMUStatePriorToCurrentPoint){


	fmi2Real levelMax = comp->r[LEVEL_MAX_ID];
	fmi2Real level = comp->r[LEVEL_ID];
#ifdef DEBUG_PRINT
/*	printf("\tinternal step controller,\n\t level is: %.16g",level);
	printf(" min: %d", LEVEL_MIN);
	printf(" max: %f\n", levelMax);*/
	//comp->b[VALVE_ID] = !comp->b[VALVE_ID];
#endif
	if (level >= levelMax)
	{
		comp->b[VALVE_ID] = fmi2True;
	}

	if (level <= LEVEL_MIN)
	{
		comp->b[VALVE_ID] = fmi2False;
	}

	/*if (comp->b[VALVE_ID] == fmi2True)
	{
		comp->b[VALVE_ID] = fmi2False;
	}
	else
	{
		comp->b[VALVE_ID] = fmi2True;
	}*/

//	printf(comp->time);
	return fmi2OK;
}

// include code that implements the FMI based on the above definitions
#include "fmuTemplate.c"

