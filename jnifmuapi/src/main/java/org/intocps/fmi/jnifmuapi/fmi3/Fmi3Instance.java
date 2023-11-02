package org.intocps.fmi.jnifmuapi.fmi3;

import org.intocps.fmi.FmiInvalidNativeStateException;

public class Fmi3Instance extends NativeFmu3Instance implements IFmi3Instance {

    private final long instancePtr;
    private final DirectoryFmi3Fmu fmu;
    private final long fmuPtr;

    public Fmi3Instance(long instancePtr, DirectoryFmi3Fmu fmi3Fmu) {
        this.instancePtr = instancePtr;
        this.fmu = fmi3Fmu;
        this.fmuPtr = fmi3Fmu.getFmuPtr();
    }

    @Override
    public Fmi3Status terminate() throws FmiInvalidNativeStateException {
        checkState();
        return this.nTerminate(instancePtr);
    }


    @Override
    public Fmi3Status reset() throws FmiInvalidNativeStateException {
        checkState();
        return this.nReset(instancePtr);
    }

    @Override
    public void freeInstance() throws FmiInvalidNativeStateException {
        checkState();
        this.nFreeInstance(instancePtr);
    }

    @Override
    public Fmi3Status enterInitializationMode(Double tolerance, double startTime, Double stopTime) throws FmiInvalidNativeStateException {
        checkState();
        return this.nEnterInitializationMode(instancePtr, tolerance != null, tolerance == null ? 0 : tolerance, startTime, stopTime != null,
                stopTime == null ? 0 : stopTime);
    }

    @Override
    public Fmi3Status exitInitializationMode() throws FmiInvalidNativeStateException {
        checkState();
        return this.nExitInitializationMode(instancePtr);
    }

    @Override
    public Fmi3Status enterEventMode() throws FmiInvalidNativeStateException {
        checkState();

        return this.nEnterEventMode(instancePtr);
    }

    @Override
    public Fmi3Status enterConfigurationMode() throws FmiInvalidNativeStateException {
        checkState();
        return this.nEnterConfigurationMode(instancePtr);
    }

    @Override
    public Fmi3Status exitConfigurationMode() throws FmiInvalidNativeStateException {
        checkState();
        return this.nExitConfigurationMode(instancePtr);
    }

    @Override
    public FmuResult<DoStepResult> doStep(double currentCommunicationPoint, double communicationStepSize,
            boolean noSetFMUStatePriorToCurrentPoint) throws FmiInvalidNativeStateException {

        boolean[] eventHandlingNeeded = new boolean[1];
        boolean[] terminateSimulation = new boolean[1];
        boolean[] earlyReturn = new boolean[1];
        double[] lastSuccessfulTime = new double[1];

        return new FmuResult<>(this.doStep(currentCommunicationPoint, communicationStepSize, noSetFMUStatePriorToCurrentPoint, eventHandlingNeeded,
                terminateSimulation, earlyReturn, lastSuccessfulTime),
                new DoStepResult(eventHandlingNeeded[0], terminateSimulation[0], earlyReturn[0], lastSuccessfulTime[0]));
    }

    @Override
    public Fmi3Status doStep(double currentCommunicationPoint, double communicationStepSize, boolean noSetFMUStatePriorToCurrentPoint,
            boolean[] eventHandlingNeeded, boolean[] terminateSimulation, boolean[] earlyReturn,
            double[] lastSuccessfulTime) throws FmiInvalidNativeStateException {
        checkState();
        if (eventHandlingNeeded == null || eventHandlingNeeded.length < 1) {
            eventHandlingNeeded = new boolean[1];
        }

        if (terminateSimulation == null || terminateSimulation.length < 1) {
            terminateSimulation = new boolean[1];
        }

        if (earlyReturn == null || earlyReturn.length < 1) {
            earlyReturn = new boolean[1];
        }

        if (lastSuccessfulTime == null || lastSuccessfulTime.length < 1) {
            lastSuccessfulTime = new double[1];
        }

        return this.nDoStep(instancePtr, currentCommunicationPoint, communicationStepSize, noSetFMUStatePriorToCurrentPoint, eventHandlingNeeded,
                terminateSimulation, earlyReturn, lastSuccessfulTime);
    }

    @Override
    public Fmi3Status setDebugLogging(boolean loggingOn, String... categories) throws FmiInvalidNativeStateException {
        checkState();
        if (categories == null) {
            categories = new String[0];
        }

        return nSetDebugLogging(instancePtr, loggingOn, categories);
    }

