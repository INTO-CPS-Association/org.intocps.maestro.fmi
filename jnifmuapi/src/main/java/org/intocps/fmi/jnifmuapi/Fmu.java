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

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipException;
import java.util.zip.ZipFile;

import org.apache.commons.io.IOUtils;
import org.intocps.fmi.FmiInvalidNativeStateException;
import org.intocps.fmi.FmuInvocationException;
import org.intocps.fmi.FmuMissingLibraryException;

final class Fmu extends DirectoryFmu
{
	private final File tmpFolder;
	final File path;

	boolean unpacked = false;

	/**
	 * Extract fmu at path to temp dir
	 * 
	 * @param path
	 * @throws IOException
	 * @throws FmuInvocationException
	 */
	public Fmu(File path) throws IOException, FmuInvocationException
	{
		this(path, createTempDir(getFmuName(path)), true);
	}

	public Fmu(File path, File extractDirectory) throws FmuInvocationException
	{
		this(path, extractDirectory, false);
	}

	public Fmu(File path, File extractDirectory, boolean usingTemp)
			throws FmuInvocationException
	{
		super(extractDirectory, getFmuName(path));
		this.path = path;
		if (usingTemp)
		{
			this.tmpFolder = extractDirectory;
		} else
		{
			this.tmpFolder = null;
		}
	}

	private static File createTempDir(String prefix)
	{
		TempDirectory dir = new TempDirectory(prefix);
		dir.deleteOnExit();
		return dir.getPath().toFile();
	}

	private static String getFmuName(File path) throws FmuInvocationException
	{
		if (path.getName().indexOf(".fmu") == -1)
		{
			throw new FmuInvocationException("invalid fmu name: "
					+ path.getName());
		}

		return path.getName().substring(0, path.getName().indexOf('.'));
	}

	public void unPack() throws IOException
	{
		this.dir.mkdirs();
		logger.debug("Extracting: " + path.getAbsolutePath() + " to "
				+ dir.getAbsolutePath());
		ZipUtility.unzipApacheCompress(path, dir);
		logger.debug("Extracted '" + path.getAbsolutePath() + "' to '"
				+ dir.getAbsolutePath() + "'");
		unpacked = true;
	}



	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmu#load()
	 */
	@Override
	public void load() throws FmuInvocationException, FmuMissingLibraryException {
		logger.debug("Load FMU {}", path);
		if (loaded)
		{
			return;
		}

		if (!unpacked)
		{
			try
			{
				unPack();
			} catch (ZipException e)
			{
				throw new FmuInvocationException(e.getMessage());
			} catch (IOException e)
			{
				throw new FmuInvocationException(e.getMessage());
			}
		}
		super.load();
	}

	@Override
	public void unLoad() throws FmiInvalidNativeStateException
	{
		logger.debug("UnLoad FMU {}, temp folder", path, tmpFolder);
		// unload native
		super.unLoad();

		if (tmpFolder != null && tmpFolder.exists())
		{
			logger.debug("Deleting temp folder {}", tmpFolder);
			TempDirectory.delete(tmpFolder.toPath());
		}

	}

	@Override
	public InputStream getModelDescription() throws ZipException, IOException
	{

		if (!path.exists() || !path.canRead())
		{
			return null;
		}

		ZipFile zipFile = null;
		try
		{
			zipFile = new ZipFile(path);

			Enumeration<? extends ZipEntry> entries = zipFile.entries();

			while (entries.hasMoreElements())
			{
				ZipEntry entry = entries.nextElement();
				if (!entry.isDirectory()
						&& entry.getName().equals(MODEL_DESCRIPTION))
				{
					byte[] bytes = IOUtils.toByteArray(zipFile.getInputStream(entry));
					return new ByteArrayInputStream(bytes);
				}
			}
		} finally
		{
			if (zipFile != null)
			{
				zipFile.close();
			}
		}
		return null;
	}

	public void testNativeCallback() throws FmiInvalidNativeStateException
	{
		checkState();
		super.callbackTest(getFmuPtr(),name);
	}

}
