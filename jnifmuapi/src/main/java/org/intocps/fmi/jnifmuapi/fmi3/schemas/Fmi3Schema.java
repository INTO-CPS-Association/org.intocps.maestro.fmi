package org.intocps.fmi.jnifmuapi.fmi3.schemas;

import org.intocps.fmi.jnifmuapi.xml.SchemaProvider;

import java.io.InputStream;

public class Fmi3Schema implements SchemaProvider {

    public InputStream getSchema() {
        return getSchema("fmi3ModelDescription.xsd");
    }

    public InputStream getSchema(String path) {
       return this.getClass().getResourceAsStream(path);
    }


}
