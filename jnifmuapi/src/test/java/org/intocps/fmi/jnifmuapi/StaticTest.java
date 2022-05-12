package org.intocps.fmi.jnifmuapi;

import org.intocps.fmi.jnifmuapi.fmi3.DirectoryFmi3Fmu;
import org.junit.Assert;
import org.junit.Test;

public class StaticTest {

    @Test
    public void test2NativeVersion() {

        String apiVersion = Fmu.getJniApiVersion();
        Assert.assertTrue("Library should have been loaded", NativeFmuLibraryLoader.isNativeApiLoaded());
        Assert.assertEquals("2.0", apiVersion);

    }


    @Test
    public void test3NativeVersion() {

        String apiVersion = DirectoryFmi3Fmu.getJniApiFmiVersion();
        Assert.assertTrue("Library should have been loaded", NativeFmuLibraryLoader.isNativeApiLoaded());
        Assert.assertEquals("3.0", apiVersion);

    }
}
