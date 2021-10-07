package org.intocps.fmi3;

public interface ICallbackLogMessage {
    void logMessage(long instanceEnvironment, String instanceName, Fmi3Status status, String category, String message);
}
