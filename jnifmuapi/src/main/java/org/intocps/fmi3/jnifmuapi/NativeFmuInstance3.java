package org.intocps.fmi3.jnifmuapi;


import org.intocps.fmi3.Fmi3Status;

public class NativeFmuInstance3 {

    final long fmuPtr;

    public NativeFmuInstance3(long fmuPtr) {
        this.fmuPtr = fmuPtr;
    }

    native Fmi3Status nSetDebugLogging(long instance, boolean loggingOn, int nCategories, String categories[]);

    /* tag::FreeInstance[] */
    native void nFreeInstance(long instance);
    /* end::FreeInstance[] */

    /* Enter and exit initialization mode, enter event mode, terminate and reset */
    /* tag::EnterInitializationMode[] */
    native Fmi3Status nEnterInitializationMode(long instance, boolean toleranceDefined, double tolerance, double startTime, boolean stopTimeDefined,
            double stopTime);
    /* end::EnterInitializationMode[] */

    /* tag::ExitInitializationMode[] */
    native Fmi3Status nExitInitializationMode(long instance);
    /* end::ExitInitializationMode[] */

    /* tag::EnterEventMode[] */
    native Fmi3Status nEnterEventMode(long instance, boolean stepEvent, int rootsFound[], int nEventIndicators, boolean timeEvent);
    /* end::EnterEventMode[] */

    /* tag::Terminate[] */
    native Fmi3Status nTerminate(long instance);
    /* end::Terminate[] */

    /* tag::Reset[] */
    native Fmi3Status nReset(long instance);
    /* end::Reset[] */

    /* Getting and setting variable values */
    /* tag::Getters[] */
    native Fmi3Status nGetFloat32(long instance, long valueReferences[], int nValueReferences, double values[], int nValues);

    native Fmi3Status nGetFloat64(long instance, long valueReferences[], int nValueReferences, double values[], int nValues);

