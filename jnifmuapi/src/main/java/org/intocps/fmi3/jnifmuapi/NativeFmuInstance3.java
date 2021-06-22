package org.intocps.fmi3.jnifmuapi;


import org.intocps.fmi.Fmi2Status;

public class NativeFmuInstance3 {

    final long fmuPtr;

    public NativeFmuInstance3(long fmuPtr) {
        this.fmuPtr = fmuPtr;
    }

    /**
     * size_t nCategories is ignored as it is calculated from categories[].
     * @param instance
     * @param loggingOn
     * @param categories
     * @return
     */
    native Fmi2Status nSetDebugLogging(long fmu, long instance, boolean loggingOn, String categories[]);

    /* tag::FreeInstance[] */
    native void nFreeInstance(long fmu, long instance);
    /* end::FreeInstance[] */

    /* Enter and exit initialization mode, enter event mode, terminate and reset */
    /* tag::EnterInitializationMode[] */
    native Fmi2Status nEnterInitializationMode(long fmu, long instance, boolean toleranceDefined, double tolerance, double startTime, boolean stopTimeDefined,
                                               double stopTime);
    /* end::EnterInitializationMode[] */

    /* tag::ExitInitializationMode[] */
    native Fmi2Status nExitInitializationMode(long instance);
    /* end::ExitInitializationMode[] */

    /* tag::EnterEventMode[] */
    native Fmi2Status nEnterEventMode(long instance, boolean stepEvent, int rootsFound[], int nEventIndicators, boolean timeEvent);
    /* end::EnterEventMode[] */

    /* tag::Terminate[] */
    native Fmi2Status nTerminate(long instance);
    /* end::Terminate[] */

    /* tag::Reset[] */
    native Fmi2Status nReset(long instance);
    /* end::Reset[] */

    /* Getting and setting variable values */
    /* tag::Getters[] */
    native Fmi2Status nGetFloat32(long instance, long valueReferences[], int nValueReferences, double values[], int nValues);

    native Fmi2Status nGetFloat64(long instance, long valueReferences[], int nValueReferences, double values[], int nValues);

