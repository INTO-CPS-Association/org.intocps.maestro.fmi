package org.intocps.fmi.jnifmuapi.fmi3;

import org.intocps.fmi.FmiInvalidNativeStateException;

public interface IFmi3Instance {


    //    enum IntervalQualifier {
    //        fmi3IntervalNotYetKnown(0),
    //        fmi3IntervalUnchanged(1),
    //        fmi3IntervalChanged(2);
    //
    //        private final int value;
    //
    //        IntervalQualifier(final int newValue) {
    //            value = newValue;
    //        }
    //
    //        public int getValue() {
    //            return value;
    //        }
    //    }


    boolean isValid();

    Fmi3Status terminate() throws FmiInvalidNativeStateException;


    Fmi3Status reset() throws FmiInvalidNativeStateException;

    void freeInstance() throws FmiInvalidNativeStateException;

    Fmi3Status enterInitializationMode(Double tolerance, double startTime, Double stopTime) throws FmiInvalidNativeStateException;

    Fmi3Status exitInitializationMode() throws FmiInvalidNativeStateException;

    Fmi3Status enterEventMode() throws FmiInvalidNativeStateException;

    Fmi3Status enterConfigurationMode() throws FmiInvalidNativeStateException;

    Fmi3Status exitConfigurationMode() throws FmiInvalidNativeStateException;

    FmuResult<DoStepResult> doStep(double currentCommunicationPoint, double communicationStepSize,
            boolean noSetFMUStatePriorToCurrentPoint) throws FmiInvalidNativeStateException;


    Fmi3Status doStep(double currentCommunicationPoint, double communicationStepSize, boolean noSetFMUStatePriorToCurrentPoint,
            boolean[] eventEncountered, boolean[] terminateSimulation, boolean[] earlyReturn,
            double[] lastSuccessfulTime) throws FmiInvalidNativeStateException;

    Fmi3Status setDebugLogging(boolean loggingOn, String... categories) throws FmiInvalidNativeStateException;

    FmuResult<Long> getNumberOfVariableDependencies(long valueReference) throws FmiInvalidNativeStateException;

    Fmi3Status setClock(long[] valueReferences, boolean[] clockActive) throws FmiInvalidNativeStateException;

    FmuResult<boolean[]> getClock(long[] valueReferences) throws FmiInvalidNativeStateException;

    class VariableDependency {
        long[] elementIndicesOfDependent;
        long[] independents;
        long[] elementIndicesOfIndependents;
        Fmi3DependencyKind[] dependencyKinds;

        public long[] getElementIndicesOfDependent() {
            return elementIndicesOfDependent;
        }

        public long[] getIndependents() {
            return independents;
        }

        public long[] getElementIndicesOfIndependents() {
            return elementIndicesOfIndependents;
        }

        public Fmi3DependencyKind[] getDependencyKinds() {
            return dependencyKinds;
        }

        public VariableDependency(int size) {
            this.dependencyKinds = new Fmi3DependencyKind[size];
            this.elementIndicesOfDependent = new long[size];
            this.elementIndicesOfIndependents = new long[size];
            this.independents = new long[size];
        }
    }

    FmuResult<VariableDependency> getVariableDependencies(long dependent, long nDependencies) throws FmiInvalidNativeStateException;


    FmuResult<Fmi3State> getState() throws FmiInvalidNativeStateException;

    Fmi3Status setState(Fmi3State state) throws FmiInvalidNativeStateException;

    Fmi3Status freeState(Fmi3State state) throws FmiInvalidNativeStateException;

    FmuResult<Long> getSerializedStateSize(Fmi3State state) throws FmiInvalidNativeStateException;

    FmuResult<byte[]> serializedState(Fmi3State state, long size) throws FmiInvalidNativeStateException;

    FmuResult<Fmi3State> deSerializedState(byte[] serializedState) throws FmiInvalidNativeStateException;


    FmuResult<double[]> getDirectionalDerivative(long unknowns[], long knowns[], double seed[]) throws FmiInvalidNativeStateException;

    FmuResult<double[]> getGetAdjointDerivative(long unknowns[], long knowns[], double seed[],
            int nSensitivity) throws FmiInvalidNativeStateException;


    class GetIntervalDecimalResponse {
        double[] intervals;
        Fmi3IntervalQualifier[] qualifiers;

        public double[] getIntervals() {
            return intervals;
        }

        public Fmi3IntervalQualifier[] getQualifiers() {
            return qualifiers;
        }

        public GetIntervalDecimalResponse(int size) {
            this.intervals = new double[size];
            this.qualifiers = new Fmi3IntervalQualifier[size];
        }
    }

    FmuResult<GetIntervalDecimalResponse> getIntervalDecimal(long[] valueReferences) throws FmiInvalidNativeStateException;

