package org.intocps.fmi.jnifmuapi;

import org.intocps.fmi.jnifmuapi.shared.NativeFmuApi;
import org.junit.Test;

import java.io.File;

import static org.junit.Assert.assertEquals;

public class PlatformLibraryPathTest {
    private void assertFmuPaths(String os, String arch, String fmi2Folder, String fmi3Folder, String extension) {
        File root = new File("fmu");
        assertEquals(new File(root, "binaries/" + fmi2Folder + "/model" + extension),
                FmiUtil.generateLibraryFileFromPlatform(os, arch, "model", root, FmiUtil.FMIVersion.FMI2));
        assertEquals(new File(root, "binaries/" + fmi3Folder + "/model" + extension),
                FmiUtil.generateLibraryFileFromPlatform(os, arch, "model", root, FmiUtil.FMIVersion.FMI3));
    }

    @Test
    public void macArchitectures() {
        assertFmuPaths("Mac OS X", "x86_64", "darwin64", "x86_64-darwin", ".dylib");
        assertFmuPaths("Mac OS X", "amd64", "darwin64", "x86_64-darwin", ".dylib");
        assertFmuPaths("Mac OS X", "aarch64", "darwin64", "aarch64-darwin", ".dylib");
        assertFmuPaths("Mac OS X", "arm64", "darwin64", "aarch64-darwin", ".dylib");
        assertFmuPaths("Mac OS X", "i386", "darwin32", "x86-darwin", ".dylib");
    }

    @Test
    public void linuxArchitectures() {
        assertFmuPaths("Linux", "amd64", "linux64", "x86_64-linux", ".so");
        assertFmuPaths("Linux", "x86_64", "linux64", "x86_64-linux", ".so");
        assertFmuPaths("Linux", "x86", "linux32", "x86-linux", ".so");
    }

    @Test
    public void windowsArchitectures() {
        assertFmuPaths("Windows 11", "amd64", "win64", "x86_64-windows", ".dll");
        assertFmuPaths("Windows 11", "x86_64", "win64", "x86_64-windows", ".dll");
        assertFmuPaths("Windows 10", "x86", "win32", "x86-windows", ".dll");
    }

    @Test
    public void nativeApiUsesJavaPlatformNames() {
        String originalOs = System.getProperty("os.name");
        String originalArch = System.getProperty("os.arch");
        try {
            assertNativeApiPath("Mac OS X", "x86_64", "lib/Mac-x86_64/libfmuapi.dylib");
            assertNativeApiPath("Mac OS X", "aarch64", "lib/Mac-aarch64/libfmuapi.dylib");
            assertNativeApiPath("Mac OS X", "arm64", "lib/Mac-arm64/libfmuapi.dylib");
            assertNativeApiPath("Linux", "amd64", "lib/Linux-amd64/libfmuapi.so");
            assertNativeApiPath("Windows 11", "amd64", "lib/Windows-amd64/fmuapi.dll");
        } finally {
            System.setProperty("os.name", originalOs);
            System.setProperty("os.arch", originalArch);
        }
    }

    private void assertNativeApiPath(String os, String arch, String expected) {
        System.setProperty("os.name", os);
        System.setProperty("os.arch", arch);
        assertEquals(expected, NativeFmuApi.getPlatformSpecificLibraryPath("fmuapi"));
    }

    @Test(expected = IllegalArgumentException.class)
    public void unsupportedOperatingSystem() {
        FmiUtil.generateLibraryFileFromPlatform("Unknown", "amd64", "model", new File("fmu"), FmiUtil.FMIVersion.FMI3);
    }

    @Test(expected = IllegalArgumentException.class)
    public void unsupportedFmi2Architecture() {
        FmiUtil.generateLibraryFileFromPlatform("Linux", "unknown", "model", new File("fmu"), FmiUtil.FMIVersion.FMI2);
    }

    @Test(expected = IllegalArgumentException.class)
    public void fmi2DoesNotLoadIntelLibraryOnArmLinux() {
        FmiUtil.generateLibraryFileFromPlatform("Linux", "aarch64", "model", new File("fmu"), FmiUtil.FMIVersion.FMI2);
    }
}
