package org.intocps.fmi.jnifmuapi.fmi3;

public interface IIntermediateUpdateCallback {

    class IntermediateUpdateResponse {
        boolean earlyReturnRequested;

        public double getEarlyReturnTime() {
            return earlyReturnTime;
        }

        public boolean isEarlyReturnRequested() {
            return earlyReturnRequested;
        }

        public IntermediateUpdateResponse(boolean earlyReturnRequested, double earlyReturnTime) {
            this.earlyReturnRequested = earlyReturnRequested;
            this.earlyReturnTime = earlyReturnTime;
        }

        double earlyReturnTime;
    }

    //(JDZZZZZ)Lorg/intocps/fmi/jnifmuapi/fmi3/IIntermediateUpdateCallback$IntermediateUpdateResponse
    IntermediateUpdateResponse intermediateUpdate(long instanceEnvironment, double intermediateUpdateTime, boolean clocksTicked,
            boolean intermediateVariableSetRequested, boolean intermediateVariableGetAllowed, boolean intermediateStepFinished,
            boolean canReturnEarly);
}