    @Override
    public FmuResult<Long> getNumberOfVariableDependencies(long valueReference) throws FmiInvalidNativeStateException {
        checkState();
        long[] ret = new long[1];
        Fmi3Status status = nGetNumberOfVariableDependencies(instancePtr, valueReference, ret);
        return new FmuResult<>(status, ret[0]);
    }

    @Override
    public Fmi3Status setClock(long[] valueReferences, boolean[] clockActive) throws FmiInvalidNativeStateException {
        checkState();
        if (valueReferences == null) {
            valueReferences = new long[0];
        }
        if (clockActive == null) {
            clockActive = new boolean[0];
        }

        return nSetClock(instancePtr, valueReferences, valueReferences.length, clockActive);
    }

    @Override
    public FmuResult<boolean[]> getClock(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();
        if (valueReferences == null) {
            valueReferences = new long[0];
        }
        boolean[] result = new boolean[valueReferences.length];
        return new FmuResult<>(nGetClock(instancePtr, valueReferences, valueReferences.length, result), result);
    }

    @Override
    public FmuResult<VariableDependency> getVariableDependencies(long dependent, long nDependencies) throws FmiInvalidNativeStateException {
        checkState();

        VariableDependency dependency = new VariableDependency((int) nDependencies);

        Fmi3Status status = nGetVariableDependencies(instancePtr, dependent, dependency.elementIndicesOfDependent, dependency.independents,
                dependency.elementIndicesOfIndependents, dependency.dependencyKinds, nDependencies);
        return new FmuResult<>(status, dependency);
    }

    abstract class Fmu3StateImpl implements Fmi3State {
        final long ptr;

        public Fmu3StateImpl(long ptr) {
            this.ptr = ptr;
        }
    }

    @Override
    public FmuResult<Fmi3State> getState() throws FmiInvalidNativeStateException {
        checkState();

        long[] s = new long[1];
        Fmi3Status status = nGetFMUState(instancePtr, s);
        return new FmuResult<>(status, new Fmu3StateImpl(s[0]) {
            @Override
            public Fmi3Status free() throws FmiInvalidNativeStateException {
                return freeState(this);
            }
        });
    }

    @Override
    public Fmi3Status setState(Fmi3State state) throws FmiInvalidNativeStateException {
        checkState();

        if (state instanceof Fmu3StateImpl) {

            return nSetFMUState(instancePtr, ((Fmu3StateImpl) state).ptr);
        }
        return Fmi3Status.Error;
    }

    @Override
    public Fmi3Status freeState(Fmi3State state) throws FmiInvalidNativeStateException {
        checkState();
        if (state instanceof Fmu3StateImpl) {

            return nFreeFMUState(instancePtr, ((Fmu3StateImpl) state).ptr);
        }
        return Fmi3Status.Error;
    }

    @Override
    public FmuResult<Long> getSerializedStateSize(Fmi3State state) throws FmiInvalidNativeStateException {
        checkState();
        if (state instanceof Fmu3StateImpl) {

            long[] size = new long[1];
            Fmi3Status status = nSerializedFMUStateSize(instancePtr, ((Fmu3StateImpl) state).ptr, size);
            return new FmuResult<>(status, size[0]);
        }
        return new FmuResult<>(Fmi3Status.Error, null);
    }

    @Override
    public FmuResult<byte[]> serializedState(Fmi3State state, long size) throws FmiInvalidNativeStateException {
        checkState();
        if (state instanceof Fmu3StateImpl) {

            byte[] data = new byte[(int) size];
            Fmi3Status status = nSerializeFMUState(instancePtr, ((Fmu3StateImpl) state).ptr, data, size);
            return new FmuResult<>(status, data);
        }
        return new FmuResult<>(Fmi3Status.Error, null);
    }

    @Override
    public FmuResult<Fmi3State> deSerializedState(byte[] serializedState) throws FmiInvalidNativeStateException {
        checkState();

        if (serializedState == null) {
            return new FmuResult<>(Fmi3Status.Error, null);
        }

        long[] ptr = new long[1];
        Fmi3Status status = nDeserializeFMUState(instancePtr, serializedState, serializedState.length, ptr);
        return new FmuResult<>(status, new Fmu3StateImpl(ptr[0]) {
            @Override
            public Fmi3Status free() throws FmiInvalidNativeStateException {
                return freeState(this);
            }
        });
    }