    native Fmi2Status nGetInt8(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi2Status nGetUInt8(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi2Status nGetInt16(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi2Status nGetUInt16(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi2Status nGetInt32(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi2Status nGetUInt32(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi2Status nGetInt64(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi2Status nGetUInt64(long instance, long valueReferences[], int nValueReferences, long values[], int nValues);

    native Fmi2Status nGetBoolean(long instance, long valueReferences[], int nValueReferences, boolean values[], int nValues);

    native Fmi2Status nGetString(long instance, long valueReferences[], int nValueReferences, String values[], int nValues);

    native Fmi2Status nGetBinary(long instance, long valueReferences[], int nValueReferences, int sizes[], long values[], int nValues);
    /* end::Getters[] */

    /* tag::Setters[] */
    native Fmi2Status nSetFloat32(long instance, long valueReferences[], int nValueReferences, double values[], int nValues);

    native Fmi2Status nSetFloat64(long instance, long valueReferences[], int nValueReferences, double values[], int nValues);

    native Fmi2Status nSetInt8(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi2Status nSetUInt8(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi2Status nSetInt16(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi2Status nSetUInt16(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi2Status nSetInt32(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi2Status nSetUInt32(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi2Status nSetInt64(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi2Status nSetUInt64(long instance, long valueReferences[], int nValueReferences, long values[], int nValues);

    native Fmi2Status nSetBoolean(long instance, long valueReferences[], int nValueReferences, boolean values[], int nValues);

    native Fmi2Status nSetString(long instance, long valueReferences[], int nValueReferences, String values[], int nValues);

    native Fmi2Status nSetBinary(long instance, long valueReferences[], int nValueReferences, int sizes[], long values[], int nValues);
    /* end::Setters[] */

    /* Getting Variable Dependency Information */
    /* tag::GetNumberOfVariableDependencies[] */
    native Fmi2Status nGetNumberOfVariableDependencies(long instance, long valueReference, int[] nDependencies);
    /* end::GetNumberOfVariableDependencies[] */

    /* tag::GetVariableDependencies[] */
    native Fmi2Status nGetVariableDependencies(long instance, long dependent, int elementIndicesOfDependent[], long independents[],
            int elementIndicesOfIndependents[], int dependencyKinds[], int nDependencies);
    /* end::GetVariableDependencies[] */

    /* Getting and setting the internal FMU state */
    /* tag::GetFMUState[] */
    native Fmi2Status nGetFMUState(long instance, long[] FMUState);
    /* end::GetFMUState[] */

    /* tag::SetFMUState[] */
    native Fmi2Status nSetFMUState(long instance, long FMUState);
    /* end::SetFMUState[] */

    /* tag::FreeFMUState[] */
    native Fmi2Status nFreeFMUState(long instance, long FMUState);
    /* end::FreeFMUState[] */

    /* tag::SerializedFMUStateSize[] */
    native Fmi2Status nSerializedFMUStateSize(long instance, long FMUState, int[] size);
    /* end::SerializedFMUStateSize[] */

    //    /* tag::SerializeFMUState[] */
    //    native Fmi2Status nSerializeFMUState     (long instance,
    //            fmi3FMUState  FMUState,
    //            fmi3Byte serializedState[],
    //            int size);
    //    /* end::SerializeFMUState[] */
    //
    //    /* tag::DeSerializeFMUState[] */
    //    native Fmi2Status nDeSerializeFMUState   (long instance,
    //            fmi3Byte serializedState[],
    //            int size,
    //            fmi3FMUState* FMUState);
    //    /* end::DeSerializeFMUState[] */

    /* Getting partial derivatives */
    /* tag::GetDirectionalDerivative[] */
    native Fmi2Status nGetDirectionalDerivative(long instance, long unknowns[], int nUnknowns, long knowns[], int nKnowns, double seed[], int nSeed,
            double sensitivity[], int nSensitivity);
    /* end::GetDirectionalDerivative[] */

    /* tag::GetAdjointDerivative[] */
    native Fmi2Status nGetAdjointDerivative(long instance, long unknowns[], int nUnknowns, long knowns[], int nKnowns, double seed[], int nSeed,
            double sensitivity[], int nSensitivity);
    /* end::GetAdjointDerivative[] */


    /* Entering and exiting the Configuration or Reconfiguration Mode */
    /* tag::EnterConfigurationMode[] */
    native Fmi2Status nEnterConfigurationMode(long instance);
    /* end::EnterConfigurationMode[] */

    /* tag::ExitConfigurationMode[] */
    native Fmi2Status nExitConfigurationMode(long instance);
    /* end::ExitConfigurationMode[] */

    /* Clock related functions */
    /* tag::GetClock[] */
    native Fmi2Status nGetClock(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);
    /* end::GetClock[] */

    /* tag::SetClock[] */
    native Fmi2Status nSetClock(long instance, long valueReferences[], int nValueReferences, int values[], boolean subactive[], int nValues);
    /* end::SetClock[] */

    /* tag::GetIntervalDecimal[] */
    native Fmi2Status nGetIntervalDecimal(long instance, long valueReferences[], int nValueReferences, double interval[], int nValues);
    /* end::GetIntervalDecimal[] */

    /* tag::GetIntervalFraction[] */
    native Fmi2Status nGetIntervalFraction(long instance, long valueReferences[], int nValueReferences, long intervalCounter[], long resolution[],
            int nValues);
    /* end::GetIntervalFraction[] */

    /* tag::SetIntervalDecimal[] */
    native Fmi2Status nSetIntervalDecimal(long instance, long valueReferences[], int nValueReferences, double interval[], int nValues);
    /* end::SetIntervalDecimal[] */

    /* tag::SetIntervalFraction[] */
    native Fmi2Status nSetIntervalFraction(long instance, long valueReferences[], int nValueReferences, long intervalCounter[], long resolution[],
            int nValues);
    /* end::SetIntervalFraction[] */

    /* tag::UpdateDiscreteStates[] */
    native Fmi2Status nUpdateDiscreteStates(long instance, boolean[] discreteStatesNeedUpdate, boolean[] terminateSimulation,
            boolean[] nominalsOfContinuousStatesChanged, boolean[] valuesOfContinuousStatesChanged, boolean[] nextEventTimeDefined,
            double[] nextEventTime);
    /* end::UpdateDiscreteStates[] */

    /***************************************************
     Types for Functions for Model Exchange
     ****************************************************/

    /* tag::EnterContinuousTimeMode[] */
    native Fmi2Status nEnterContinuousTimeMode(long instance);
    /* end::EnterContinuousTimeMode[] */

    /* tag::CompletedIntegratorStep[] */
    native Fmi2Status nCompletedIntegratorStep(long instance, boolean noSetFMUStatePriorToCurrentPoint, boolean[] enterEventMode,
            boolean[] terminateSimulation);
    /* end::CompletedIntegratorStep[] */

    /* Providing independent variables and re-initialization of caching */
    /* tag::SetTime[] */
    native Fmi2Status nSetTime(long instance, double time);
    /* end::SetTime[] */

    /* tag::SetContinuousStates[] */
    native Fmi2Status nSetContinuousStates(long instance, double continuousStates[], int nContinuousStates);
    /* end::SetContinuousStates[] */

    /* Evaluation of the model equations */
    /* tag::GetDerivatives[] */
    native Fmi2Status nGetDerivatives(long instance, double derivatives[], int nContinuousStates);
    /* end::GetDerivatives[] */

    /* tag::GetEventIndicators[] */
    native Fmi2Status nGetEventIndicators(long instance, double eventIndicators[], int nEventIndicators);
    /* end::GetEventIndicators[] */

    /* tag::GetContinuousStates[] */
    native Fmi2Status nGetContinuousStates(long instance, double continuousStates[], int nContinuousStates);
    /* end::GetContinuousStates[] */

    /* tag::GetNominalsOfContinuousStates[] */
    native Fmi2Status nGetNominalsOfContinuousStates(long instance, double nominals[], int nContinuousStates);
    /* end::GetNominalsOfContinuousStates[] */

    /* tag::GetNumberOfEventIndicators[] */
    native Fmi2Status nGetNumberOfEventIndicators(long instance, int[] nEventIndicators);
    /* end::GetNumberOfEventIndicators[] */

    /* tag::GetNumberOfContinuousStates[] */
    native Fmi2Status nGetNumberOfContinuousStates(long instance, int[] nContinuousStates);
    /* end::GetNumberOfContinuousStates[] */

    /***************************************************
     Types for Functions for Co-Simulation
     ****************************************************/

    /* Simulating the FMU */

    /* tag::EnterStepMode[] */
    native Fmi2Status nEnterStepMode(long instance);
    /* end::EnterStepMode[] */

    /* tag::GetOutputDerivatives[] */
    native Fmi2Status nGetOutputDerivatives(long instance, long valueReferences[], int nValueReferences, int orders[], double values[], int nValues);
    /* end::GetOutputDerivatives[] */

    /* tag::DoStep[] */
    native Fmi2Status nDoStep(long instance, double currentCommunicationPoint, double communicationStepSize, boolean noSetFMUStatePriorToCurrentPoint,
            boolean[] eventEncountered, boolean[] clocksAboutToTick, boolean[] terminate, boolean[] earlyReturn, double[] lastSuccessfulTime);
    /* end::DoStep[] */

    /* tag::ActivateModelPartition[] */
    native Fmi2Status nActivateModelPartition(long instance, long clockReference, int clockElementIndex, double activationTime);

}
