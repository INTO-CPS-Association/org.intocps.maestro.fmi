package org.intocps.fmi.jnifmuapi;

import org.junit.Assert;
import org.junit.Test;

public class StaticTest {
    @Test
    public void StaticTest() {

        Assert.assertEquals("2.0.0", Fmu.getJniApiVersion());
    }
}
