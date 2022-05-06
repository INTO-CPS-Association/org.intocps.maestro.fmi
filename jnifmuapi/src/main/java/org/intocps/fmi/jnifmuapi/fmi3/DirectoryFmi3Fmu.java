package org.intocps.fmi.jnifmuapi.fmi3;

import org.apache.commons.io.IOUtils;
import org.intocps.fmi.FmiInvalidNativeStateException;
import org.intocps.fmi.FmuInvocationException;
import org.intocps.fmi.FmuMissingLibraryException;
import org.intocps.fmi.jnifmuapi.FmiUtil;
import org.intocps.fmi.jnifmuapi.NativeFmuLibraryLoader;
import org.intocps.fmi.jnifmuapi.shared.NativeLoadManager;

import java.io.*;

public class DirectoryFmi3Fmu extends NativeFmu3 implements IFmi3Fmu {

    protected static final String MODEL_DESCRIPTION = "modelDescription.xml";
    public final String name;
    public final File dir;
    protected final NativeLoadManager lifeCycle;

    public long getFmuPtr() {
        return lifeCycle.getFmuPtr();
    }

    static class LiveCycleProtectedFunctions implements NativeLoadManager.NativeLifecycleFunction {
        public LiveCycleProtectedFunctions(NativeFmu3 nativeFmu) {
            this.nativeFmu = nativeFmu;
        }

        final NativeFmu3 nativeFmu;

        @Override
        public long lifeCycleLoadLibrary(String libraryPath) {
            return nativeFmu.nLoadLibrary(libraryPath);
        }

        @Override
        public void lifeCycleUnLoad(long ptr) {
            nativeFmu.nUnLoad(ptr);
        }

    }


    public DirectoryFmi3Fmu(File path, String name) {
        this.dir = path;
        this.name = name;
        lifeCycle = new NativeLoadManager(this.dir, this.name, new LiveCycleProtectedFunctions(this));
    }


    @Override
    public void load() throws FmuInvocationException, FmuMissingLibraryException {
        if (lifeCycle.isLoaded()) {
            return;
        }

        if (!lifeCycle.isDirValid()) {
            throw new FmuInvocationException("FMU Directory invalid: " + this.dir);
        }

        String modelIdentifier = null;
        final String errorMsg = "Fmu do not conform to the standard. Unable to obtain modelIdentifier '%s' for co-simulation, defaulting to archieve name '%s'";
        final String recovery_log_message = "Attempting to recover loading of the library by setting modelIdentifier to {}";
        try {
            modelIdentifier = FmiUtil.getModelIdentifier(getModelDescription());
        } catch (IOException e) {
            logger.error(String.format(errorMsg, "", name), e);
            logger.warn(recovery_log_message, name);
        }


        this.lifeCycle.load(modelIdentifier, FmiUtil.FMIVersion.FMI3);
    }

    @Override
    public void unLoad() throws FmiInvalidNativeStateException {
        this.lifeCycle.unLoad();
    }

    @Override
    public String getVersion() throws FmiInvalidNativeStateException {
        NativeFmuLibraryLoader.loadNativeApi();
        return nGetVersion(this.getFmuPtr());
    }


    @Override
    public InputStream getModelDescription() throws IOException {
        File path = new File(dir, MODEL_DESCRIPTION);
        if (!path.exists() || !path.canRead()) {
            return null;
        }

        byte[] bytes = IOUtils.toByteArray(new FileInputStream(path));
        return new ByteArrayInputStream(bytes);

    }

    @Override
    public boolean isValid() {
        return lifeCycle.isValid();
    }

    @Override
    public IFmi3Instance instantiateModelExchange(String instanceName, String instantiationToken, String resourceLocation, boolean visible,
            boolean loggingOn, ILogMessageCallback logMessage) {

        if (!lifeCycle.isLoaded()) {
            return null;
        }
        long instancePtr = nInstantiateModelExchange(lifeCycle.getFmuPtr(), instanceName, instantiationToken, resourceLocation, visible, loggingOn,
                logMessage);
        if (instancePtr == 0) {
            return null;
        }
        return new Fmi3Instance(instancePtr, this);
    }


    @Override
    public IFmi3Instance instantiateCoSimulation(String instanceName, String instantiationToken, String resourceLocation, boolean visible,
            boolean loggingOn, boolean eventModeUsed, boolean earlyReturnAllowed, long[] requiredIntermediateVariables,
            ILogMessageCallback logMessage, IIntermediateUpdateCallback intermediateUpdate) {
        if (!lifeCycle.isLoaded()) {
            return null;
        }

        if (requiredIntermediateVariables == null) {
            requiredIntermediateVariables = new long[0];
        }

        long instancePtr = nInstantiateCoSimulation(lifeCycle.getFmuPtr(), instanceName, instantiationToken, resourceLocation, visible, loggingOn,
                eventModeUsed, earlyReturnAllowed, requiredIntermediateVariables, requiredIntermediateVariables.length, logMessage,
                intermediateUpdate);
        if (instancePtr == 0) {
            return null;
        }
        return new Fmi3Instance(instancePtr, this);
    }

    @Override
    public IFmi3Instance instantiateScheduledExecution(String instanceName, String instantiationToken, String resourceLocation, boolean visible,
            boolean loggingOn, ILogMessageCallback logMessage, IClockUpdateCallback clockUpdate, ILockPreemptionCallback lockPreemption,
            IUnlockPreemptionCallback unlockPreemption) {

        if (!lifeCycle.isLoaded()) {
            return null;
        }

        long instancePtr = nInstantiateScheduledExecution(lifeCycle.getFmuPtr(), instanceName, instantiationToken, resourceLocation, visible,
                loggingOn, logMessage, clockUpdate, lockPreemption, unlockPreemption);
        if (instancePtr == 0) {
            return null;
        }
        return new Fmi3Instance(instancePtr, this);

    }

    /**
     * Return the FMI version that the JNI API is implementing
     *
     * @return the version of FMI
     */
    public static String getJniApiFmiVersion() {
        NativeFmuLibraryLoader.loadNativeApi();

        return NativeFmu3.nGetJniApiFmiVersion();
    }
}
