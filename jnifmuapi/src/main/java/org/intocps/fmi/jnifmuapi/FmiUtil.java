/*
 * This file is part of the INTO-CPS toolchain.
 *
 * Copyright (c) 2017-CurrentYear, INTO-CPS Association,
 * c/o Professor Peter Gorm Larsen, Department of Engineering
 * Finlandsgade 22, 8200 Aarhus N.
 *
 * All rights reserved.
 *
 * THIS PROGRAM IS PROVIDED UNDER THE TERMS OF GPL VERSION 3 LICENSE OR
 * THIS INTO-CPS ASSOCIATION PUBLIC LICENSE VERSION 1.0.
 * ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS PROGRAM CONSTITUTES
 * RECIPIENT'S ACCEPTANCE OF THE OSMC PUBLIC LICENSE OR THE GPL
 * VERSION 3, ACCORDING TO RECIPIENTS CHOICE.
 *
 * The INTO-CPS toolchain  and the INTO-CPS Association Public License
 * are obtained from the INTO-CPS Association, either from the above address,
 * from the URLs: http://www.into-cps.org, and in the INTO-CPS toolchain distribution.
 * GNU version 3 is obtained from: http://www.gnu.org/copyleft/gpl.html.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without
 * even the implied warranty of  MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE, EXCEPT AS EXPRESSLY SET FORTH IN THE
 * BY RECIPIENT SELECTED SUBSIDIARY LICENSE CONDITIONS OF
 * THE INTO-CPS ASSOCIATION.
 *
 * See the full INTO-CPS Association Public License conditions for more details.
 */

/*
 * Author:
 *		Kenneth Lausdahl
 */

package org.intocps.fmi.jnifmuapi;

import org.intocps.fmi.FmuInvocationException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.xpath.*;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Path;

public class FmiUtil {

    public enum FMIVersion {
        FMI2,
        FMI3
    }

    final static Logger logger = LoggerFactory.getLogger(FmiUtil.class);

    /**
     * Function to generate FMI library path from OS name OS arch modelIdentifier and the root for the FMU
     *
     * @param osName          os name
     * @param osArch          arch name
     * @param modelIdentifier model identifier
     * @param fmuRoot         root path
     * @return the platform file
     */
    public static File generateLibraryFileFromPlatform(String osName, String osArch, String modelIdentifier, File fmuRoot, FMIVersion fmiVersion) {
        String arch = osArch;

        String libDir = "";
        String libExtension = "";

        if (osName.toLowerCase().contains("windows")) {
            libExtension = ".dll";
            switch (fmiVersion) {
                case FMI2:
                    if (arch.contains("amd64")) {
                        libDir = "win64";
                    } else {
                        // x86
                        libDir = "win32";
                    }
                case FMI3:
                    if (arch.contains("amd64")) {
                        libDir = "x86_64-windows";
                    } else {
                        // x86
                        libDir = "x86-windows";
                    }
            }
        } else if (osName.contains("nix") || osName.contains("nux") || osName.indexOf("aix") > 0) {
            libExtension = ".so";
            switch (fmiVersion) {
                case FMI2:
                    if (arch.contains("amd64")) {
                        libDir = "linux64";
                    } else {
                        // x86
                        libDir = "linux32";
                    }
                case FMI3:
                    if (arch.contains("amd64")) {
                        libDir = "x86_64-linux";
                    } else {
                        // x86
                        libDir = "x86-linux";
                    }
            }
        } else if (osName.toLowerCase().contains("mac"))// extension
        {
            libExtension = ".dylib";
            switch (fmiVersion) {
                case FMI2:
                    if (arch.contains("x86_64")) {
                        libDir = "darwin64";
                    } else {
                        // x86
                        libDir = "darwin32";
                    }
                    break;
                case FMI3:
                    if (arch.contains("x86_64")) {
                        libDir = "x86_64-darwin";
                    } else {
                        // x86
                        libDir = "x86_32-darwin";
                    }
                    break;
            }
        }

        File libraryPath = new File(new File(new File(fmuRoot, "binaries"), libDir), modelIdentifier + libExtension);
        return libraryPath;
    }

    /**
     * Function to obtain the model identifier from a modelDescription.xml stream
     *
     * @param modelDescription
     * @return
     */
    public static String getModelIdentifier(InputStream modelDescription) {
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        factory.setNamespaceAware(true);
        try {
            DocumentBuilder builder = factory.newDocumentBuilder();

            Document doc = builder.parse(modelDescription);
            XPathFactory xPathfactory = XPathFactory.newInstance();
            XPath xpath = xPathfactory.newXPath();

            XPathExpression expr = xpath.compile("fmiModelDescription/CoSimulation/@modelIdentifier");

            final NodeList list = (NodeList) expr.evaluate(doc, XPathConstants.NODESET);

            if (list != null) {
                Node n = list.item(0);
                return n.getNodeValue();
            }

        } catch (ParserConfigurationException | SAXException | IOException | XPathExpressionException e) {
            logger.error("Unable to parse model description", e);
        }
        return null;
    }

    public static File generateLibraryFile(String modelIdentifier, File dir, FMIVersion fmiVersion) {
        String osName = System.getProperty("os.name");
        String arch = System.getProperty("os.arch");

        return FmiUtil.generateLibraryFileFromPlatform(osName, arch, modelIdentifier, dir, fmiVersion);
    }

    public static String logMessageLibraryPath(File libraryPath) {
        Path p = libraryPath.toPath();
        int pLength = p.getNameCount();
        String desiredPath = p.subpath(pLength - 3, pLength).toString();
        return desiredPath;
    }

    public static File createTempDir(String prefix) {
        TempDirectory dir = new TempDirectory(prefix);
        dir.deleteOnExit();
        return dir.getPath().toFile();
    }

    public static void unPack(File fromFile, File toDirectory) throws IOException {
        toDirectory.mkdirs();
        logger.debug("Extracting: " + fromFile.getAbsolutePath() + " to " + toDirectory.getAbsolutePath());
        ZipUtility.unzipApacheCompress(fromFile, toDirectory);
        logger.debug("Extracted '" + fromFile.getAbsolutePath() + "' to '" + toDirectory.getAbsolutePath() + "'");
    }

    public static String getFmuName(File path) throws FmuInvocationException {
        if (path.getName().indexOf(".fmu") == -1) {
            throw new FmuInvocationException("invalid fmu name: " + path.getName());
        }

        return path.getName().substring(0, path.getName().indexOf('.'));
    }
}
