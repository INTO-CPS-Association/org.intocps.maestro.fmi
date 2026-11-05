package org.intocps.fmi.jnifmuapi;

import org.apache.commons.lang3.SystemUtils;
import org.intocps.fmi.jnifmuapi.fmi3.NativeFmu3;
import org.intocps.fmi.jnifmuapi.shared.NativeFmuApi;
import org.intocps.fmi.jnifmuapi.shared.NativeLibraryLoader;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TemporaryFolder;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;

import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.Arrays;
import java.util.Collection;

import static org.junit.Assert.*;

@RunWith(Parameterized.class)
public class LibraryLoadErrorTest {
    @Parameterized.Parameters(name = "FMI {0}")
    public static Collection<Object[]> versions() {
        return Arrays.asList(new Object[][]{{2}, {3}});
    }

    @Rule
    public TemporaryFolder temporaryFolder = new TemporaryFolder();

    private final int version;

    public LibraryLoadErrorTest(int version) {
        this.version = version;
    }

    @Before
    public void loadNativeApi() {
        NativeFmuLibraryLoader.loadNativeApi();
    }

    private long load(String path) {
        if (version == 3) {
            return new Fmi3Loader().load(path);
        }
        return new DirectoryFmu(temporaryFolder.getRoot(), "error-test").nLoadLibrary(path);
    }

    @Test
    public void nonexistentLibraryIncludesPathAndLoaderReason() {
        File path = new File(temporaryFolder.getRoot(), "missing-library.dll");
        for (int i = 0; i < 10; i++) {
            Exception failure = assertThrows(Exception.class, () -> load(path.getAbsolutePath()));
            assertDiagnostic(failure, path, 126);
        }
    }

    @Test
    public void malformedLibraryIncludesPathAndLoaderReason() throws Exception {
        File path = temporaryFolder.newFile("invalid-library.dll");
        Files.write(path.toPath(), "not a native library".getBytes(StandardCharsets.UTF_8));
        Exception failure = assertThrows(Exception.class, () -> load(path.getAbsolutePath()));
        assertDiagnostic(failure, path, 193);
    }

    @Test
    public void missingFmiSymbolsDoNotCrashOrUnloadTheJniApi() throws Exception {
        File library = new NativeLibraryLoader().loadLibrary(NativeFmuApi.getPlatformSpecificLibraryPath("fmuapi"));
        Exception failure = assertThrows(Exception.class, () -> load(library.getAbsolutePath()));
        assertTrue(failure.getMessage(), failure.getMessage().contains(library.getAbsolutePath()));
        assertTrue(failure.getMessage(), failure.getMessage().contains("FMI " + version));
        assertTrue(failure.getMessage(), failure.getMessage().toLowerCase().contains("missing"));
        assertEquals("2.0", DirectoryFmu.getJniApiVersion());
    }

    @Test
    public void nullLibraryPathRaisesException() {
        Exception failure = assertThrows(Exception.class, () -> load(null));
        assertTrue(failure.getMessage().contains("must not be null"));
    }

    private void assertDiagnostic(Exception failure, File path, int windowsCode) {
        String message = failure.getMessage();
        assertNotNull(message);
        assertTrue(message, message.startsWith("Could not load \""));
        assertTrue(message, message.contains(path.getAbsolutePath()));
        int separator = message.indexOf("\": ");
        assertTrue(message, separator >= 0);
        String reason = message.substring(separator + 3).trim();
        assertFalse(message, reason.isEmpty());
        if (SystemUtils.IS_OS_WINDOWS) {
            assertTrue(message, reason.startsWith("Windows error " + windowsCode + ": "));
            assertFalse(message, reason.substring(reason.indexOf(':') + 1).trim().isEmpty());
        }
    }

    private static class Fmi3Loader extends NativeFmu3 {
        long load(String path) {
            return nLoadLibrary(path);
        }
    }
}
