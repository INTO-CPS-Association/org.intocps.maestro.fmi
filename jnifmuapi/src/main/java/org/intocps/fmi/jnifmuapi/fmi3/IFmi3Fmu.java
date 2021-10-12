package org.intocps.fmi.jnifmuapi.fmi3;

import org.intocps.fmi.*;

import java.io.IOException;
import java.io.InputStream;
import java.util.zip.ZipException;

public interface IFmi3Fmu {
    void load() throws FmuInvocationException, FmuMissingLibraryException;

//    /**
//     * Creates an instance of the FMU
//     *
//     * @param name
//     * @param visible
//     * @param loggingOn
//     * @return
//     * @throws XPathExpressionException
//     * @throws FmiInvalidNativeStateException
//     */
//    public abstract IFmiComponent instantiate(String guid, String name,
//            boolean visible, boolean loggingOn, IFmuCallback callback)
//            throws XPathExpressionException, FmiInvalidNativeStateException;

    void unLoad() throws FmiInvalidNativeStateException;

    String getVersion() throws FmiInvalidNativeStateException;

    String getTypesPlatform() throws FmiInvalidNativeStateException;

    InputStream getModelDescription() throws ZipException, IOException;

    boolean isValid();

     IFmi3Instance instantiateModelExchange(
            String                 instanceName,
             String                 instantiationToken,
             String                 resourceLocation,
            boolean                visible,
            boolean                loggingOn,
//            fmi3InstanceEnvironment    instanceEnvironment,
            ICallbackLogMessage logMessage);

    IFmi3Instance instantiateCoSimulation(
            String                     instanceName,
            String                     instantiationToken,
            String                     resourceLocation,
            boolean                    visible,
            boolean                    loggingOn,
            boolean                    eventModeUsed,
            boolean                    earlyReturnAllowed,
            long[]                     requiredIntermediateVariables,
            int                        nRequiredIntermediateVariables,
            long                       instanceEnvironment,
            ICallbackLogMessage        logMessage,
            ICallbackIntermediateUpdate intermediateUpdate);

    IFmi3Instance instantiateScheduledExecution(
            String                     instanceName,
            String                     instantiationToken,
            String                     resourceLocation,
            boolean                    visible,
            boolean                    loggingOn,
            long[]                     requiredIntermediateVariables,
            int                        nRequiredIntermediateVariables,
         //   fmi3InstanceEnvironment        instanceEnvironment,
            ICallbackLogMessage logMessage,
            ICallbackIntermediateUpdate intermediateUpdate,
            ICallbackLockPreemption     lockPreemption,
            ICallbackUnlockPreemption   unlockPreemption);
}
