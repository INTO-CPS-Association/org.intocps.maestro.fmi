package org.intocps.fmi.jnifmuapi.fmi3;

import org.apache.commons.io.IOUtils;
import org.intocps.fmi.FmiInvalidNativeStateException;
import org.intocps.fmi.FmuInvocationException;
import org.intocps.fmi.FmuMissingLibraryException;
import org.intocps.fmi.jnifmuapi.TempDirectory;
import org.intocps.fmi.jnifmuapi.ZipUtility;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class Fmu3 extends DirectoryFmi3Fmu {
    private final File tmpFolder;
    final File path;

    boolean unpacked = false;

    /**
     * Extract fmu at path to temp dir
     *
     * @param path the fmu path
     * @throws IOException            if an io error occur
     * @throws FmuInvocationException if the native invocation fails
     */
    public Fmu3(File path) throws IOException, FmuInvocationException {
        this(path, createTempDir(getFmuName(path)), true);
    }

    public Fmu3(File path, File extractDirectory) throws FmuInvocationException {
        this(path, extractDirectory, false);
    }

    public Fmu3(File path, File extractDirectory, boolean usingTemp) throws FmuInvocationException {
        super(extractDirectory, getFmuName(path));
        this.path = path;
        if (usingTemp) {
            this.tmpFolder = extractDirectory;
        } else {
            this.tmpFolder = null;
        }
    }

    private static File createTempDir(String prefix) {
        TempDirectory dir = new TempDirectory(prefix);
        dir.deleteOnExit();
        return dir.getPath().toFile();
    }

    private static String getFmuName(File path) throws FmuInvocationException {
        if (!path.getName().contains(".fmu")) {
            throw new FmuInvocationException("invalid fmu name: " + path.getName());
        }

        return path.getName().substring(0, path.getName().indexOf('.'));
    }

    public void unPack() throws IOException {
        //noinspection ResultOfMethodCallIgnored
        this.dir.mkdirs();
        logger.debug("Extracting: " + path.getAbsolutePath() + " to " + dir.getAbsolutePath());
        ZipUtility.unzipApacheCompress(path, dir);
        logger.debug("Extracted '" + path.getAbsolutePath() + "' to '" + dir.getAbsolutePath() + "'");
        unpacked = true;
    }


    /*
     * (non-Javadoc)
     * @see intocps.fmuapi.IFmu#load()
     */
    @Override
    public void load() throws FmuInvocationException, FmuMissingLibraryException {
        logger.debug("Load FMU {}", path);
        if (lifeCycle.isLoaded()) {
            return;
        }

        if (!unpacked) {
            try {
                unPack();
            } catch (IOException e) {
                throw new FmuInvocationException(e.getMessage());
            }
        }
        super.load();
    }

    @Override
    public void unLoad() throws FmiInvalidNativeStateException {
        logger.debug("UnLoad FMU {}, temp folder {}", path, tmpFolder);
        // unload native
        super.unLoad();

        if (tmpFolder != null && tmpFolder.exists()) {
            logger.debug("Deleting temp folder {}", tmpFolder);
            TempDirectory.delete(tmpFolder.toPath());
        }

    }

    @Override
    public InputStream getModelDescription() throws IOException {

        if (!path.exists() || !path.canRead()) {
            return null;
        }

        try (ZipFile zipFile = new ZipFile(path)) {

            Enumeration<? extends ZipEntry> entries = zipFile.entries();

            while (entries.hasMoreElements()) {
                ZipEntry entry = entries.nextElement();
                if (!entry.isDirectory() && entry.getName().equals(MODEL_DESCRIPTION)) {
                    byte[] bytes = IOUtils.toByteArray(zipFile.getInputStream(entry));
                    return new ByteArrayInputStream(bytes);
                }
            }
        }
        return null;
    }


}
