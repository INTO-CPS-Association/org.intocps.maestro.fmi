package org.intocps.fmi.jnifmuapi;

import org.intocps.fmi.jnifmuapi.fmi2.schemas.Fmi2Schema;
import org.intocps.fmi.jnifmuapi.fmi3.schemas.Fmi3Schema;
import org.junit.Assert;
import org.junit.Test;

public class SchemaTest {
    @Test
    public void testFmi2ModelDescriptionSchema() {
        Assert.assertNotNull(new Fmi2Schema().getSchema());
    }

    @Test
    public void testFmi3ModelDescriptionSchema() {
        Assert.assertNotNull(new Fmi3Schema().getSchema());
    }
}
