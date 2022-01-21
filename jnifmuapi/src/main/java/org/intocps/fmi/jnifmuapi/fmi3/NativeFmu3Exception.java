package org.intocps.fmi.jnifmuapi.fmi3;

public class NativeFmu3Exception extends Exception {
    public NativeFmu3Exception() {
    }

    public NativeFmu3Exception(String message) {
        super(message);
    }

    public NativeFmu3Exception(String message, Throwable cause) {
        super(message, cause);
    }

    public NativeFmu3Exception(Throwable cause) {
        super(cause);
    }

    public NativeFmu3Exception(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace) {
        super(message, cause, enableSuppression, writableStackTrace);
    }
}
