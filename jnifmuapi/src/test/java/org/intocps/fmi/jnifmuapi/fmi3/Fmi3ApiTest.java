package org.intocps.fmi.jnifmuapi.fmi3;

import org.junit.Assert;
import org.junit.Test;

public class Fmi3ApiTest {
    @Test
    public void testPlatformVersion() {
        Assert.assertEquals("3.0-beta.3", DirectoryFmi3Fmu.getJniApiFmiVersion());
    }
}
