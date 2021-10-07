package org.intocps.fmi3.jnifmuapi;

import org.intocps.fmi3.Fmi3Status;
import org.intocps.fmi3.Fmi3DependencyKind;
import org.intocps.fmi3.Fmi3IntervalQualifier;

public class NativeFmu3Instance {

    final long fmuPtr;

    public NativeFmu3Instance(long fmuPtr) {
        this.fmuPtr = fmuPtr;
    }

    /**
     * size_t nCategories is ignored as it is calculated from categories[].
     *
     * @param instance
     * @param loggingOn
     * @param categories
     * @return
     */
    native Fmi3Status nSetDebugLogging(long fmu, long instance, boolean loggingOn, String categories[]);

    /* tag::FreeInstance[] */
    native void nFreeInstance(long fmu, long instance);
    /* end::FreeInstance[] */

    /* Enter and exit initialization mode, enter event mode, terminate and reset */
    /* tag::EnterInitializationMode[] */
    native Fmi3Status nEnterInitializationMode(long fmu, long instance, boolean toleranceDefined, double tolerance, double startTime, boolean stopTimeDefined,
                                               double stopTime);
    /* end::EnterInitializationMode[] */

    /* tag::ExitInitializationMode[] */
    native Fmi3Status nExitInitializationMode(long fmu, long instance);
    /* end::ExitInitializationMode[] */

    /* tag::EnterEventMode[] */
    native Fmi3Status nEnterEventMode(long fmu, long instance, boolean stepEvent, boolean stateEvent, int rootsFound[], int nEventIndicators, boolean timeEvent);
    /* end::EnterEventMode[] */

    /* tag::Terminate[] */
    native Fmi3Status nTerminate(long fmu, long instance);
    /* end::Terminate[] */

    /* tag::Reset[] */
    native Fmi3Status nReset(long fmu, long instance);
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

    native Fmi3Status nGetBinary(long instance, long valueReferences[], int nValueReferences, long sizes[], byte values[], int nValues);
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

    native Fmi3Status nSetBinary(long instance, long valueReferences[], int nValueReferences, long sizes[], byte array_name[][], int nValues);
    /* end::Setters[] */

    /* Getting Variable Dependency Information */
    /* tag::GetNumberOfVariableDependencies[] */
    native Fmi3Status nGetNumberOfVariableDependencies(long instance, long valueReference, long[] nDependencies);
    /* end::GetNumberOfVariableDependencies[] */

    /* tag::GetVariableDependencies[] */
    native Fmi3Status nGetVariableDependencies(long fmu, long instance, long dependent, long elementIndicesOfDependent[], long independents[],
                                               long elementIndicesOfIndependents[], Fmi3DependencyKind dependencyKinds[], int nDependencies);
    /* end::GetVariableDependencies[] */

    /* Getting and setting the internal FMU state */
    /* tag::GetFMUState[] */
    native Fmi3Status nGetFMUState(long fmu, long instance, long[] FMUState);
    /* end::GetFMUState[] */

    /* tag::SetFMUState[] */
    native Fmi3Status nSetFMUState(long instance, long FMUState);
    /* end::SetFMUState[] */

    /* tag::FreeFMUState[] */
    native Fmi3Status nFreeFMUState(long instance, long FMUState);
    /* end::FreeFMUState[] */

    /* tag::SerializedFMUStateSize[] */
    native Fmi3Status nSerializedFMUStateSize(long fmu, long instance, long FMUState, long[] size);
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
    native Fmi3Status nGetDirectionalDerivative(long fmu, long instance, long unknowns[], int nUnknowns, long knowns[], int nKnowns, double seed[], int nSeed,
                                                double sensitivity[], int nSensitivity);
    /* end::GetDirectionalDerivative[] */

    /* tag::GetAdjointDerivative[] */
    native Fmi3Status nGetAdjointDerivative(long fmu, long instance, long unknowns[], int nUnknowns, long knowns[], int nKnowns, double seed[], int nSeed,
                                            double sensitivity[], int nSensitivity);
    /* end::GetAdjointDerivative[] */


    /* Entering and exiting the Configuration or Reconfiguration Mode */
    /* tag::EnterConfigurationMode[] */
    native Fmi3Status nEnterConfigurationMode(long fmu, long instance);
    /* end::EnterConfigurationMode[] */

    /* tag::ExitConfigurationMode[] */
    native Fmi3Status nExitConfigurationMode(long fmu, long instance);
    /* end::ExitConfigurationMode[] */

    /* Clock related functions */
    /* tag::GetClock[] */
    native Fmi3Status nGetClock(long fmu, long instance, long valueReferences[], int nValueReferences, boolean values[], int nValues);
    /* end::GetClock[] */

    /* tag::SetClock[] */
    native Fmi3Status nSetClock(long fmu, long instance, long valueReferences[], int nValueReferences, boolean values[], int nValues);
    /* end::SetClock[] */

