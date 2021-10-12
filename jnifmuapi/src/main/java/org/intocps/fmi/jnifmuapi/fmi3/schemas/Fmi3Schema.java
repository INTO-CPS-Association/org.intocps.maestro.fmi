package org.intocps.fmi.jnifmuapi.fmi3.schemas;

import java.io.InputStream;

public class Fmi3Schema {
    public static InputStream getSchema(){
        return Fmi3Schema.class.getResourceAsStream("fmi3ModelDescription.xsd");
    }
}
