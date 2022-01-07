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

package org.intocps.fmi.jnifmuapi.fmi3;

import org.junit.Assert;
import org.junit.Test;

import java.io.File;

public class BouncingBallFmi3Test {
    static final String FMU_UNPACKED_PATH = "target/fmus/fmi3/bounchingball".replace('/', File.separatorChar);
    static final String FMU_GUID = "{8c4e810f-3df3-4a00-8276-176fa3c9f003}";

    @Test
    public void LoadFmuTempDir() throws Exception {
        DirectoryFmi3Fmu f = new DirectoryFmi3Fmu(new File(FMU_UNPACKED_PATH), "bouncingball");
        f.load();
        f.unLoad();
    }

    @Test
    public void InstantiateForCoSimulation() throws Exception {
        DirectoryFmi3Fmu f = new DirectoryFmi3Fmu(new File(FMU_UNPACKED_PATH), "bouncingball");
        f.load();
        final String[] instanceName_ = new String[1];
        final Fmi3Status[] status_ = new Fmi3Status[1];
        final String[] category_ = new String[1];
        final String[] message_ = new String[1];
        ICallbackLogMessage lm = new ICallbackLogMessage() {

            @Override
            public void logMessage(String instanceName, Fmi3Status status, String category, String message) {
                instanceName_[0] = instanceName;
                status_[0] = status;
                category_[0] = category;
                message_[0] = message;
                System.out.println(String.format("Received log message:\n" + "instanceName: %s\n" + "Status: %s\n" + "Category: %s\n" + "Message: %s",
                        instanceName, status.name(), category, message));
            }
        };
        IFmi3Instance instance = f.instantiateCoSimulation("Bouncing Ball", FMU_GUID, null, false, false, false, false, null, 0, 0, lm, null);
        Assert.assertNotNull("Instantiate returned null", instance);
        Fmi3Instance fmi3Instance = null;
        if (instance instanceof Fmi3Instance) {
            fmi3Instance = (Fmi3Instance) instance;
        }
        Fmi3Status status = fmi3Instance.terminate();
        Fmi3Status expectedFmi3Status = Fmi3Status.fmi3Error;
        Assert.assertEquals("Terminate result status did not match", expectedFmi3Status, status);
        Assert.assertEquals("Log Instance name did not match", "Bouncing Ball", instanceName_[0]);
        Assert.assertEquals("Log Fmi3Status did not match", expectedFmi3Status, status_[0]);
        Assert.assertEquals("Log Category", "logStatusError", category_[0]);
        Assert.assertEquals("Log Message", "fmi3Terminate: Illegal call sequence.", message_[0]);


        f.unLoad();
    }
}
