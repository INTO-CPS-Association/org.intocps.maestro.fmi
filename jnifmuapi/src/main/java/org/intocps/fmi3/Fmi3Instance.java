package org.intocps.fmi3;

import org.intocps.fmi3.jnifmuapi.NativeFmu3Instance;

public class Fmi3Instance extends NativeFmu3Instance implements IFmi3Instance{
    public Fmi3Instance(long fmuPtr) {
        super(fmuPtr);
    }
}