    class IntervalFractionResponse {
        long[] intervalCounters;
        long[] resolutions;
        Fmi3IntervalQualifier[] qualifiers;

        public long[] getIntervalCounters() {
            return intervalCounters;
        }

        public long[] getResolutions() {
            return resolutions;
        }

        public Fmi3IntervalQualifier[] getQualifiers() {
            return qualifiers;
        }

        public IntervalFractionResponse(int size) {
            this.intervalCounters = new long[size];
            this.resolutions = new long[size];
            this.qualifiers = new Fmi3IntervalQualifier[size];
        }
    }

    FmuResult<IntervalFractionResponse> getIntervalFraction(long[] valueReferences) throws FmiInvalidNativeStateException;

    FmuResult<double[]> getShiftDecimal(long[] valueReferences) throws FmiInvalidNativeStateException;


    class GetShiftFractionResponse {
        long[] shiftCounters;
        long[] resolutions;

        public long[] getShiftCounters() {
            return shiftCounters;
        }

        public long[] getResolutions() {
            return resolutions;
        }

        public GetShiftFractionResponse(int size) {
            this.shiftCounters = new long[size];
            this.resolutions = new long[size];
        }
    }

    FmuResult<GetShiftFractionResponse> getShiftFraction(long[] valueReferences) throws FmiInvalidNativeStateException;

    Fmi3Status setIntervalDecimal(long[] valueReferences, double[] intervals) throws FmiInvalidNativeStateException;

    Fmi3Status setIntervalFraction(long[] valueReferences, long[] intervalCounters, long[] resolutions) throws FmiInvalidNativeStateException;

    Fmi3Status setShiftDecimal(long[] valueReferences, double[] shifts);

    Fmi3Status setShiftFraction(long[] valueReferences, long[] counters, long[] resolutions);


    Fmi3Status evaluateDiscreteStates() throws FmiInvalidNativeStateException;

    class UpdateDiscreteStates {
        boolean discreteStatesNeedUpdate;
        boolean terminateSimulation;
        boolean nominalsOfContinuousStatesChanged;
        boolean valuesOfContinuousStatesChanged;
        boolean nextEventTimeDefined;
        double nextEventTime;

        public boolean isDiscreteStatesNeedUpdate() {
            return discreteStatesNeedUpdate;
        }

        public boolean isTerminateSimulation() {
            return terminateSimulation;
        }

        public boolean isNominalsOfContinuousStatesChanged() {
            return nominalsOfContinuousStatesChanged;
        }

        public boolean isValuesOfContinuousStatesChanged() {
            return valuesOfContinuousStatesChanged;
        }

        public boolean isNextEventTimeDefined() {
            return nextEventTimeDefined;
        }

        public double getNextEventTime() {
            return nextEventTime;
        }

        public UpdateDiscreteStates(boolean discreteStatesNeedUpdate, boolean terminateSimulation, boolean nominalsOfContinuousStatesChanged,
                boolean valuesOfContinuousStatesChanged, boolean nextEventTimeDefined, double nextEventTime) {
            this.discreteStatesNeedUpdate = discreteStatesNeedUpdate;
            this.terminateSimulation = terminateSimulation;
            this.nominalsOfContinuousStatesChanged = nominalsOfContinuousStatesChanged;
            this.valuesOfContinuousStatesChanged = valuesOfContinuousStatesChanged;
            this.nextEventTimeDefined = nextEventTimeDefined;
            this.nextEventTime = nextEventTime;
        }
    }

    FmuResult<UpdateDiscreteStates> updateDiscreteStates() throws FmiInvalidNativeStateException;

    Fmi3Status enterContinuousTimeMode() throws FmiInvalidNativeStateException;

    class CompletedIntegratorStepResponse {
        boolean enterEventMode;
        boolean terminateSimulation;

        public boolean isEnterEventMode() {
            return enterEventMode;
        }

        public boolean isTerminateSimulation() {
            return terminateSimulation;
        }

        public CompletedIntegratorStepResponse(boolean enterEventMode, boolean terminateSimulation) {
            this.enterEventMode = enterEventMode;
            this.terminateSimulation = terminateSimulation;
        }
    }

    FmuResult<CompletedIntegratorStepResponse> completedIntegratorStep(
            boolean noSetFMUStatePriorToCurrentPoint) throws FmiInvalidNativeStateException;

    Fmi3Status setTime(double time) throws FmiInvalidNativeStateException;

    Fmi3Status setContinuousStates(double[] continuousStates) throws FmiInvalidNativeStateException;


    FmuResult<double[]> getContinuousStateDerivatives(int nContinuousStates) throws FmiInvalidNativeStateException;

