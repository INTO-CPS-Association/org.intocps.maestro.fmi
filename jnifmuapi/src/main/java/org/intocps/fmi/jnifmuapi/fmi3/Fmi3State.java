package org.intocps.fmi.jnifmuapi.fmi3;

import org.intocps.fmi.FmiInvalidNativeStateException;

public interface Fmi3State {
    Fmi3Status free() throws FmiInvalidNativeStateException;
}
