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

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.zip.ZipException;

import org.apache.commons.io.IOUtils;
import org.intocps.fmi.FmuInvocationException;
import org.junit.Assert;
import org.junit.Test;

public class FmuModelDescriptiontest
{

	private static final String SRC_TEST_RESOURCES_FMU2TEST1_FMU = "src/test/resources/fmu2test1.fmu";

	//private static final String SRC_TEST_RESOURCES_FMU2TEST1_FMU = "/Users/kela/Downloads/I26A0640.fmu";
//	private static final String SRC_TEST_RESOURCES_FMU2TEST1_FMU = "/Users/kel/Downloads/BooleanNetwork1.fmu";
	
	
	
	@Test
	public void fmuLoadModelDescriptionTest() throws ZipException, IOException,
			FmuInvocationException
	{
		InputStream in = new Fmu(new File(SRC_TEST_RESOURCES_FMU2TEST1_FMU.replace('/', File.separatorChar))).getModelDescription();
		
		String md = IOUtils.toString(in);
		Assert.assertNotNull(md);
		Assert.assertFalse(md.isEmpty());;
	}
}