    native Fmi3Status nGetInt8(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nGetUInt8(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nGetInt16(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nGetUInt16(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nGetInt32(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nGetUInt32(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nGetInt64(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nGetUInt64(long instance, long valueReferences[], int nValueReferences, long values[], int nValues);

    native Fmi3Status nGetBoolean(long instance, long valueReferences[], int nValueReferences, boolean values[], int nValues);

    native Fmi3Status nGetString(long instance, long valueReferences[], int nValueReferences, String values[], int nValues);

    native Fmi3Status nGetBinary(long instance, long valueReferences[], int nValueReferences, int sizes[], long values[], int nValues);
    /* end::Getters[] */

    /* tag::Setters[] */
    native Fmi3Status nSetFloat32(long instance, long valueReferences[], int nValueReferences, double values[], int nValues);

    native Fmi3Status nSetFloat64(long instance, long valueReferences[], int nValueReferences, double values[], int nValues);

    native Fmi3Status nSetInt8(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nSetUInt8(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nSetInt16(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nSetUInt16(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nSetInt32(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nSetUInt32(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nSetInt64(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nSetUInt64(long instance, long valueReferences[], int nValueReferences, long values[], int nValues);

    native Fmi3Status nSetBoolean(long instance, long valueReferences[], int nValueReferences, boolean values[], int nValues);

    native Fmi3Status nSetString(long instance, long valueReferences[], int nValueReferences, String values[], int nValues);

    native Fmi3Status nSetBinary(long instance, long valueReferences[], int nValueReferences, int sizes[], long values[], int nValues);
    /* end::Setters[] */

    /* Getting Variable Dependency Information */
    /* tag::GetNumberOfVariableDependencies[] */
    native Fmi3Status nGetNumberOfVariableDependencies(long instance, long valueReference, int[] nDependencies);
    /* end::GetNumberOfVariableDependencies[] */

    /* tag::GetVariableDependencies[] */
    native Fmi3Status nGetVariableDependencies(long instance, long dependent, int elementIndicesOfDependent[], long independents[],
            int elementIndicesOfIndependents[], int dependencyKinds[], int nDependencies);
    /* end::GetVariableDependencies[] */

    /* Getting and setting the internal FMU state */
    /* tag::GetFMUState[] */
    native Fmi3Status nGetFMUState(long instance, long[] FMUState);
    /* end::GetFMUState[] */

    /* tag::SetFMUState[] */
    native Fmi3Status nSetFMUState(long instance, long FMUState);
    /* end::SetFMUState[] */

    /* tag::FreeFMUState[] */
    native Fmi3Status nFreeFMUState(long instance, long FMUState);
    /* end::FreeFMUState[] */

    /* tag::SerializedFMUStateSize[] */
    native Fmi3Status nSerializedFMUStateSize(long instance, long FMUState, int[] size);
    /* end::SerializedFMUStateSize[] */

    //    /* tag::SerializeFMUState[] */
    //    native Fmi3Status nSerializeFMUState     (long instance,
    //            fmi3FMUState  FMUState,
    //            fmi3Byte serializedState[],
    //            int size);
    //    /* end::SerializeFMUState[] */
    //
    //    /* tag::DeSerializeFMUState[] */
    //    native Fmi3Status nDeSerializeFMUState   (long instance,
    //            fmi3Byte serializedState[],
    //            int size,
    //            fmi3FMUState* FMUState);
    //    /* end::DeSerializeFMUState[] */

    /* Getting partial derivatives */
    /* tag::GetDirectionalDerivative[] */
    native Fmi3Status nGetDirectionalDerivative(long instance, long unknowns[], int nUnknowns, long knowns[], int nKnowns, double seed[], int nSeed,
            double sensitivity[], int nSensitivity);
    /* end::GetDirectionalDerivative[] */

    /* tag::GetAdjointDerivative[] */
    native Fmi3Status nGetAdjointDerivative(long instance, long unknowns[], int nUnknowns, long knowns[], int nKnowns, double seed[], int nSeed,
            double sensitivity[], int nSensitivity);
    /* end::GetAdjointDerivative[] */


    /* Entering and exiting the Configuration or Reconfiguration Mode */
    /* tag::EnterConfigurationMode[] */
    native Fmi3Status nEnterConfigurationMode(long instance);
    /* end::EnterConfigurationMode[] */

    /* tag::ExitConfigurationMode[] */
    native Fmi3Status nExitConfigurationMode(long instance);
    /* end::ExitConfigurationMode[] */

    /* Clock related functions */
    /* tag::GetClock[] */
    native Fmi3Status nGetClock(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);
    /* end::GetClock[] */

    /* tag::SetClock[] */
    native Fmi3Status nSetClock(long instance, long valueReferences[], int nValueReferences, int values[], boolean subactive[], int nValues);
    /* end::SetClock[] */

    /* tag::GetIntervalDecimal[] */
    native Fmi3Status nGetIntervalDecimal(long instance, long valueReferences[], int nValueReferences, double interval[], int nValues);
    /* end::GetIntervalDecimal[] */

    /* tag::GetIntervalFraction[] */
    native Fmi3Status nGetIntervalFraction(long instance, long valueReferences[], int nValueReferences, long intervalCounter[], long resolution[],
            int nValues);
    /* end::GetIntervalFraction[] */

    /* tag::SetIntervalDecimal[] */
    native Fmi3Status nSetIntervalDecimal(long instance, long valueReferences[], int nValueReferences, double interval[], int nValues);
    /* end::SetIntervalDecimal[] */

    /* tag::SetIntervalFraction[] */
    native Fmi3Status nSetIntervalFraction(long instance, long valueReferences[], int nValueReferences, long intervalCounter[], long resolution[],
            int nValues);
    /* end::SetIntervalFraction[] */

    /* tag::UpdateDiscreteStates[] */
    native Fmi3Status nUpdateDiscreteStates(long instance, boolean[] discreteStatesNeedUpdate, boolean[] terminateSimulation,
            boolean[] nominalsOfContinuousStatesChanged, boolean[] valuesOfContinuousStatesChanged, boolean[] nextEventTimeDefined,
            double[] nextEventTime);
    /* end::UpdateDiscreteStates[] */

    /***************************************************
     Types for Functions for Model Exchange
     ****************************************************/

    /* tag::EnterContinuousTimeMode[] */
    native Fmi3Status nEnterContinuousTimeMode(long instance);
    /* end::EnterContinuousTimeMode[] */

    /* tag::CompletedIntegratorStep[] */
    native Fmi3Status nCompletedIntegratorStep(long instance, boolean noSetFMUStatePriorToCurrentPoint, boolean[] enterEventMode,
            boolean[] terminateSimulation);
    /* end::CompletedIntegratorStep[] */

    /* Providing independent variables and re-initialization of caching */
    /* tag::SetTime[] */
    native Fmi3Status nSetTime(long instance, double time);
    /* end::SetTime[] */

    /* tag::SetContinuousStates[] */
    native Fmi3Status nSetContinuousStates(long instance, double continuousStates[], int nContinuousStates);
    /* end::SetContinuousStates[] */

    /* Evaluation of the model equations */
    /* tag::GetDerivatives[] */
    native Fmi3Status nGetDerivatives(long instance, double derivatives[], int nContinuousStates);
    /* end::GetDerivatives[] */

    /* tag::GetEventIndicators[] */
    native Fmi3Status nGetEventIndicators(long instance, double eventIndicators[], int nEventIndicators);
    /* end::GetEventIndicators[] */

    /* tag::GetContinuousStates[] */
    native Fmi3Status nGetContinuousStates(long instance, double continuousStates[], int nContinuousStates);
    /* end::GetContinuousStates[] */

    /* tag::GetNominalsOfContinuousStates[] */
    native Fmi3Status nGetNominalsOfContinuousStates(long instance, double nominals[], int nContinuousStates);
    /* end::GetNominalsOfContinuousStates[] */

    /* tag::GetNumberOfEventIndicators[] */
    native Fmi3Status nGetNumberOfEventIndicators(long instance, int[] nEventIndicators);
    /* end::GetNumberOfEventIndicators[] */

    /* tag::GetNumberOfContinuousStates[] */
    native Fmi3Status nGetNumberOfContinuousStates(long instance, int[] nContinuousStates);
    /* end::GetNumberOfContinuousStates[] */

    /***************************************************
     Types for Functions for Co-Simulation
     ****************************************************/

    /* Simulating the FMU */

    /* tag::EnterStepMode[] */
    native Fmi3Status nEnterStepMode(long instance);
    /* end::EnterStepMode[] */

    /* tag::GetOutputDerivatives[] */
    native Fmi3Status nGetOutputDerivatives(long instance, long valueReferences[], int nValueReferences, int orders[], double values[], int nValues);
    /* end::GetOutputDerivatives[] */

    /* tag::DoStep[] */
    native Fmi3Status nDoStep(long instance, double currentCommunicationPoint, double communicationStepSize, boolean noSetFMUStatePriorToCurrentPoint,
            boolean[] eventEncountered, boolean[] clocksAboutToTick, boolean[] terminate, boolean[] earlyReturn, double[] lastSuccessfulTime);
    /* end::DoStep[] */

    /* tag::ActivateModelPartition[] */
    native Fmi3Status nActivateModelPartition(long instance, long clockReference, int clockElementIndex, double activationTime);

}