    @Override
    public FmuResult<double[]> getDirectionalDerivative(long[] unknowns, long[] knowns, double[] seed) throws FmiInvalidNativeStateException {
        checkState();

        if (unknowns == null) {
            unknowns = new long[0];
        }

        if (knowns == null) {
            knowns = new long[0];
        }
        if (seed == null) {
            seed = new double[0];
        }

        //FIXME not sure how to make this size
        double[] sensitivity = new double[knowns.length * unknowns.length];

        Fmi3Status status = nGetDirectionalDerivative(instancePtr, unknowns, unknowns.length, knowns, knowns.length, seed, seed.length, sensitivity,
                sensitivity.length);


        return new FmuResult<>(status, sensitivity);
    }

    @Override
    public FmuResult<double[]> getGetAdjointDerivative(long[] unknowns, long[] knowns, double[] seed,
            int nSensitivity) throws FmiInvalidNativeStateException {
        checkState();

        if (unknowns == null) {
            unknowns = new long[0];
        }

        if (knowns == null) {
            knowns = new long[0];
        }

        if (seed == null) {
            seed = new double[0];
        }

        double[] sensitivity = new double[nSensitivity];
        Fmi3Status status = nGetAdjointDerivative(instancePtr, unknowns, unknowns.length, knowns, knowns.length, seed, seed.length, sensitivity,
                sensitivity.length);
        return new FmuResult<>(status, sensitivity);
    }

    @Override
    public FmuResult<GetIntervalDecimalResponse> getIntervalDecimal(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();

        if (valueReferences == null) {
            valueReferences = new long[0];
        }

        GetIntervalDecimalResponse response = new GetIntervalDecimalResponse(valueReferences.length);

        Fmi3Status status = nGetIntervalDecimal(instancePtr, valueReferences, valueReferences.length, response.intervals, response.qualifiers);

        return new FmuResult<>(status, response);
    }

    @Override
    public FmuResult<IntervalFractionResponse> getIntervalFraction(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();
        if (valueReferences == null) {
            valueReferences = new long[0];
        }

        IntervalFractionResponse response = new IntervalFractionResponse(valueReferences.length);

        Fmi3Status status = nGetIntervalFraction(instancePtr, valueReferences, valueReferences.length, response.intervalCounters,
                response.resolutions, response.qualifiers);

        return new FmuResult<>(status, response);
    }

    @Override
    public FmuResult<double[]> getShiftDecimal(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();
        if (valueReferences == null) {
            valueReferences = new long[0];
        }

        double[] shifts = new double[valueReferences.length];
        Fmi3Status status = nGetShiftDecimal(instancePtr, valueReferences, valueReferences.length, shifts);

        return new FmuResult<>(status, shifts);
    }

    @Override
    public FmuResult<GetShiftFractionResponse> getShiftFraction(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();
        if (valueReferences == null) {
            valueReferences = new long[0];
        }

        GetShiftFractionResponse response = new GetShiftFractionResponse(valueReferences.length);

        Fmi3Status status = nGetShiftFraction(instancePtr, valueReferences, valueReferences.length, response.shiftCounters, response.resolutions);

        return new FmuResult<>(status, response);
    }

    @Override
    public Fmi3Status setIntervalDecimal(long[] valueReferences, double[] intervals) throws FmiInvalidNativeStateException {
        checkState();

        if (valueReferences == null) {
            valueReferences = new long[0];
        }

        if (intervals == null) {
            intervals = new double[0];
        }

        return nSetIntervalDecimal(instancePtr, valueReferences, valueReferences.length, intervals);
    }

    @Override
    public Fmi3Status setIntervalFraction(long[] valueReferences, long[] intervalCounters, long[] resolutions) throws FmiInvalidNativeStateException {
        checkState();
        if (valueReferences == null) {
            valueReferences = new long[0];
        }

        if (intervalCounters == null) {
            intervalCounters = new long[0];
        }
        if (resolutions == null) {
            resolutions = new long[0];
        }

        return nSetIntervalFraction(instancePtr, valueReferences, valueReferences.length, intervalCounters, resolutions);
    }

    @Override
    public Fmi3Status setShiftDecimal(long[] valueReferences, double[] shifts) {
        if (valueReferences == null) {
            valueReferences = new long[0];
        }

        if (shifts == null) {
            shifts = new double[0];
        }


        return nSetShiftDecimal(instancePtr, valueReferences, valueReferences.length, shifts);
    }

