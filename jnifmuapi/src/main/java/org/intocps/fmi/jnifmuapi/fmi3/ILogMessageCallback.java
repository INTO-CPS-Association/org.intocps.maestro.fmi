package org.intocps.fmi.jnifmuapi.fmi3;

public interface ILogMessageCallback {
    void logMessage(String instanceName, Fmi3Status status, String category, String message);

    ILogMessageCallback CONSOLE_LOGGER = (instanceName, status, category, message) -> {
        System.out.printf("Received log message:\n" + "instanceName: '%s'\n" + "Status: '%s'\n" + "Category: '%s'\n" + "Message: '%s'%n",
                instanceName, status.name(), category, message);
    };
}