    /* tag::GetIntervalDecimal[] */
    native Fmi3Status nGetIntervalDecimal(long fmu, long instance, long valueReferences[], int nValueReferences, double intervals[], Fmi3IntervalQualifier qualifiers[], int nIntervals);
    /* end::GetIntervalDecimal[] */

    /* tag::GetIntervalFraction[] */
    native Fmi3Status nGetIntervalFraction(long fmu, long instance, long valueReferences[], int nValueReferences, long intervalCounters[], long resolutions[], int nIntervals);
    /* end::GetIntervalFraction[] */

    /* tag::SetIntervalDecimal[] */
    native Fmi3Status nSetIntervalDecimal(long fmu, long instance, long valueReferences[], int nValueReferences, double interval[], int nValues);
    /* end::SetIntervalDecimal[] */

    /* tag::SetIntervalFraction[] */
    native Fmi3Status nSetIntervalFraction(long fmu, long instance, long valueReferences[], int nValueReferences, long intervalCounter[], long resolution[],
                                           int nValues);
    /* end::SetIntervalFraction[] */

    /* tag::UpdateDiscreteStates[] */
    native Fmi3Status nUpdateDiscreteStates(long fmu, long instance, boolean[] discreteStatesNeedUpdate, boolean[] terminateSimulation,
                                            boolean[] nominalsOfContinuousStatesChanged, boolean[] valuesOfContinuousStatesChanged, boolean[] nextEventTimeDefined,
                                            double[] nextEventTime);
    /* end::UpdateDiscreteStates[] */

    /***************************************************
     Types for Functions for Model Exchange
     ****************************************************/

    /* tag::EnterContinuousTimeMode[] */
    native Fmi3Status nEnterContinuousTimeMode(long fmu, long instance);
    /* end::EnterContinuousTimeMode[] */

    /* tag::CompletedIntegratorStep[] */
    native Fmi3Status nCompletedIntegratorStep(long fmu, long instance, boolean noSetFMUStatePriorToCurrentPoint, boolean[] enterEventMode,
                                               boolean[] terminateSimulation);
    /* end::CompletedIntegratorStep[] */

    /* Providing independent variables and re-initialization of caching */
    /* tag::SetTime[] */
    native Fmi3Status nSetTime(long fmu, long instance, double time);
    /* end::SetTime[] */

    /* tag::SetContinuousStates[] */
    native Fmi3Status nSetContinuousStates(long fmu, long instance, double continuousStates[], int nContinuousStates);
    /* end::SetContinuousStates[] */

    /* Evaluation of the model equations */
    /* tag::GetDerivatives[] */
    native Fmi3Status nGetDerivatives(long instance, double derivatives[], int nContinuousStates); //TODO: Is this a FMI3 method?
    /* end::GetDerivatives[] */

    /* tag::GetEventIndicators[] */
    native Fmi3Status nGetEventIndicators(long fmu, long instance, double eventIndicators[], int nEventIndicators);
    /* end::GetEventIndicators[] */

    /* tag::GetContinuousStates[] */
    native Fmi3Status nGetContinuousStates(long fmu, long instance, double continuousStates[], int nContinuousStates);
    /* end::GetContinuousStates[] */

    /* tag::GetNominalsOfContinuousStates[] */
    native Fmi3Status nGetNominalsOfContinuousStates(long fmu, long instance, double nominals[], int nContinuousStates);
    /* end::GetNominalsOfContinuousStates[] */

    /* tag::GetNumberOfEventIndicators[] */
    native Fmi3Status nGetNumberOfEventIndicators(long fmu, long instance, long[] nEventIndicators);
    /* end::GetNumberOfEventIndicators[] */

    /* tag::GetNumberOfContinuousStates[] */
    native Fmi3Status nGetNumberOfContinuousStates(long fmu, long instance, long[] nContinuousStates);
    /* end::GetNumberOfContinuousStates[] */

    /***************************************************
     Types for Functions for Co-Simulation
     ****************************************************/

    /* Simulating the FMU */

    /* tag::EnterStepMode[] */
    native Fmi3Status nEnterStepMode(long fmu, long instance);
    /* end::EnterStepMode[] */

    /* tag::GetOutputDerivatives[] */
    native Fmi3Status nGetOutputDerivatives(long fmu, long instance, long valueReferences[], int nValueReferences, int orders[], double values[], int nValues);
    /* end::GetOutputDerivatives[] */

    /* tag::DoStep[] */
    native Fmi3Status nDoStep(long fmu, long instance, double currentCommunicationPoint, double communicationStepSize, boolean noSetFMUStatePriorToCurrentPoint,
                              boolean[] eventEncountered, boolean[] terminate, boolean[] earlyReturn, double[] lastSuccessfulTime);
    /* end::DoStep[] */

    /* tag::ActivateModelPartition[] */
    native Fmi3Status nActivateModelPartition(long fmu, long instance, long clockReference, int clockElementIndex, double activationTime);

}