    @Override
    public Fmi3Status setShiftFraction(long[] valueReferences, long[] counters, long[] resolutions) {
        if (valueReferences == null) {
            valueReferences = new long[0];
        }

        if (counters == null) {
            counters = new long[0];
        }
        if (resolutions == null) {
            resolutions = new long[0];
        }

        return nSetShiftFraction(instancePtr, valueReferences, valueReferences.length, counters, resolutions);
    }

    @Override
    public Fmi3Status evaluateDiscreteStates() throws FmiInvalidNativeStateException {
        checkState();
        return nEvaluateDiscreteStates(instancePtr);
    }

    @Override
    public FmuResult<UpdateDiscreteStates> updateDiscreteStates() throws FmiInvalidNativeStateException {
        checkState();

        double[] nextEventTime = new double[1];
        boolean[] discreteStatesNeedUpdate = new boolean[1];
        boolean[] terminateSimulation = new boolean[1];
        boolean[] nominalsOfContinuousStatesChanged = new boolean[1];
        boolean[] valuesOfContinuousStatesChanged = new boolean[1];
        boolean[] nextEventTimeDefined = new boolean[1];


        return new FmuResult<>(nUpdateDiscreteStates(instancePtr, discreteStatesNeedUpdate, terminateSimulation, nominalsOfContinuousStatesChanged,
                valuesOfContinuousStatesChanged, nextEventTimeDefined, nextEventTime),
                new UpdateDiscreteStates(discreteStatesNeedUpdate[0], terminateSimulation[0], nominalsOfContinuousStatesChanged[0],
                        valuesOfContinuousStatesChanged[0], nextEventTimeDefined[0], nextEventTime[0]));
    }

    @Override
    public Fmi3Status enterContinuousTimeMode() throws FmiInvalidNativeStateException {
        checkState();
        return nEnterContinuousTimeMode(instancePtr);
    }

    @Override
    public FmuResult<CompletedIntegratorStepResponse> completedIntegratorStep(
            boolean noSetFMUStatePriorToCurrentPoint) throws FmiInvalidNativeStateException {
        checkState();
        boolean[] terminateSimulation = new boolean[1];
        boolean[] enterEventMode = new boolean[1];
        Fmi3Status status = nCompletedIntegratorStep(instancePtr, noSetFMUStatePriorToCurrentPoint, enterEventMode, terminateSimulation);

        return new FmuResult<>(status, new CompletedIntegratorStepResponse(enterEventMode[0], terminateSimulation[0]));
    }

    @Override
    public Fmi3Status setTime(double time) throws FmiInvalidNativeStateException {
        checkState();
        return nSetTime(instancePtr, time);
    }

    @Override
    public Fmi3Status setContinuousStates(double[] continuousStates) throws FmiInvalidNativeStateException {
        checkState();
        if (continuousStates == null) {
            continuousStates = new double[0];
        }

        return nSetContinuousStates(instancePtr, continuousStates, continuousStates.length);
    }

    @Override
    public FmuResult<double[]> getContinuousStateDerivatives(int nContinuousStates) throws FmiInvalidNativeStateException {
        checkState();

        double[] derivatives = new double[nContinuousStates];
        Fmi3Status status = nGetContinuousStateDerivatives(instancePtr, derivatives, derivatives.length);
        return new FmuResult<>(status, derivatives);
    }

    @Override
    public FmuResult<double[]> getGetEventIndicators(int nEventIndicators) throws FmiInvalidNativeStateException {
        checkState();
        double[] indicators = new double[nEventIndicators];
        Fmi3Status status = nGetEventIndicators(instancePtr, indicators, indicators.length);
        return new FmuResult<>(status, indicators);
    }

    @Override
    public FmuResult<double[]> getGetContinuousStates(int nContinuousStates) throws FmiInvalidNativeStateException {
        checkState();

        double[] continuousStates = new double[nContinuousStates];
        return new FmuResult<>(nGetContinuousStates(instancePtr, continuousStates, nContinuousStates), continuousStates);
    }

    @Override
    public FmuResult<double[]> getGetNominalsOfContinuousStates(int nContinuousStates) throws FmiInvalidNativeStateException {
        checkState();
        double[] nominals = new double[nContinuousStates];
        return new FmuResult<>(nGetNominalsOfContinuousStates(instancePtr, nominals, nContinuousStates), nominals);
    }

