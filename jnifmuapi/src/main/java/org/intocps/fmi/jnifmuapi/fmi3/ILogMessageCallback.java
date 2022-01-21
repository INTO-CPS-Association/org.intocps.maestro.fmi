package org.intocps.fmi.jnifmuapi.fmi3;

public interface ILogMessageCallback {
    void logMessage(String instanceName, Fmi3Status status, String category, String message);
}
