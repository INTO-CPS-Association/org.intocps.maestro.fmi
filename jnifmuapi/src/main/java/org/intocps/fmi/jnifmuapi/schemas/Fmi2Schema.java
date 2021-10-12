package org.intocps.fmi.jnifmuapi.schemas;

import java.io.InputStream;

public class Fmi2Schema {
    public static InputStream getSchema(){
        return Fmi2Schema.class.getResourceAsStream("fmi2ModelDescription.xsd");
    }
}