    @Override
    public FmuResult<Long> getNumberOfEventIndicators() throws FmiInvalidNativeStateException {
        checkState();

        long[] count = new long[1];
        Fmi3Status status = nGetNumberOfEventIndicators(instancePtr, count);
        return new FmuResult<>(status, count[0]);
    }

    @Override
    public FmuResult<Long> getNumberOfContinuousStates() throws FmiInvalidNativeStateException {
        checkState();

        long[] count = new long[1];
        Fmi3Status status = nGetNumberOfContinuousStates(instancePtr, count);
        return new FmuResult<>(status, count[0]);
    }

    @Override
    public Fmi3Status enterStepMode() throws FmiInvalidNativeStateException {
        checkState();
        return nEnterStepMode(instancePtr);
    }

    @Override
    public FmuResult<double[]> getOutputDerivatives(long[] valueReferences, int[] orders) throws FmiInvalidNativeStateException {
        checkState();
        double[] values = new double[valueReferences.length];
        Fmi3Status status = nGetOutputDerivatives(instancePtr, valueReferences, valueReferences.length, orders, values, valueReferences.length);
        return new FmuResult<>(status, values);
    }

    @Override
    public Fmi3Status activateModelPartition(long clockReference, double activationTime) throws FmiInvalidNativeStateException {
        checkState();
        return nActivateModelPartition(instancePtr, clockReference, activationTime);
    }

    @Override
    public FmuResult<float[]> getFloat32(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();
        float[] out = new float[valueReferences.length];
        return new FmuResult<>(nGetFloat32(instancePtr, valueReferences, valueReferences.length, out, out.length), out);
    }

    @Override
    public FmuResult<double[]> getFloat64(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();
        double[] out = new double[valueReferences.length];
        return new FmuResult<>(nGetFloat64(instancePtr, valueReferences, valueReferences.length, out, out.length), out);
    }

    @Override
    public FmuResult<byte[]> getInt8(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();
        byte[] out = new byte[valueReferences.length];
        return new FmuResult<>(nGetInt8(instancePtr, valueReferences, valueReferences.length, out, out.length), out);
    }

    @Override
    public FmuResult<byte[]> getUInt8(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();
        byte[] out = new byte[valueReferences.length];
        return new FmuResult<>(nGetUInt8(instancePtr, valueReferences, valueReferences.length, out, out.length), out);
    }

    @Override
    public FmuResult<short[]> getInt16(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();
        short[] out = new short[valueReferences.length];
        return new FmuResult<>(nGetInt16(instancePtr, valueReferences, valueReferences.length, out, out.length), out);
    }

    @Override
    public FmuResult<short[]> getUInt16(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();
        short[] out = new short[valueReferences.length];
        return new FmuResult<>(nGetUInt16(instancePtr, valueReferences, valueReferences.length, out, out.length), out);
    }

    @Override
    public FmuResult<int[]> getInt32(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();
        int[] out = new int[valueReferences.length];
        return new FmuResult<>(nGetInt32(instancePtr, valueReferences, valueReferences.length, out, out.length), out);
    }

    @Override
    public FmuResult<int[]> getUInt32(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();
        int[] out = new int[valueReferences.length];
        return new FmuResult<>(nGetUInt32(instancePtr, valueReferences, valueReferences.length, out, out.length), out);
    }

    @Override
    public FmuResult<long[]> getInt64(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();
        long[] out = new long[valueReferences.length];
        return new FmuResult<>(nGetInt64(instancePtr, valueReferences, valueReferences.length, out, out.length), out);
    }

    @Override
    public FmuResult<long[]> getUInt64(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();
        long[] out = new long[valueReferences.length];
        return new FmuResult<>(nGetUInt64(instancePtr, valueReferences, valueReferences.length, out, out.length), out);
    }

    @Override
    public FmuResult<boolean[]> getBoolean(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();
        boolean[] out = new boolean[valueReferences.length];
        return new FmuResult<>(nGetBoolean(instancePtr, valueReferences, valueReferences.length, out, out.length), out);
    }

    @Override
    public FmuResult<String[]> getString(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();
        String[] out = new String[valueReferences.length];
        return new FmuResult<>(nGetString(instancePtr, valueReferences, valueReferences.length, out, out.length), out);
    }

