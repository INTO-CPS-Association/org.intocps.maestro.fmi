package org.intocps.fmi.jnifmuapi.fmi3;

import org.apache.commons.io.FilenameUtils;
import org.apache.commons.io.IOUtils;
import org.intocps.fmi.*;
import org.intocps.fmi.jnifmuapi.FmiUtil;
import org.intocps.fmi.jnifmuapi.NativeFmuLibraryLoader;

import java.io.*;
import java.util.zip.ZipException;

public class Fmi3Fmu extends NativeFmu3 implements IFmi3Fmu {
    private String name;
    private long fmuPtr;
    private boolean loaded;
    protected final static Object lock = new Object();
    protected static final String MODEL_DESCRIPTION = "modelDescription.xml";
    public File fmuDirectoryLocation;

    /**
     * If the FMU is a .fmu file, then it is unzipped to a temporary directory.
     * If the FMU is a directory, then it is not unzipped.
     * @param fmuPath is the path to the FMU
     */
    public Fmi3Fmu(File fmuPath) throws IOException {
        if(FilenameUtils.getExtension(fmuPath.getName()).equals("fmu")){
            File fmuDirectoryLocation = FmiUtil.createTempDir(FilenameUtils.getBaseName(fmuPath.getName()));
            this.extractToDirectory(fmuPath, fmuDirectoryLocation);
        }
        else if(fmuPath.isDirectory()){
            this.fmuDirectoryLocation = fmuPath;
        }
    }

    public Fmi3Fmu(File fmuPath, File extractDirectory) throws IOException, FmuInvocationException {
        this.extractToDirectory(fmuPath, extractDirectory);
        this.calculateName(fmuPath);
    }

    private void calculateName(File fmuPath) throws FmuInvocationException {
        this.name=FmiUtil.getFmuName(fmuPath);
    }

    private void extractToDirectory(File fmuPath, File extractDirectory) throws IOException {
        FmiUtil.unPack(fmuPath, extractDirectory);
        this.fmuDirectoryLocation = extractDirectory;
    }

    long getFmuPtr() {
        return fmuPtr;
    }

    /**
     *
     * @throws FmuInvocationException
     * @throws FmuMissingLibraryException
     */

    @Override
    public void load() throws FmuInvocationException, FmuMissingLibraryException {
        if (loaded) {
            return;
        }

        String modelIdentifier = null;
        final String errorMsg = "Fmu do not conform to the standard. Unable to obtain modelIndentifier '%s' for co-simulation, defaulting to archieve name '%s'";
        final String recovery_log_message = "Attempting to recover loading of the library by setting modelIdentifier to '%s'";
        try {
            modelIdentifier = FmiUtil.getModelIdentifier(getModelDescription());
        } catch (IOException e) {
            logger.error(String.format(errorMsg, "", name), e);
            logger.warn(recovery_log_message, name);
            modelIdentifier = name;
        }

        if (modelIdentifier == null) {
            logger.error(String.format(errorMsg, "", name));
            logger.warn(recovery_log_message, name);
            modelIdentifier = name;
        }

        File libraryPath = FmiUtil.generateLibraryFile(modelIdentifier, this.fmuDirectoryLocation, FmiUtil.FMIVersion.FMI3);

        if (!libraryPath.exists()) {
            logger.error(String.format("The library corresponding to the modelIdentifier '%s' could not be found at: '%s", modelIdentifier,
                    FmiUtil.logMessageLibraryPath(libraryPath)));
            logger.warn(String.format(recovery_log_message, name));
            modelIdentifier = name;
            libraryPath = FmiUtil.generateLibraryFile(modelIdentifier, this.fmuDirectoryLocation, FmiUtil.FMIVersion.FMI3);
        }
        internalLoad(libraryPath);
    }

    public void internalLoad(File libraryPath) throws FmuInvocationException, FmuMissingLibraryException {
        if (loaded) {
            return;
        }

        if (libraryPath == null || !libraryPath.exists()) {
            String errorMsg = FmiUtil.logMessageLibraryPath(libraryPath);
            throw new FmuMissingLibraryException("The library for the architecture and OS does not exist within the FMU at: " + errorMsg);
        }

        logger.debug("Loading FMU library: {}", libraryPath);
        NativeFmuLibraryLoader.loadNativeApi();
        // load dll
        synchronized (lock) {
            fmuPtr = nLoadLibrary(libraryPath.getAbsolutePath());
        }

        if (fmuPtr == 0) {
            throw new FmuInvocationException("Load failed");
        }

        logger.debug("Succesfully loaded FMU library: {}", libraryPath);
        loaded = true;
        logger.debug("Loaded library into DLL pointer: " + fmuPtr);
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
        logger.debug("Unloaded {} DLL pointer", fmuPtr);
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
        File path = new File(this.fmuDirectoryLocation, MODEL_DESCRIPTION);
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
    public IFmi3Instance instantiateModelExchange(String instanceName, String instantiationToken, String resourceLocation, boolean visible, boolean loggingOn, org.intocps.fmi.jnifmuapi.fmi3.ICallbackLogMessage logMessage) {
        return null;
    }

    @Override
    public IFmi3Instance instantiateCoSimulation(String instanceName, String instantiationToken, String resourceLocation, boolean visible, boolean loggingOn, boolean eventModeUsed, boolean earlyReturnAllowed, long[] requiredIntermediateVariables, int nRequiredIntermediateVariables, long instanceEnvironment, ICallbackLogMessage logMessage, ICallbackIntermediateUpdate intermediateUpdate) {
        if (!loaded) {
            return null;
        }

        long instancePtr = nInstantiateCoSimulation(this.fmuPtr, instanceName, instantiationToken, resourceLocation, visible, loggingOn, eventModeUsed, earlyReturnAllowed, requiredIntermediateVariables, instanceEnvironment, nRequiredIntermediateVariables, logMessage, intermediateUpdate);
        return new Fmi3Instance(instancePtr, this);
    }

    @Override
    public IFmi3Instance instantiateScheduledExecution(String instanceName, String instantiationToken, String resourceLocation, boolean visible, boolean loggingOn, long[] requiredIntermediateVariables, int nRequiredIntermediateVariables, ICallbackLogMessage logMessage, ICallbackIntermediateUpdate intermediateUpdate, org.intocps.fmi.jnifmuapi.fmi3.ICallbackLockPreemption lockPreemption, org.intocps.fmi.jnifmuapi.fmi3.ICallbackUnlockPreemption unlockPreemption) {
        return null;
    }

    private void checkState() throws FmiInvalidNativeStateException
    {
        if (!isValid()) {
            throw new FmiInvalidNativeStateException("The internal " + (fmuPtr != 0 ? "" : " FMU state is invalid. "));
        }
    }
}
