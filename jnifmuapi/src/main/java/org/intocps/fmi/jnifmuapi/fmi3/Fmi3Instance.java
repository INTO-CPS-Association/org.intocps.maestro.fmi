package org.intocps.fmi.jnifmuapi.fmi3;

public class Fmi3Instance extends NativeFmu3Instance implements IFmi3Instance{

    private final long instancePtr;
    private final Fmi3Fmu fmu;
    private final long fmuPtr;

    public Fmi3Instance(long instancePtr, Fmi3Fmu fmi3Fmu) {
        this.instancePtr = instancePtr;
        this.fmu = fmi3Fmu;
        this.fmuPtr = fmi3Fmu.getFmuPtr();
    }

    @Override
    public Fmi3Status terminate() {
        return this.nTerminate(fmuPtr, instancePtr);
    }
}
