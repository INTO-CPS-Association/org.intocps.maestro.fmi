package org.intocps.fmi3;

public interface ICallbackIntermediateUpdate {
    void intermediateUpdate(long instanceEnvironment, double intermediateUpdateTime, boolean clocksTicked,
                            boolean intermediateVariableSetRequested, boolean intermediateVariableGetAllowed, boolean intermediateStepFinished,
                            boolean canReturnEarly, boolean[] earlyReturnRequested, double[] earlyReturnTime);
}
