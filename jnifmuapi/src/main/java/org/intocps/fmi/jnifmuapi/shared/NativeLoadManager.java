package org.intocps.fmi.jnifmuapi.shared;

import org.intocps.fmi.FmiInvalidNativeStateException;
import org.intocps.fmi.FmuInvocationException;
import org.intocps.fmi.FmuMissingLibraryException;
import org.intocps.fmi.jnifmuapi.FmiUtil;
import org.intocps.fmi.jnifmuapi.NativeFmuLibraryLoader;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.File;
import java.nio.file.Path;

public class NativeLoadManager {

    public interface NativeLifecycleFunction {
        long lifeCycleLoadLibrary(String libraryPath);

        void lifeCycleUnLoad(long ptr);
    }

    final static Logger logger = LoggerFactory.getLogger(NativeLoadManager.class);
    public final static Object lock = new Object();
    private final String name;
    private final NativeLifecycleFunction lifecycleFunc;

    public boolean isLoaded() {
        return loaded;
    }

    boolean loaded;
    private long fmuPtr;
    public final File dir;


    public NativeLoadManager(File dir, String name, NativeLifecycleFunction lifecycleFunc) {
        this.dir = dir;
        if (!isDirValid()) {
            logger.error("Native load manager instantiated with a nonexistent directory: {}. Subsequent calls will fail.", this.dir);
        }
        this.name = name;
        this.lifecycleFunc = lifecycleFunc;
    }

    public boolean isDirValid() {
        return this.dir != null && this.dir.exists();
    }


    public long getFmuPtr() {
        return fmuPtr;
    }

    /*
     * (non-Javadoc)
     * @see intocps.fmuapi.IFmu#load()
     * The reason for the recovering behaviour is based on several FMUs having a wrong modelIdentifier attribute in the modelDescription.xml.
     */
    public void load(String modelIdentifier) throws FmuInvocationException, FmuMissingLibraryException {
        if (loaded) {
            return;
        }


        final String errorMsg = "Fmu do not conform to the standard. Unable to obtain modelIndentifier '%s' for co-simulation, defaulting to archieve name '%s'";
        final String recovery_log_message = "Attempting to recover loading of the library by setting modelIdentifier to '{}'";
        if (modelIdentifier == null) {
            modelIdentifier = name;
        }

        if (modelIdentifier == null) {
            logger.error(String.format(errorMsg, "", name));
            logger.warn(recovery_log_message, name);
        }

        File libraryPath = generateLibraryFile(modelIdentifier);

        if (!libraryPath.exists()) {
            logger.error(String.format("The library corresponding to the modelIdentifier '%s' could not be found at: '%s", modelIdentifier,
                    logMessageLibraryPath(libraryPath)));
            logger.warn(String.format(recovery_log_message, name));
            modelIdentifier = name;
            libraryPath = generateLibraryFile(modelIdentifier);
        }
        internalLoad(libraryPath);
    }


    public File generateLibraryFile(String modelIdentifier) {
        String osName = System.getProperty("os.name");
        String arch = System.getProperty("os.arch");

        return FmiUtil.generateLibraryFileFromPlatform(osName, arch, modelIdentifier, dir, FmiUtil.FMIVersion.FMI2);
    }

    public static String logMessageLibraryPath(File libraryPath) {
        Path p = libraryPath.toPath();
        int pLength = p.getNameCount();
        return p.subpath(pLength - 3, pLength).toString();
    }

    public void internalLoad(File libraryPath) throws FmuInvocationException, FmuMissingLibraryException {
        if (loaded) {
            return;
        }

        if (libraryPath == null || !libraryPath.exists()) {
            String errorMsg = libraryPath == null ? "" : logMessageLibraryPath(libraryPath);
            throw new FmuMissingLibraryException("The library for the architecture and OS does not exist within the FMU at: " + errorMsg);
        }

        logger.debug("Loading FMU library: {}", libraryPath);
        NativeFmuLibraryLoader.loadNativeApi();
        // load dll
        synchronized (lock) {
            fmuPtr = lifecycleFunc.lifeCycleLoadLibrary(libraryPath.getAbsolutePath());
        }

        if (fmuPtr == 0) {
            throw new FmuInvocationException("Load failed");
        }

        logger.debug("Successfully loaded FMU library: {}", libraryPath);
        loaded = true;
        logger.debug("Loaded library into DLL pointer: " + fmuPtr);
    }


    /*
     * (non-Javadoc)
     * @see intocps.fmuapi.IFmu#UnLoad()
     */
    public void unLoad() throws FmiInvalidNativeStateException {
        if (!loaded) {
            return;
        }
        checkState();
        synchronized (lock) {
            lifecycleFunc.lifeCycleUnLoad(fmuPtr);
        }
        loaded = false;
    }

    public boolean isValid() {
        return fmuPtr != 0;
    }

    public void checkState() throws FmiInvalidNativeStateException {
        if (!isValid()) {
            throw new FmiInvalidNativeStateException("The internal " + (fmuPtr != 0 ? "" : " FMU state is invalid. "));
        }
    }

}
