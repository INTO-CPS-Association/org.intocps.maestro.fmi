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
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

class NativeLibraryLoader
{
	final static Logger logger = LoggerFactory.getLogger(NativeLibraryLoader.class);

	static Map<String, File> loadedLibs = new HashMap<String, File>();

	public static File getTempFolder()
	{
		TempDirectory tempDir = new TempDirectory("jnifmuapi");
		tempDir.deleteOnExit();
		return tempDir.getPath().toFile();
	}

	public File loadLibrary(String relativeLibraryPath) throws Exception
	{
		if (loadedLibs.containsKey(relativeLibraryPath))
		{
			return loadedLibs.get(relativeLibraryPath);
		}

		logger.debug("Extracting library from: {}", relativeLibraryPath);
		File lib = extractNativeLibrary(getTempFolder(), relativeLibraryPath);
		logger.debug("Extracted library: {}", lib);

		if (lib == null)
		{
			throw new FileNotFoundException("Unable to find the native lib: "
					+ relativeLibraryPath);
		}

		String libname = lib.getName();

		int libnameDotIndex = libname.indexOf('.');

		if (libnameDotIndex != -1)
		{
			libname = libname.substring(0, libnameDotIndex);
		}

		if (libname.startsWith("lib")
				&& (System.getProperty("os.name").equalsIgnoreCase("linux") || System.getProperty("os.name").toLowerCase().startsWith("mac")))
		{
			libname = libname.substring(3);
		}

		// Add the folder in which the library exists to the lib load path
		addLibraryPath(lib.getParentFile());

		System.loadLibrary(libname);

		loadedLibs.put(relativeLibraryPath, lib);

		return lib;
	}

	public File extractNativeLibrary(File outputFolder,
			String relativeLibraryPath) throws IOException
	{
		String name = relativeLibraryPath;

		int slashIndex = name.lastIndexOf('/');

		if (slashIndex != -1)
		{
			name = name.substring(slashIndex + 1);
		}

		String osName = System.getProperty("os.name");

		int index = osName.indexOf(' ');
		if (index != -1)
		{
			osName = osName.substring(0, index);
		}

		outputFolder = new File(outputFolder, "jnifmuapi-" + osName + "-"
				+ System.getProperty("os.arch"));
		outputFolder.mkdirs();

		File library = new File(outputFolder, name);

		InputStream in = this.getClass().getClassLoader().getResourceAsStream(relativeLibraryPath);
		if (in != null)
		{
			Files.copy(in, Paths.get(library.getAbsolutePath()), StandardCopyOption.REPLACE_EXISTING);

			return library;
		}
		return null;
	}

	/**
	 * Adds the specified path to the java library path
	 *
	 * @param pathToAdd
	 *            the path to add
	 * @throws Exception
	 */
	public static void addLibraryPath(File dirPathToAdd) throws Exception
	{
		final String pathToAdd = dirPathToAdd.getAbsolutePath();
		final Field usrPathsField = ClassLoader.class.getDeclaredField("usr_paths");
		usrPathsField.setAccessible(true);

		// get array of paths
		final String[] paths = (String[]) usrPathsField.get(null);

		// check if the path to add is already present
		for (String path : paths)
		{
			if (path.equals(pathToAdd))
			{
				return;
			}
		}

		// add the new path
		final String[] newPaths = Arrays.copyOf(paths, paths.length + 1);
		newPaths[newPaths.length - 1] = pathToAdd;
		usrPathsField.set(null, newPaths);
	}

}
