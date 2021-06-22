package org.intocps.fmi3.jnifmuapi;


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
    native byte nSetDebugLogging(long fmu, long instance, boolean loggingOn, String categories[]);

    /* tag::FreeInstance[] */
    native void nFreeInstance(long fmu, long instance);
    /* end::FreeInstance[] */

    /* Enter and exit initialization mode, enter event mode, terminate and reset */
    /* tag::EnterInitializationMode[] */
    native byte nEnterInitializationMode(long fmu, long instance, boolean toleranceDefined, double tolerance, double startTime, boolean stopTimeDefined,
            double stopTime);
    /* end::EnterInitializationMode[] */

    /* tag::ExitInitializationMode[] */
    native byte nExitInitializationMode(long instance);
    /* end::ExitInitializationMode[] */

    /* tag::EnterEventMode[] */
    native byte nEnterEventMode(long instance, boolean stepEvent, int rootsFound[], int nEventIndicators, boolean timeEvent);
    /* end::EnterEventMode[] */

    /* tag::Terminate[] */
    native byte nTerminate(long instance);
    /* end::Terminate[] */

    /* tag::Reset[] */
    native byte nReset(long instance);
    /* end::Reset[] */

    /* Getting and setting variable values */
    /* tag::Getters[] */
    native byte nGetFloat32(long instance, long valueReferences[], int nValueReferences, double values[], int nValues);

    native byte nGetFloat64(long instance, long valueReferences[], int nValueReferences, double values[], int nValues);

    native byte nGetInt8(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native byte nGetUInt8(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native byte nGetInt16(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native byte nGetUInt16(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native byte nGetInt32(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native byte nGetUInt32(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native byte nGetInt64(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native byte nGetUInt64(long instance, long valueReferences[], int nValueReferences, long values[], int nValues);

    native byte nGetBoolean(long instance, long valueReferences[], int nValueReferences, boolean values[], int nValues);

    native byte nGetString(long instance, long valueReferences[], int nValueReferences, String values[], int nValues);

    native byte nGetBinary(long instance, long valueReferences[], int nValueReferences, int sizes[], long values[], int nValues);
    /* end::Getters[] */

    /* tag::Setters[] */
    native byte nSetFloat32(long instance, long valueReferences[], int nValueReferences, double values[], int nValues);

    native byte nSetFloat64(long instance, long valueReferences[], int nValueReferences, double values[], int nValues);

    native byte nSetInt8(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native byte nSetUInt8(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native byte nSetInt16(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native byte nSetUInt16(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native byte nSetInt32(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native byte nSetUInt32(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native byte nSetInt64(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native byte nSetUInt64(long instance, long valueReferences[], int nValueReferences, long values[], int nValues);

    native byte nSetBoolean(long instance, long valueReferences[], int nValueReferences, boolean values[], int nValues);

    native byte nSetString(long instance, long valueReferences[], int nValueReferences, String values[], int nValues);

    native byte nSetBinary(long instance, long valueReferences[], int nValueReferences, int sizes[], long values[], int nValues);
    /* end::Setters[] */

    /* Getting Variable Dependency Information */
    /* tag::GetNumberOfVariableDependencies[] */
    native byte nGetNumberOfVariableDependencies(long instance, long valueReference, int[] nDependencies);
    /* end::GetNumberOfVariableDependencies[] */

    /* tag::GetVariableDependencies[] */
    native byte nGetVariableDependencies(long instance, long dependent, int elementIndicesOfDependent[], long independents[],
            int elementIndicesOfIndependents[], int dependencyKinds[], int nDependencies);
    /* end::GetVariableDependencies[] */

    /* Getting and setting the internal FMU state */
    /* tag::GetFMUState[] */
    native byte nGetFMUState(long instance, long[] FMUState);
    /* end::GetFMUState[] */

    /* tag::SetFMUState[] */
    native byte nSetFMUState(long instance, long FMUState);
    /* end::SetFMUState[] */

    /* tag::FreeFMUState[] */
    native byte nFreeFMUState(long instance, long FMUState);
    /* end::FreeFMUState[] */

    /* tag::SerializedFMUStateSize[] */
    native byte nSerializedFMUStateSize(long instance, long FMUState, int[] size);
    /* end::SerializedFMUStateSize[] */

    //    /* tag::SerializeFMUState[] */
    //    native byte nSerializeFMUState     (long instance,
    //            fmi3FMUState  FMUState,
    //            fmi3Byte serializedState[],
    //            int size);
    //    /* end::SerializeFMUState[] */
    //
    //    /* tag::DeSerializeFMUState[] */
    //    native byte nDeSerializeFMUState   (long instance,
    //            fmi3Byte serializedState[],
    //            int size,
    //            fmi3FMUState* FMUState);
    //    /* end::DeSerializeFMUState[] */

    /* Getting partial derivatives */
    /* tag::GetDirectionalDerivative[] */
    native byte nGetDirectionalDerivative(long instance, long unknowns[], int nUnknowns, long knowns[], int nKnowns, double seed[], int nSeed,
            double sensitivity[], int nSensitivity);
    /* end::GetDirectionalDerivative[] */

    /* tag::GetAdjointDerivative[] */
    native byte nGetAdjointDerivative(long instance, long unknowns[], int nUnknowns, long knowns[], int nKnowns, double seed[], int nSeed,
            double sensitivity[], int nSensitivity);
    /* end::GetAdjointDerivative[] */


    /* Entering and exiting the Configuration or Reconfiguration Mode */
    /* tag::EnterConfigurationMode[] */
    native byte nEnterConfigurationMode(long instance);
    /* end::EnterConfigurationMode[] */

    /* tag::ExitConfigurationMode[] */
    native byte nExitConfigurationMode(long instance);
    /* end::ExitConfigurationMode[] */

    /* Clock related functions */
    /* tag::GetClock[] */
    native byte nGetClock(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);
    /* end::GetClock[] */

    /* tag::SetClock[] */
    native byte nSetClock(long instance, long valueReferences[], int nValueReferences, int values[], boolean subactive[], int nValues);
    /* end::SetClock[] */

    /* tag::GetIntervalDecimal[] */
    native byte nGetIntervalDecimal(long instance, long valueReferences[], int nValueReferences, double interval[], int nValues);
    /* end::GetIntervalDecimal[] */

    /* tag::GetIntervalFraction[] */
    native byte nGetIntervalFraction(long instance, long valueReferences[], int nValueReferences, long intervalCounter[], long resolution[],
            int nValues);
    /* end::GetIntervalFraction[] */

    /* tag::SetIntervalDecimal[] */
    native byte nSetIntervalDecimal(long instance, long valueReferences[], int nValueReferences, double interval[], int nValues);
    /* end::SetIntervalDecimal[] */

    /* tag::SetIntervalFraction[] */
    native byte nSetIntervalFraction(long instance, long valueReferences[], int nValueReferences, long intervalCounter[], long resolution[],
            int nValues);
    /* end::SetIntervalFraction[] */

    /* tag::UpdateDiscreteStates[] */
    native byte nUpdateDiscreteStates(long instance, boolean[] discreteStatesNeedUpdate, boolean[] terminateSimulation,
            boolean[] nominalsOfContinuousStatesChanged, boolean[] valuesOfContinuousStatesChanged, boolean[] nextEventTimeDefined,
            double[] nextEventTime);
    /* end::UpdateDiscreteStates[] */

    /***************************************************
     Types for Functions for Model Exchange
     ****************************************************/

    /* tag::EnterContinuousTimeMode[] */
    native byte nEnterContinuousTimeMode(long instance);
    /* end::EnterContinuousTimeMode[] */

    /* tag::CompletedIntegratorStep[] */
    native byte nCompletedIntegratorStep(long instance, boolean noSetFMUStatePriorToCurrentPoint, boolean[] enterEventMode,
            boolean[] terminateSimulation);
    /* end::CompletedIntegratorStep[] */

    /* Providing independent variables and re-initialization of caching */
    /* tag::SetTime[] */
    native byte nSetTime(long instance, double time);
    /* end::SetTime[] */

    /* tag::SetContinuousStates[] */
    native byte nSetContinuousStates(long instance, double continuousStates[], int nContinuousStates);
    /* end::SetContinuousStates[] */

    /* Evaluation of the model equations */
    /* tag::GetDerivatives[] */
    native byte nGetDerivatives(long instance, double derivatives[], int nContinuousStates);
    /* end::GetDerivatives[] */

    /* tag::GetEventIndicators[] */
    native byte nGetEventIndicators(long instance, double eventIndicators[], int nEventIndicators);
    /* end::GetEventIndicators[] */

    /* tag::GetContinuousStates[] */
    native byte nGetContinuousStates(long instance, double continuousStates[], int nContinuousStates);
    /* end::GetContinuousStates[] */

    /* tag::GetNominalsOfContinuousStates[] */
    native byte nGetNominalsOfContinuousStates(long instance, double nominals[], int nContinuousStates);
    /* end::GetNominalsOfContinuousStates[] */

    /* tag::GetNumberOfEventIndicators[] */
    native byte nGetNumberOfEventIndicators(long instance, int[] nEventIndicators);
    /* end::GetNumberOfEventIndicators[] */

    /* tag::GetNumberOfContinuousStates[] */
    native byte nGetNumberOfContinuousStates(long instance, int[] nContinuousStates);
    /* end::GetNumberOfContinuousStates[] */

    /***************************************************
     Types for Functions for Co-Simulation
     ****************************************************/

    /* Simulating the FMU */

    /* tag::EnterStepMode[] */
    native byte nEnterStepMode(long instance);
    /* end::EnterStepMode[] */

    /* tag::GetOutputDerivatives[] */
    native byte nGetOutputDerivatives(long instance, long valueReferences[], int nValueReferences, int orders[], double values[], int nValues);
    /* end::GetOutputDerivatives[] */

    /* tag::DoStep[] */
    native byte nDoStep(long instance, double currentCommunicationPoint, double communicationStepSize, boolean noSetFMUStatePriorToCurrentPoint,
            boolean[] eventEncountered, boolean[] clocksAboutToTick, boolean[] terminate, boolean[] earlyReturn, double[] lastSuccessfulTime);
    /* end::DoStep[] */

    /* tag::ActivateModelPartition[] */
    native byte nActivateModelPartition(long instance, long clockReference, int clockElementIndex, double activationTime);

}
