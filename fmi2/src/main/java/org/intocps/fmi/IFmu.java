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

package org.intocps.fmi;

import java.io.IOException;
import java.io.InputStream;
import java.util.zip.ZipException;

import javax.xml.xpath.XPathExpressionException;


public interface IFmu
{
	public abstract void load() throws FmuInvocationException, FmuMissingLibraryException;

	/**
	 * Creates an instance of the FMU
	 * 
	 * @param name
	 * @param visible
	 * @param loggingOn
	 * @return
	 * @throws XPathExpressionException
	 * @throws FmiInvalidNativeStateException 
	 */
	public abstract IFmiComponent instantiate(String guid, String name,
			boolean visible, boolean loggingOn, IFmuCallback callback)
			throws XPathExpressionException, FmiInvalidNativeStateException;

	public abstract void unLoad() throws FmiInvalidNativeStateException;

	public abstract String getVersion() throws FmiInvalidNativeStateException;

	public abstract String getTypesPlatform() throws FmiInvalidNativeStateException;
	
	public abstract InputStream getModelDescription() throws ZipException, IOException;
	
	public abstract boolean isValid();

}
