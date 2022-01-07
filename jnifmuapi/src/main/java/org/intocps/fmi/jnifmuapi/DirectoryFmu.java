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
import org.intocps.fmi.jnifmuapi.shared.NativeLoadManager;

import javax.xml.xpath.XPathExpressionException;
import java.io.*;
import java.net.URI;
import java.net.URISyntaxException;

class DirectoryFmu extends NativeFmu implements IFmu {

    protected static final String MODEL_DESCRIPTION = "modelDescription.xml";

    static class LiveCycleProtectedFunctions implements NativeLoadManager.NativeLifecycleFunction {
        public LiveCycleProtectedFunctions(NativeFmu nativeFmu) {
            this.nativeFmu = nativeFmu;
        }

        final NativeFmu nativeFmu;

        @Override
        public long lifeCycleLoadLibrary(String libraryPath) {
            return nativeFmu.nLoadLibrary(libraryPath);
        }

        @Override
        public void lifeCycleUnLoad(long ptr) {
            nativeFmu.nUnLoad(ptr);
        }

    }

    public long getFmuPtr() {
        return lifeCycle.getFmuPtr();
    }

    public final String name;
    public final File dir;
    protected final NativeLoadManager lifeCycle;

    public DirectoryFmu(File path, String name) {
        this.dir = path;
        this.name = name;
        lifeCycle = new NativeLoadManager(this.dir, this.name, new LiveCycleProtectedFunctions(this));
    }


    @Override
    public void load() throws FmuInvocationException, FmuMissingLibraryException {
        if (lifeCycle.isLoaded()) {
            return;
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


        this.lifeCycle.load(modelIdentifier);
    }

    /*
     * (non-Javadoc)
     * @see intocps.fmuapi.IFmu#instantiate(java.lang.String, java.lang.String, boolean, boolean,
     * intocps.fmuapi.Fmu.IFmuCallback)
     */
    @Override
    public IFmiComponent instantiate(String guid, String name, boolean visible, boolean loggingOn,
            final IFmuCallback callback) throws XPathExpressionException, FmiInvalidNativeStateException {
        if (!lifeCycle.isLoaded()) {
            return null;
        }

        File resourceDir = new File(dir, "resources");
        //noinspection ResultOfMethodCallIgnored
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
        lifeCycle.checkState();

        long c;
        synchronized (NativeLoadManager.lock) {
            c = ninstantiate(lifeCycle.getFmuPtr(), name, guid, resourceLocation, visible, loggingOn, cb);
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
        this.lifeCycle.unLoad();
    }

    /*
     * (non-Javadoc)
     * @see intocps.fmuapi.IFmu#getVersion()
     */
    @Override
    public String getVersion() throws FmiInvalidNativeStateException {
        this.lifeCycle.checkState();
        return nGetVersion(lifeCycle.getFmuPtr());
    }

    /*
     * (non-Javadoc)
     * @see intocps.fmuapi.IFmu#getTypesPlatform()
     */
    @Override
    public String getTypesPlatform() throws FmiInvalidNativeStateException {
        this.lifeCycle.checkState();
        return nGetTypesPlatform(lifeCycle.getFmuPtr());
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


    void synchronizedFree(FmuComponent comp) throws FmiInvalidNativeStateException {
        synchronized (NativeLoadManager.lock) {
            comp.internalFreeInstance();
        }
    }

    public static String getJniApiVersion() {
        NativeFmuLibraryLoader.loadNativeApi();

        return NativeFmu.nGetJniApiVersion();
    }


}
