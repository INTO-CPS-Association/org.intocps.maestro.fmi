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

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

abstract class NativeFmu
{
	final static Logger logger = LoggerFactory.getLogger(NativeFmu.class);

	interface ICallback
	{
		void log(String name, byte status, String category, String message);

		void stepFinished(byte fmuStatus);
	}

	static private boolean isLoaded;

	public static void loadNativeApi()
	{
		if(isLoaded)
		{
			return;
		}
		String libPath = NativeFmuApi.getPlatformSpecificLibaryPath("fmuapi");
		logger.debug("Loading library: " + libPath);
		File lib;
		try
		{
			lib = new NativeLibraryLoader().loadLibrary(libPath);
			logger.debug("Loaded Library file: " + lib);
			isLoaded = true;
		} catch (Exception e)
		{
			logger.error("Failed to load native Library file: " + libPath, e);
			isLoaded = false;
			throw new RuntimeException("Failed to load native Library file: " + libPath, e);
		}

	}

	public static boolean isNativeApiLoaded()
	{
		return isLoaded;
	}

	/* Define fmi2 function pointer types to simplify dynamic loading */

	/***************************************************
	 * Types for Common Functions
	 ****************************************************/

	/* Inquire version numbers of header files and setting logging status */
	// typedef const char* fmi2GetTypesPlatformTYPE(void);
	protected native String nGetTypesPlatform(long fmuPtr);

	// typedef const char* fmi2GetVersionTYPE(void);
	protected native String nGetVersion(long fmuPtr);

	/* Creation and destruction of FMU instances and setting debug status */
	// typedef fmi2Component fmi2InstantiateTYPE (fmi2String, fmi2Type, fmi2String, fmi2String, const
	// fmi2CallbackFunctions*, fmi2Boolean, fmi2Boolean);

	// FIXME missing callback
	protected native long ninstantiate(long fmuPtr, String name, String guid,
			String resourceLocation, boolean visible, boolean loggingOn,
			ICallback callback);

	/***************************************************
	 * C Utils
	 ****************************************************/
	protected synchronized native long nLoadLibrary(String libraryPath);

	protected synchronized native void nUnLoad(long ptr);

	protected native void callbackTest(long ptr,String name);
	protected native void throwExceptionTest() throws Exception;

	public static native long getJniApiVersion();
}
