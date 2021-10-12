package org.intocps.fmi.jnifmuapi.fmi3;

public interface ICallbackLogMessage {
    void logMessage(String instanceName, Fmi3Status status, String category, String message);
}
