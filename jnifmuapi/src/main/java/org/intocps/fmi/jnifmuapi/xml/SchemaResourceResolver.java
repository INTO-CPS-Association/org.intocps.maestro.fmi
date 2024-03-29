package org.intocps.fmi.jnifmuapi.xml;

import org.w3c.dom.ls.LSInput;
import org.w3c.dom.ls.LSResourceResolver;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.Reader;

public class SchemaResourceResolver implements LSResourceResolver {


    public SchemaResourceResolver(SchemaProvider provider) {
        this.provider = provider;
    }

    final SchemaProvider provider;


    @Override
    public LSInput resolveResource(String type, String namespaceURI, String publicId, String systemId, String baseURI) {

        // note: in this sample, the XSD's are expected to be in the root of the classpath
        InputStream resourceAsStream = provider.getSchema(systemId);
        return new Input(publicId, systemId, resourceAsStream);
    }

    public static class Input implements LSInput {

        private String publicId;

        private String systemId;
        private BufferedInputStream inputStream;

        public Input(String publicId, String sysId, InputStream input) {
            this.publicId = publicId;
            this.systemId = sysId;
            this.inputStream = new BufferedInputStream(input);
        }

        @Override
        public String getPublicId() {
            return publicId;
        }

        @Override
        public void setPublicId(String publicId) {
            this.publicId = publicId;
        }

        @Override
        public String getBaseURI() {
            return null;
        }

        @Override
        public void setBaseURI(String baseURI) {
        }

        @Override
        public InputStream getByteStream() {
            return null;
        }

        @Override
        public void setByteStream(InputStream byteStream) {
        }

        @Override
        public boolean getCertifiedText() {
            return false;
        }

        @Override
        public void setCertifiedText(boolean certifiedText) {
        }

        @Override
        public Reader getCharacterStream() {
            return null;
        }

        @Override
        public void setCharacterStream(Reader characterStream) {
        }

        @Override
        public String getEncoding() {
            return null;
        }

        @Override
        public void setEncoding(String encoding) {
        }

        @Override
        public String getStringData() {
            synchronized (inputStream) {
                try {
                    byte[] input = new byte[inputStream.available()];
                    inputStream.read(input);
                    String contents = new String(input);
                    return contents;
                } catch (IOException e) {
                    e.printStackTrace();
                    System.out.println("Exception " + e);
                    return null;
                }
            }
        }

        @Override
        public void setStringData(String stringData) {
        }

        @Override
        public String getSystemId() {
            return systemId;
        }

        @Override
        public void setSystemId(String systemId) {
            this.systemId = systemId;
        }

        public BufferedInputStream getInputStream() {
            return inputStream;
        }

        public void setInputStream(BufferedInputStream inputStream) {
            this.inputStream = inputStream;
        }
    }

}