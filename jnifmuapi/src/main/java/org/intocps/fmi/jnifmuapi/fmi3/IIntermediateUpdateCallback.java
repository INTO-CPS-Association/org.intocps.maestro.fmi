package org.intocps.fmi.jnifmuapi.fmi3;

public interface IIntermediateUpdateCallback {
    void intermediateUpdate(long instanceEnvironment, double intermediateUpdateTime, boolean clocksTicked, boolean intermediateVariableSetRequested,
            boolean intermediateVariableGetAllowed, boolean intermediateStepFinished, boolean canReturnEarly, boolean[] earlyReturnRequested,
            double[] earlyReturnTime);
}
