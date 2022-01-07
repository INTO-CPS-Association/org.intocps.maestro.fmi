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

import org.intocps.fmi.FmiInvalidNativeStateException;
import org.intocps.fmi.FmuInvocationException;
import org.intocps.fmi.FmuMissingLibraryException;
import org.intocps.fmi.IFmiComponent;
import org.junit.*;

import java.io.File;
import java.io.IOException;

public abstract class BaseFmuComponent {

    static Fmu fmu;
    protected IFmiComponent comp;

    @BeforeClass
    public static void setupFmu() throws IOException, FmuInvocationException, FmuMissingLibraryException {
        fmu = new Fmu(new File(TestUtils.DUMMY_FMU_PATH), TestUtils.TEST_EXTRACT_DIR);

        // fmu.unPack();
        // fmu.load();

        fmu.lifeCycle.internalLoad(TestUtils.getLibraryPath());
    }

    @AfterClass
    public static void freeFmu() throws FmiInvalidNativeStateException {
        fmu.unLoad();
    }

    public BaseFmuComponent() {
        super();
    }

    @Before
    public void setup() throws Exception {
        comp = fmu.instantiate(TestUtils.guid, "test", true, true, TestUtils.getTestCallback());
        Assert.assertTrue("Instance not valid", comp.isValid());
    }

    @After
    public void free() throws FmuInvocationException {
        comp.freeInstance();
    }

}
