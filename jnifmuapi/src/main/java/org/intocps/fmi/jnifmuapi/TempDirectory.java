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

import java.io.IOException;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;

public class TempDirectory
{
	final Path path;

	public TempDirectory(String prefix)
	{
		try
		{
			path = Files.createTempDirectory(prefix);
		} catch (IOException e)
		{
			throw new RuntimeException(e);
		}
	}

	public Path getPath()
	{
		return path;
	}

	public void deleteOnExit()
	{
		Runtime.getRuntime().addShutdownHook(new Thread()
		{
			@Override
			public void run()
			{
				delete(path);
			}
		});
	}

	private static void deleteOrScheduleOnExit(final Path path)
	{
		try
		{
			Files.deleteIfExists(path);
		} catch (IOException e)
		{
			try
			{
				Runtime.getRuntime().addShutdownHook(new Thread()
				{
					@Override
					public void run()
					{
						try
						{
							delete(path);
						} catch (Exception e)
						{
							// we cannot not do anything about this
						}
					}
				});
			} catch (IllegalStateException es)
			{
				// ignore
			}
		}
	}

	public static void delete(final Path path)
	{
		if (!Files.exists(path))
		{
			return;
		}
		try
		{
			Files.walkFileTree(path, new SimpleFileVisitor<Path>()
			{
				@Override
				public FileVisitResult postVisitDirectory(Path dir,
						IOException exc) throws IOException
				{
					deleteOrScheduleOnExit(dir);
					return super.postVisitDirectory(dir, exc);
				}

				@Override
				public FileVisitResult visitFile(Path file,
						BasicFileAttributes attrs) throws IOException
				{
					deleteOrScheduleOnExit(file);
					return super.visitFile(file, attrs);
				}
			});
		} catch (java.nio.file.NoSuchFileException e)
		{
			// ignore this since the file is missing anyway
		} catch (IOException e)
		{
			throw new RuntimeException(e);
		}
	}

}
