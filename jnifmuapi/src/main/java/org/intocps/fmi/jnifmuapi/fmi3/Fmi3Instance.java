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
        return this.nTerminate(fmuPtr, instancePtr);
    }

    @Override
    public Fmi3Status reset() throws FmiInvalidNativeStateException {
        checkState();
        return this.nReset(fmuPtr, instancePtr);
    }

    @Override
    public void freeInstance() throws FmiInvalidNativeStateException {
        checkState();
        this.nFreeInstance(fmuPtr, instancePtr);
    }

    @Override
    public Fmi3Status enterInitializationMode(Double tolerance, double startTime, Double stopTime) throws FmiInvalidNativeStateException {
        checkState();
        return this.nEnterInitializationMode(fmuPtr, instancePtr, tolerance != null, tolerance == null ? 0 : tolerance, startTime, stopTime != null,
                stopTime == null ? 0 : stopTime);
    }

    @Override
    public Fmi3Status exitInitializationMode() throws FmiInvalidNativeStateException {
        checkState();
        return this.nExitInitializationMode(fmuPtr, instancePtr);
    }

    @Override
    public Fmi3Status enterEventMode(boolean stepEvent, boolean stateEvent, int[] rootsFound,
            boolean timeEvent) throws FmiInvalidNativeStateException {
        checkState();
        if (rootsFound == null) {
            rootsFound = new int[0];
        }

        return this.nEnterEventMode(fmuPtr, instancePtr, stepEvent, stateEvent, rootsFound, rootsFound.length, timeEvent);
    }

    @Override
    public Fmi3Status enterConfigurationMode() throws FmiInvalidNativeStateException {
        checkState();
        return this.nEnterConfigurationMode(fmuPtr, instancePtr);
    }

    @Override
    public Fmi3Status exitConfigurationMode() throws FmiInvalidNativeStateException {
        checkState();
        return this.nExitConfigurationMode(fmuPtr, instancePtr);
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

        return this.nDoStep(fmuPtr, instancePtr, currentCommunicationPoint, communicationStepSize, noSetFMUStatePriorToCurrentPoint,
                eventHandlingNeeded, terminateSimulation, earlyReturn, lastSuccessfulTime);
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
    public Fmi3Status setFloat32(long[] valueReferences, float[] values) throws FmiInvalidNativeStateException {
        checkState();
        return nSetFloat32(instancePtr, valueReferences, valueReferences.length, values, values.length);
    }

    @Override
    public Fmi3Status setFloat64(long[] valueReferences, double[] values) throws FmiInvalidNativeStateException {
        checkState();
        return nSetFloat64(instancePtr, valueReferences, valueReferences.length, values, values.length);
    }

    @Override
    public Fmi3Status setInt8(long[] valueReferences, byte[] values) throws FmiInvalidNativeStateException {
        checkState();
        return nSetInt8(instancePtr, valueReferences, valueReferences.length, values, values.length);
    }

    @Override
    public Fmi3Status setUInt8(long[] valueReferences, byte[] values) throws FmiInvalidNativeStateException {
        checkState();
        return nSetUInt8(instancePtr, valueReferences, valueReferences.length, values, values.length);
    }

    @Override
    public Fmi3Status setInt16(long[] valueReferences, short[] values) throws FmiInvalidNativeStateException {
        checkState();
        return nSetInt16(instancePtr, valueReferences, valueReferences.length, values, values.length);
    }

    @Override
    public Fmi3Status setUInt16(long[] valueReferences, short[] values) throws FmiInvalidNativeStateException {
        checkState();
        return nSetUInt16(instancePtr, valueReferences, valueReferences.length, values, values.length);
    }

    @Override
    public Fmi3Status setInt32(long[] valueReferences, int[] values) throws FmiInvalidNativeStateException {
        checkState();
        return nSetInt32(instancePtr, valueReferences, valueReferences.length, values, values.length);
    }

    @Override
    public Fmi3Status setUInt32(long[] valueReferences, int[] values) throws FmiInvalidNativeStateException {
        checkState();
        return nSetUInt32(instancePtr, valueReferences, valueReferences.length, values, values.length);
    }

    @Override
    public Fmi3Status setInt64(long[] valueReferences, long[] values) throws FmiInvalidNativeStateException {
        checkState();
        return nSetInt64(instancePtr, valueReferences, valueReferences.length, values, values.length);
    }

    @Override
    public Fmi3Status setUInt64(long[] valueReferences, long[] values) throws FmiInvalidNativeStateException {
        checkState();
        return nSetUInt64(instancePtr, valueReferences, valueReferences.length, values, values.length);
    }

    @Override
    public Fmi3Status setBoolean(long[] valueReferences, boolean[] values) throws FmiInvalidNativeStateException {
        checkState();
        return nSetBoolean(instancePtr, valueReferences, valueReferences.length, values, values.length);
    }

    @Override
    public Fmi3Status setString(long[] valueReferences, String[] values) throws FmiInvalidNativeStateException {
        checkState();
        return nSetString(instancePtr, valueReferences, valueReferences.length, values, values.length);
    }

    @Override
    public Fmi3Status setBinary(long[] valueReferences, byte[][] array_name) throws FmiInvalidNativeStateException {
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