    FmuResult<double[]> getGetEventIndicators(int nEventIndicators) throws FmiInvalidNativeStateException;

    FmuResult<double[]> getGetContinuousStates(int nContinuousStates) throws FmiInvalidNativeStateException;

    FmuResult<double[]> getGetNominalsOfContinuousStates(int nContinuousStates) throws FmiInvalidNativeStateException;


    FmuResult<Long> getNumberOfEventIndicators() throws FmiInvalidNativeStateException;

    FmuResult<Long> getNumberOfContinuousStates() throws FmiInvalidNativeStateException;

    Fmi3Status enterStepMode() throws FmiInvalidNativeStateException;

    FmuResult<double[]> getOutputDerivatives(long[] valueReferences, int[] orders) throws FmiInvalidNativeStateException;


    class DoStepResult {
        final boolean eventHandlingNeeded;
        final boolean terminateSimulation;
        final boolean earlyReturn;

        public boolean isEventHandlingNeeded() {
            return eventHandlingNeeded;
        }

        public boolean isTerminateSimulation() {
            return terminateSimulation;
        }

        public boolean isEarlyReturn() {
            return earlyReturn;
        }

        public double getLastSuccessfulTime() {
            return lastSuccessfulTime;
        }

        public DoStepResult(boolean eventHandlingNeeded, boolean terminateSimulation, boolean earlyReturn, double lastSuccessfulTime) {
            this.eventHandlingNeeded = eventHandlingNeeded;
            this.terminateSimulation = terminateSimulation;
            this.earlyReturn = earlyReturn;
            this.lastSuccessfulTime = lastSuccessfulTime;
        }

        final double lastSuccessfulTime;
    }

    Fmi3Status activateModelPartition(long clockReference, double activationTime) throws FmiInvalidNativeStateException;


    FmuResult<float[]> getFloat32(long[] valueReferences) throws FmiInvalidNativeStateException;

    FmuResult<double[]> getFloat64(long[] valueReferences) throws FmiInvalidNativeStateException;

    FmuResult<byte[]> getInt8(long[] valueReferences) throws FmiInvalidNativeStateException;

    FmuResult<byte[]> getUInt8(long[] valueReferences) throws FmiInvalidNativeStateException;

    FmuResult<short[]> getInt16(long[] valueReferences) throws FmiInvalidNativeStateException;

    FmuResult<short[]> getUInt16(long[] valueReferences) throws FmiInvalidNativeStateException;

    FmuResult<int[]> getInt32(long[] valueReferences) throws FmiInvalidNativeStateException;

    FmuResult<int[]> getUInt32(long[] valueReferences) throws FmiInvalidNativeStateException;

    FmuResult<long[]> getInt64(long[] valueReferences) throws FmiInvalidNativeStateException;

    FmuResult<long[]> getUInt64(long[] valueReferences) throws FmiInvalidNativeStateException;

    FmuResult<boolean[]> getBoolean(long[] valueReferences) throws FmiInvalidNativeStateException;

    FmuResult<String[]> getString(long[] valueReferences) throws FmiInvalidNativeStateException;

    FmuResult<byte[][]> getBinary(long[] valueReferences) throws FmiInvalidNativeStateException;
    /* end::Getters[] */

    /* tag::Setters[] */
    Fmi3Status setFloat32(long[] valueReferences, float[] values) throws FmiInvalidNativeStateException;

    Fmi3Status setFloat64(long[] valueReferences, double[] values) throws FmiInvalidNativeStateException;

    Fmi3Status setInt8(long[] valueReferences, byte[] values) throws FmiInvalidNativeStateException;

    Fmi3Status setUInt8(long[] valueReferences, byte[] values) throws FmiInvalidNativeStateException;

    Fmi3Status setInt16(long[] valueReferences, short[] values) throws FmiInvalidNativeStateException;

    Fmi3Status setUInt16(long[] valueReferences, short[] values) throws FmiInvalidNativeStateException;

    Fmi3Status setInt32(long[] valueReferences, int[] values) throws FmiInvalidNativeStateException;

    Fmi3Status setUInt32(long[] valueReferences, int[] values) throws FmiInvalidNativeStateException;

    Fmi3Status setInt64(long[] valueReferences, long[] values) throws FmiInvalidNativeStateException;

    Fmi3Status setUInt64(long[] valueReferences, long[] values) throws FmiInvalidNativeStateException;

    Fmi3Status setBoolean(long[] valueReferences, boolean[] values) throws FmiInvalidNativeStateException;

    Fmi3Status setString(long[] valueReferences, String[] values) throws FmiInvalidNativeStateException;

    Fmi3Status setBinary(long[] valueReferences, byte[][] array_name) throws FmiInvalidNativeStateException;
}
