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
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import java.util.zip.ZipEntry;

import org.apache.commons.compress.archivers.zip.ZipArchiveEntry;
import org.apache.commons.compress.archivers.zip.ZipFile;
import org.apache.commons.io.IOUtils;

public class ZipUtility
{

	/**
	 * Native unzip method for achieves, recursive
	 * 
	 * @param file
	 * @param outputDir
	 * @throws IOException
	 */
	static public void unzip(File file, File outputDir) throws IOException
	{
		java.util.zip.ZipFile zipFile = new java.util.zip.ZipFile(file);
		try
		{
			Enumeration<? extends ZipEntry> entries = zipFile.entries();
			while (entries.hasMoreElements())
			{
				ZipEntry entry = entries.nextElement();
				File entryDestination = new File(outputDir, entry.getName());
				if (entry.isDirectory())
				{
					entryDestination.mkdirs();
				} else
				{
					entryDestination.getParentFile().mkdirs();
					InputStream in = zipFile.getInputStream(entry);
					OutputStream out = new FileOutputStream(entryDestination);
					IOUtils.copy(in, out);
					IOUtils.closeQuietly(in);
					out.close();
				}
			}
		} finally
		{
			zipFile.close();
		}
	}

	protected static final short PERM_READ = 1 << 0;

	protected static final short PERM_WRITE = 1 << 1;

	protected static final short PERM_EXECUTE = 1 << 2;

	protected static final short PERM_APPEND = 1 << 3;

	protected static final short PERM_GFS_APPEND = 1 << 4;

	protected static final short PERM_CREATE = 1 << 5;

	protected static final short PERM_TRUNCATE = 1 << 6;

	protected static final short PERM_STRICT_READ = 1 << 7;

	protected static final short PERM_DELETE = 1 << 8;

	protected static final short PERM_SUID_SGID = 1 << 14;

	protected static final short READ_MASK = PERM_READ | PERM_STRICT_READ;

	protected static final short WRITE_MASK = PERM_WRITE | PERM_APPEND
			| PERM_GFS_APPEND | PERM_CREATE | PERM_TRUNCATE | PERM_DELETE;

	protected static final short EXEC_MASK = PERM_EXECUTE;

	protected static final short READ_ONLY_MASK = -1 & 365;

	/**
	 * Unpacks the given archive file into the output directory. Preserving *nix permissions
	 * 
	 * @param archiveFile
	 *            an archive file
	 * @param toDir
	 *            where to unpack the archive to
	 * @throws IOException
	 */
	public static void unzipApacheCompress(File archiveFile, File toDir)
			throws IOException
	{
		ZipFile zipFile = null;
		try
		{
			zipFile = new ZipFile(archiveFile);
			Enumeration<ZipArchiveEntry> zipEntries = zipFile.getEntriesInPhysicalOrder();
			while (zipEntries.hasMoreElements())
			{
				ZipArchiveEntry entry = zipEntries.nextElement();
				String entryName = entry.getName();

				File outFile = new File(toDir, entryName);
				if (!outFile.getParentFile().exists())
				{
					if (!outFile.getParentFile().mkdirs())
					{
//
//						throw new IOException("Failed to create parent directory: "
//								+ outFile.getParentFile().getCanonicalPath());
					}
				}

				if (entry.isDirectory())
				{
					if (!outFile.mkdir())
					{
//						throw new IOException("Failed to create directory: "
//								+ outFile.getCanonicalPath());
					}
				} else
				{
					InputStream zipStream = null;
					OutputStream outFileStream = null;

					zipStream = zipFile.getInputStream(entry);
					outFileStream = new FileOutputStream(outFile);
					try
					{
						IOUtils.copy(zipStream, outFileStream);
					} finally
					{
						IOUtils.closeQuietly(zipStream);
						IOUtils.closeQuietly(outFileStream);

						if ((entry.getUnixMode() & EXEC_MASK) != 0)
						{
							outFile.setExecutable(true);
						}

					}
				}
			}
		} finally
		{
			ZipFile.closeQuietly(zipFile);
		}
	}
}