    @Override
    public FmuResult<byte[][]> getBinary(long[] valueReferences) throws FmiInvalidNativeStateException {
        checkState();
        byte[][] out = new byte[valueReferences.length][];
        return new FmuResult<>(nGetBinary(instancePtr, valueReferences, valueReferences.length, out, out.length), out);
    }

    @Override
    public Fmi3Status setFloat32(long[] valueReferences, int nValueReferences, float[] values, int nv) throws FmiInvalidNativeStateException {
        checkState();
        return nSetFloat32(instancePtr, valueReferences, nValueReferences, values, nv);
    }

    @Override
    public Fmi3Status setFloat64(long[] valueReferences, int nValueReferences, double[] values, int nv) throws FmiInvalidNativeStateException {
        checkState();
        return nSetFloat64(instancePtr, valueReferences, nValueReferences, values, nv);
    }

    @Override
    public Fmi3Status setInt8(long[] valueReferences, int nValueReferences, byte[] values, int nv) throws FmiInvalidNativeStateException {
        checkState();
        return nSetInt8(instancePtr, valueReferences, nValueReferences, values, nv);
    }

    @Override
    public Fmi3Status setUInt8(long[] valueReferences, int nValueReferences, byte[] values, int nv) throws FmiInvalidNativeStateException {
        checkState();
        return nSetUInt8(instancePtr, valueReferences, nValueReferences, values, nv);
    }

    @Override
    public Fmi3Status setInt16(long[] valueReferences, int nValueReferences, short[] values, int nv) throws FmiInvalidNativeStateException {
        checkState();
        return nSetInt16(instancePtr, valueReferences, nValueReferences, values, nv);
    }

    @Override
    public Fmi3Status setUInt16(long[] valueReferences, int nValueReferences, short[] values, int nv) throws FmiInvalidNativeStateException {
        checkState();
        return nSetUInt16(instancePtr, valueReferences, nValueReferences, values, nv);
    }

    @Override
    public Fmi3Status setInt32(long[] valueReferences, int nValueReferences, int[] values, int nv) throws FmiInvalidNativeStateException {
        checkState();
        return nSetInt32(instancePtr, valueReferences, nValueReferences, values, nv);
    }

    @Override
    public Fmi3Status setUInt32(long[] valueReferences, int nValueReferences, int[] values, int nv) throws FmiInvalidNativeStateException {
        checkState();
        return nSetUInt32(instancePtr, valueReferences, nValueReferences, values, nv);
    }

    @Override
    public Fmi3Status setInt64(long[] valueReferences, int nValueReferences, long[] values, int nv) throws FmiInvalidNativeStateException {
        checkState();
        return nSetInt64(instancePtr, valueReferences, nValueReferences, values, nv);
    }

    @Override
    public Fmi3Status setUInt64(long[] valueReferences, int nValueReferences, long[] values, int nv) throws FmiInvalidNativeStateException {
        checkState();
        return nSetUInt64(instancePtr, valueReferences, nValueReferences, values, nv);
    }

    @Override
    public Fmi3Status setBoolean(long[] valueReferences, int nValueReferences, boolean[] values, int nv) throws FmiInvalidNativeStateException {
        checkState();
        return nSetBoolean(instancePtr, valueReferences, nValueReferences, values, nv);
    }

    @Override
    public Fmi3Status setString(long[] valueReferences, int nValueReferences, String[] values, int nv) throws FmiInvalidNativeStateException {
        checkState();
        return nSetString(instancePtr, valueReferences, nValueReferences, values, nv);
    }

    @Override
    public Fmi3Status setBinary(long[] valueReferences,  byte[][] array_name) throws FmiInvalidNativeStateException {
        checkState();
        long[] sizes = new long[array_name.length];
        for (int i = 0; i < array_name.length; i++) {
            if (array_name[i] == null) {
                sizes[i] = 0;
            } else {
                sizes[i] = array_name[i].length;
            }
        }
        return nSetBinary(instancePtr, valueReferences, valueReferences.length, sizes, array_name, array_name.length);
    }


    @Override
    public boolean isValid() {
        return fmuPtr != 0 && instancePtr != 0;
    }

    private void checkState() throws FmiInvalidNativeStateException {
        if (!isValid()) {
            throw new FmiInvalidNativeStateException(
                    "The internal " + (fmuPtr != 0 ? "" : " FMU state is invalid. ") + (instancePtr != 0 ? "" : " Component state is invalid"));
        }
    }
}
