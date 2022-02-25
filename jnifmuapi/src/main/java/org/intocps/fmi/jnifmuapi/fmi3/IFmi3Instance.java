package org.intocps.fmi.jnifmuapi.fmi3;

import org.intocps.fmi.FmiInvalidNativeStateException;

public interface IFmi3Instance {
    boolean isValid();

    Fmi3Status terminate() throws FmiInvalidNativeStateException;


    Fmi3Status reset() throws FmiInvalidNativeStateException;

    void freeInstance() throws FmiInvalidNativeStateException;

    Fmi3Status enterInitializationMode(Double tolerance, double startTime, Double stopTime) throws FmiInvalidNativeStateException;

    Fmi3Status exitInitializationMode() throws FmiInvalidNativeStateException;

    Fmi3Status enterEventMode(boolean stepEvent, boolean stateEvent, int[] rootsFound, boolean timeEvent) throws FmiInvalidNativeStateException;

    Fmi3Status enterConfigurationMode() throws FmiInvalidNativeStateException;

    Fmi3Status exitConfigurationMode() throws FmiInvalidNativeStateException;

    FmuResult<DoStepResult> doStep(double currentCommunicationPoint, double communicationStepSize,
            boolean noSetFMUStatePriorToCurrentPoint) throws FmiInvalidNativeStateException;

    Fmi3Status doStep(double currentCommunicationPoint, double communicationStepSize, boolean noSetFMUStatePriorToCurrentPoint,
            boolean[] eventEncountered, boolean[] terminateSimulation, boolean[] earlyReturn,
            double[] lastSuccessfulTime) throws FmiInvalidNativeStateException;

    class DoStepResult {
        final boolean eventHandlingNeeded;
        final boolean terminateSimulation;
        final boolean earlyReturn;

        public DoStepResult(boolean eventHandlingNeeded, boolean terminateSimulation, boolean earlyReturn, double lastSuccessfulTime) {
            this.eventHandlingNeeded = eventHandlingNeeded;
            this.terminateSimulation = terminateSimulation;
            this.earlyReturn = earlyReturn;
            this.lastSuccessfulTime = lastSuccessfulTime;
        }

        final double lastSuccessfulTime;
    }


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
