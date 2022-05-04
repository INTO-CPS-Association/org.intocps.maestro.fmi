package org.intocps.fmi.jnifmuapi.fmi2.schemas;

import org.apache.commons.io.IOUtils;
import org.intocps.fmi.jnifmuapi.xml.SchemaProvider;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;

public class Fmi2Schema implements SchemaProvider {
    public InputStream getSchema() {
        return getSchema("fmi2ModelDescription.xsd");
    }

    public InputStream getSchema(String path) {


        InputStream is = Fmi2Schema.class.getResourceAsStream(path);

        if (path.endsWith("fmi2AttributeGroups.xsd") && is != null) {
            //FIX for fmi2 as the release version of the schema is invalid. Its likely using utf-16 or similar oppose to UTF-8

            byte[] feff = new byte[]{(byte) 0xEF, (byte) 0xBB, (byte) 0xBF};
            byte[] bytes;
            try {
                bytes = IOUtils.toByteArray(is);
                if (bytes.length > 3 && bytes[0] == feff[0] && bytes[1] == feff[1] && bytes[2] == feff[2]) {
                    bytes = Arrays.copyOfRange(bytes, 3, bytes.length);

                }
            } catch (IOException e) {
                e.printStackTrace();
                return null;
            }
            is = new ByteArrayInputStream(bytes);
        }
        return is;

    }


}
