package org.intocps.fmi.jnifmuapi.fmi3;

public class FmuResult<T> {
    final public T result;
    final public Fmi3Status status;

    public FmuResult(Fmi3Status status, T result) {
        this.result = result;
        this.status = status;
    }
}