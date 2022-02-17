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

import org.intocps.fmi.Fmi2Status;
import org.intocps.fmi.IFmuCallback;

import java.io.File;
import java.nio.file.Paths;

public class TestUtils {
    public static final String guid = "{12345678-9999-9999-9999-000000000000}";
    public static final String DUMMY_FMU_PATH = Paths.get("target", "fmus", "fmi2", "fmi2functiontest.fmu").toString();
    public static final int REAL_ID = 1;
    public static final long INTEGER_ID = 0;
    public static final long BOOLEAN_ID = 0;
    public static final File TEST_EXTRACT_DIR = new File(new File("target"), "fmus");


    public static IFmuCallback getTestCallback() {
        return new IFmuCallback() {

            @Override
            public void stepFinished(Fmi2Status status) {

            }

            @Override
            public void log(String instanceName, Fmi2Status status, String category, String message) {
                System.out.println("log: " + instanceName + " " + status + " " + category + " " + message);
            }
        };
    }
}
