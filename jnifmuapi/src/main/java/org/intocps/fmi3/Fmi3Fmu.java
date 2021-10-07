package org.intocps.fmi3;

import org.apache.commons.io.IOUtils;
import org.intocps.fmi.*;
import org.intocps.fmi3.jnifmuapi.NativeFmu3;

import java.io.*;
import java.util.zip.ZipException;

public class Fmi3Fmu extends NativeFmu3 implements IFmi3Fmu {
    private long fmuPtr;
    private boolean loaded;
    protected final static Object lock = new Object();
    protected static final String MODEL_DESCRIPTION = "modelDescription.xml";
    public final File dir;

    public Fmi3Fmu(File dir) {
        this.dir = dir;
    }

    @Override
    public void load() throws FmuInvocationException, FmuMissingLibraryException {

    }

    @Override
    public void unLoad() throws FmiInvalidNativeStateException {
        if (!loaded) {
            return;
        }
        checkState();
        synchronized (lock) {
            nUnLoad(fmuPtr);
        }
        loaded = false;
    }

    @Override
    public String getVersion() throws FmiInvalidNativeStateException {
        return nGetVersion(this.fmuPtr);
    }

    @Override
    public String getTypesPlatform() throws FmiInvalidNativeStateException {
        return null;
    }

    @Override
    public InputStream getModelDescription() throws ZipException, IOException {
        File path = new File(dir, MODEL_DESCRIPTION);
        if (!path.exists() || !path.canRead()) {
            return null;
        }

        byte[] bytes = IOUtils.toByteArray(new FileInputStream(path));
        return new ByteArrayInputStream(bytes);

    }

    @Override
    public boolean isValid() {
        return fmuPtr != 0;
    }

    @Override
    public IFmi3Instance instantiateModelExchange(String instanceName, String instantiationToken, String resourceLocation, boolean visible, boolean loggingOn, org.intocps.fmi3.ICallbackLogMessage logMessage) {
        return null;
    }

    @Override
    public IFmi3Instance instantiateCoSimulation(String instanceName, String instantiationToken, String resourceLocation, boolean visible, boolean loggingOn, boolean eventModeUsed, boolean earlyReturnAllowed, long[] requiredIntermediateVariables, int nRequiredIntermediateVariables, long instanceEnvironment, ICallbackLogMessage logMessage, ICallbackIntermediateUpdate intermediateUpdate) {
        if (!loaded) {
            return null;
        }

        long instancePtr = nInstantiateCoSimulation(this.fmuPtr, instanceName, instantiationToken, resourceLocation, visible, loggingOn, eventModeUsed, earlyReturnAllowed, requiredIntermediateVariables, instanceEnvironment, nRequiredIntermediateVariables, logMessage, intermediateUpdate);
        return new Fmi3Instance(instancePtr);
    }

    @Override
    public IFmi3Instance instantiateScheduledExecution(String instanceName, String instantiationToken, String resourceLocation, boolean visible, boolean loggingOn, long[] requiredIntermediateVariables, int nRequiredIntermediateVariables, org.intocps.fmi3.ICallbackLogMessage logMessage, ICallbackIntermediateUpdate intermediateUpdate, ICallbackLockPreemption lockPreemption, ICallbackUnlockPreemption unlockPreemption) {
        return null;
    }

    private void checkState() throws FmiInvalidNativeStateException
    {
        if (!isValid()) {
            throw new FmiInvalidNativeStateException("The internal " + (fmuPtr != 0 ? "" : " FMU state is invalid. "));
        }
    }

}
