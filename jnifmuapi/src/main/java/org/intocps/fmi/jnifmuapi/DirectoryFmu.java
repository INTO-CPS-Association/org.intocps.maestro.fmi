/*
 * This file is part of the INTO-CPS toolchain.
 *
 * Copyright (c) 2017-CurrentYear, INTO-CPS Association,
 * c/o Professor Peter Gorm Larsen, Department of Engineering
 * Finlandsgade 22, 8200 Aarhus N.
 *
 * All rights reserved.
 *
 * THIS PROGRAM IS PROVIDED UNDER THE TERMS OF GPL VERSION 3 LICENSE OR
 * THIS INTO-CPS ASSOCIATION PUBLIC LICENSE VERSION 1.0.
 * ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS PROGRAM CONSTITUTES
 * RECIPIENT'S ACCEPTANCE OF THE OSMC PUBLIC LICENSE OR THE GPL
 * VERSION 3, ACCORDING TO RECIPIENTS CHOICE.
 *
 * The INTO-CPS toolchain  and the INTO-CPS Association Public License
 * are obtained from the INTO-CPS Association, either from the above address,
 * from the URLs: http://www.into-cps.org, and in the INTO-CPS toolchain distribution.
 * GNU version 3 is obtained from: http://www.gnu.org/copyleft/gpl.html.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without
 * even the implied warranty of  MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE, EXCEPT AS EXPRESSLY SET FORTH IN THE
 * BY RECIPIENT SELECTED SUBSIDIARY LICENSE CONDITIONS OF
 * THE INTO-CPS ASSOCIATION.
 *
 * See the full INTO-CPS Association Public License conditions for more details.
 */

/*
 * Author:
 *		Kenneth Lausdahl
 */

package org.intocps.fmi.jnifmuapi;

import org.apache.commons.io.IOUtils;
import org.intocps.fmi.*;

import javax.xml.xpath.XPathExpressionException;
import java.io.*;
import java.net.URI;
import java.net.URISyntaxException;
import java.nio.file.Path;
import java.util.zip.ZipException;

class DirectoryFmu extends NativeFmu implements IFmu {
    protected final static Object lock = new Object();

    protected static final String MODEL_DESCRIPTION = "modelDescription.xml";

    public final String name;
    public final File dir;
    boolean loaded;
    private long fmuPtr;

    public DirectoryFmu(File path, String name) throws FmuInvocationException {
        this.dir = path;
        this.name = name;
    }

    long getFmuPtr() {
        return fmuPtr;
    }

    /*
     * (non-Javadoc)
     * @see intocps.fmuapi.IFmu#load()
     * The reason for the recovering behaviour is based on several FMUs having a wrong modelIdentifier attribute in the modelDescription.xml.
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

        return FmiUtil.generateLibraryFileFromPlatform(osName, arch, modelIdentifier, dir);
    }

    public static String logMessageLibraryPath(File libraryPath) {
        Path p = libraryPath.toPath();
        int pLength = p.getNameCount();
        String desiredPath = p.subpath(pLength - 3, pLength).toString();
        return desiredPath;
    }

    public void internalLoad(File libraryPath) throws FmuInvocationException, FmuMissingLibraryException {
        if (loaded) {
            return;
        }

        if (libraryPath == null || !libraryPath.exists()) {
            String errorMsg = logMessageLibraryPath(libraryPath);
            throw new FmuMissingLibraryException("The library for the architecture and OS does not exist within the FMU at: " + errorMsg);
        }

        logger.debug("Loading FMU library: {}", libraryPath);
        NativeFmu.loadNativeApi();
        // load dll
        synchronized (lock) {
            fmuPtr = nLoadLibrary(libraryPath.getAbsolutePath());
        }

        if (fmuPtr == 0) {
            throw new FmuInvocationException("Load failed");
        }

        loaded = true;
        logger.debug("Dll pointer: " + fmuPtr);
    }

    /*
     * (non-Javadoc)
     * @see intocps.fmuapi.IFmu#instantiate(java.lang.String, java.lang.String, boolean, boolean,
     * intocps.fmuapi.Fmu.IFmuCallback)
     */
    @Override
    public IFmiComponent instantiate(String guid, String name, boolean visible, boolean loggingOn,
            final IFmuCallback callback) throws XPathExpressionException, FmiInvalidNativeStateException {
        if (!loaded) {
            return null;
        }

        File resourceDir = new File(dir, "resources");
        resourceDir.mkdirs();

        URI resourceUri = resourceDir.toURI();

        if (System.getProperty("fmi.instantiate.with.empty.authority") != null && resourceUri.getAuthority() == null) {
            /*
             * This is a compatibility feature since e.g. Modelon FMI Toolbox for MATHLAB/Simulink is not compliant with
             * FMI 2
             */
            try {
                logger.debug("Inserting empty authority into URI: {}", resourceUri);
                resourceUri = new URI(resourceUri.getScheme(), "", resourceUri.getPath(), null);
                logger.trace("URI changed to '{}'", resourceUri);
            } catch (URISyntaxException e) {
                logger.error("Failed to create resourceUri with empty authority");
            }
        }

        String resourceLocation = resourceUri.toString();

        logger.trace("resourceLocationURI is: '{}'", resourceUri);

        NativeFmu.ICallback cb = null;

        if (callback != null) {
            cb = new ICallback() {
                @Override
                public void log(String name, byte status, String category, String message) {
                    callback.log(name, Fmi2Status.valueOf(status), category, message);
                }

                @Override
                public void stepFinished(byte fmuStatus) {
                    callback.stepFinished(Fmi2Status.valueOf(fmuStatus));
                }
            };
        }
        checkState();

        long c = 0;
        synchronized (lock) {
            c = ninstantiate(fmuPtr, name, guid, resourceLocation, visible, loggingOn, cb);
        }
        if (c == 0) {
            return null;// init failed
        }

        return new FmuComponent(this, c, guid, name);
    }

    /*
     * (non-Javadoc)
     * @see intocps.fmuapi.IFmu#UnLoad()
     */
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

    /*
     * (non-Javadoc)
     * @see intocps.fmuapi.IFmu#getVersion()
     */
    @Override
    public String getVersion() throws FmiInvalidNativeStateException {
        checkState();
        return nGetVersion(fmuPtr);
    }

    /*
     * (non-Javadoc)
     * @see intocps.fmuapi.IFmu#getTypesPlatform()
     */
    @Override
    public String getTypesPlatform() throws FmiInvalidNativeStateException {
        checkState();
        return nGetTypesPlatform(fmuPtr);
    }

    @Override
    public InputStream getModelDescription() throws ZipException, IOException {
        File path = new File(dir, MODEL_DESCRIPTION);
        if (!path.exists() || !path.canRead()) {
            return null;
        }

        try {
            byte[] bytes = IOUtils.toByteArray(new FileInputStream(path));
            return new ByteArrayInputStream(bytes);

        } finally {
        }
    }

    @Override
    public boolean isValid() {
        return fmuPtr != 0;
    }

    protected void checkState() throws FmiInvalidNativeStateException {
        if (!isValid()) {
            throw new FmiInvalidNativeStateException("The internal " + (fmuPtr != 0 ? "" : " FMU state is invalid. "));
        }
    }

    void synchronizedFree(FmuComponent comp) throws FmiInvalidNativeStateException {
        synchronized (lock) {
            comp.internalFreeInstance();
        }
    }

    public static String getJniApiVersion() {
        NativeFmu.loadNativeApi();
        
        return NativeFmu.nGetJniApiVersion();
    }
}
