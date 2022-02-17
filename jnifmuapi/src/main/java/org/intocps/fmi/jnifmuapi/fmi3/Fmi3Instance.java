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
