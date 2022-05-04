package org.intocps.fmi.jnifmuapi.xml;

import java.io.InputStream;

public interface SchemaProvider {
    InputStream getSchema();

    InputStream getSchema(String path);
}
