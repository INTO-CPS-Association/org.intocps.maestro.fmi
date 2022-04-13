package org.intocps.fmi.jnifmuapi;

import org.intocps.fmi.jnifmuapi.shared.NativeFmuApi;
import org.intocps.fmi.jnifmuapi.shared.NativeLibraryLoader;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.File;

public class NativeFmuLibraryLoader {
    final static Logger logger = LoggerFactory.getLogger(NativeFmuLibraryLoader.class);

    static private boolean isLoaded;

    public static void loadNativeApi() {
        if (isLoaded) {
            return;
        }
        String libPath = NativeFmuApi.getPlatformSpecificLibraryPath("fmuapi");
        logger.debug("Loading library: " + libPath);
        File lib;
        try {
            lib = new NativeLibraryLoader().loadLibrary(libPath);
            logger.debug("Loaded Library file: " + lib);
            isLoaded = true;
        } catch (Exception e) {
            logger.error("Failed to load native Library file: " + libPath, e);
            isLoaded = false;
            throw new RuntimeException("Failed to load native Library file: " + libPath, e);
        }

    }

    public static boolean isNativeApiLoaded() {
        return isLoaded;
    }
}
