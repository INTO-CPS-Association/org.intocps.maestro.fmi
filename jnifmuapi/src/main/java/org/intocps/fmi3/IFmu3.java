package org.intocps.fmi3;

import org.intocps.fmi.*;

import javax.xml.xpath.XPathExpressionException;
import java.io.IOException;
import java.io.InputStream;
import java.util.zip.ZipException;

public interface IFmu3 {
    public abstract void load() throws FmuInvocationException, FmuMissingLibraryException;

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

    public abstract void unLoad() throws FmiInvalidNativeStateException;

    public abstract String getVersion() throws FmiInvalidNativeStateException;

    public abstract String getTypesPlatform() throws FmiInvalidNativeStateException;

    public abstract InputStream getModelDescription() throws ZipException, IOException;

    public abstract boolean isValid();

     IFmi3Instance instantiateModelExchange(
            String                 instanceName,
             String                 instantiationToken,
             String                 resourceLocation,
            boolean                visible,
            boolean                loggingOn,
//            fmi3InstanceEnvironment    instanceEnvironment,
            ICallbackLog     logMessage);

    IFmi3Instance instantiateCoSimulation(
            String                     instanceName,
            String                     instantiationToken,
            String                     resourceLocation,
            boolean                    visible,
            boolean                    loggingOn,
            boolean                    eventModeUsed,
            long       requiredIntermediateVariables[],
            int                         nRequiredIntermediateVariables,
           // fmi3InstanceEnvironment        instanceEnvironment,
            ICallbackLog         logMessage,
            ICallbackIntermediateUpdate intermediateUpdate);

    IFmi3Instance instantiateScheduledExecution(
            String                     instanceName,
            String                     instantiationToken,
            String                     resourceLocation,
            boolean                    visible,
            boolean                    loggingOn,
            long      requiredIntermediateVariables[],
            int                         nRequiredIntermediateVariables,
         //   fmi3InstanceEnvironment        instanceEnvironment,
            ICallbackLog         logMessage,
            ICallbackIntermediateUpdate intermediateUpdate,
            ICallbackLockPreemption     lockPreemption,
            ICallbackUnlockPreemption   unlockPreemption);
}
