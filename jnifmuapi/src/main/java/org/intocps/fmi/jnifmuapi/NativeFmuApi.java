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

class NativeFmuApi
{

	final static String LIB_BASE = "lib";

	public static String getPlatformSpecificLibaryPath(String name)
	{
		String osName = System.getProperty("os.name");

		int index = osName.indexOf(' ');
		if (index != -1)
		{
			osName = osName.substring(0, index);
		}

		String libPath = LIB_BASE + "/" + osName + "-"
				+ System.getProperty("os.arch") + "/" + name;

		if (osName.equalsIgnoreCase("windows"))
		{
			libPath += ".dll";
			return libPath;
		} else if (osName.equalsIgnoreCase("linux"))
		{
			libPath = LIB_BASE + "/" + osName + "-"
					+ System.getProperty("os.arch") + "/lib" + name + ".so";

			return libPath;
		} else if (osName.equalsIgnoreCase("Mac"))
		{
			libPath = LIB_BASE + "/" + osName + "-"
					+ System.getProperty("os.arch") + "/lib" + name + ".dylib";

			return libPath;
		}
		return null;
	}

}
