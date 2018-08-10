package org.intocps.fmi;

public class FmuMissingLibraryException extends Exception {
    /**
     * serial
     */
    private static final long serialVersionUID = 1L;

    public FmuMissingLibraryException(String msg)
    {
        super(msg);
    }
}
