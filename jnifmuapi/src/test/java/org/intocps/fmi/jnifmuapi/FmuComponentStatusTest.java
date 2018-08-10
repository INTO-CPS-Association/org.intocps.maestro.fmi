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

import org.intocps.fmi.Fmi2StatusKind;
import org.intocps.fmi.FmuInvocationException;
import org.junit.Test;

// Before running these tests extract the latest fmi.native lib and place the lib folder in resources. See https://github.com/INTO-CPS-Association/org.intocps.maestro.fmi
public class FmuComponentStatusTest extends BaseFmuComponent
{
	@Override
	public void setup() throws Exception
	{
		super.setup();
		comp.enterInitializationMode();
		comp.exitInitializationMode();
		comp.doStep(0, 0.1, false);
	}

	// @Test
	// public void testGetStatus()
	// {
	// FmuResult<Fmi2Status> res = comp.getStatus(Fmi2StatusKind.DoStepStatus);
	// Assert.assertEquals(Fmi2Status.OK, res.status);
	// Assert.assertEquals(Fmi2Status.OK, res.result);
	// }

	@Test
	public void testGetStatus() throws FmuInvocationException
	{

		/* FmuResult<Double> res = */comp.getRealStatus(Fmi2StatusKind.LastSuccessfulTime);
		// Assert.assertEquals(Fmi2Status.OK, res.status);
		// Assert.assertEquals(Fmi2Status.OK, res.result);
	}
}
