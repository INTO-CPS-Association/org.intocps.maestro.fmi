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
}
