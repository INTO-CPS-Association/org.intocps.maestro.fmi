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

import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.Vector;

import javax.xml.xpath.XPathExpressionException;

import org.apache.commons.io.FileUtils;
import org.intocps.fmi.Fmi2Status;
import org.intocps.fmi.FmiInvalidNativeStateException;
import org.intocps.fmi.FmuInvocationException;
import org.intocps.fmi.IFmiComponent;
import org.intocps.fmi.IFmu;
import org.intocps.fmi.IFmuCallback;
import org.intocps.fmi.jnifmuapi.Factory;

public class Main
{

	public static void main(String[] args) throws IOException,
			InterruptedException
	{
		File testDir = new File("target/conc-test");
		FileUtils.deleteDirectory(testDir);

		testDir.mkdirs();

		String fmuPath = args[0];

		System.out.println("FMU is: " + fmuPath);

		System.out.println("Starting replication...");

		File sourceFmu = new File(fmuPath);

		List<File> fmus = new Vector<>();
		for (int i = 0; i < Integer.parseInt(args[1]); i++)
		{
			File destFmu = new File(new File(testDir, "test-" + i), sourceFmu.getName());
			destFmu.getParentFile().mkdirs();

			FileUtils.copyFile(sourceFmu, destFmu);
			fmus.add(destFmu);
			System.out.println("\tGenerated: " + destFmu.getPath());
		}

		System.out.println("Forking threads ...");

		final List<Thread> threads = new Vector<Thread>();
		int tid = 0;

		for (final File file : fmus)
		{
			final Thread wokrer = createWorker(threads, file, tid++);
			wokrer.start();
			threads.add(wokrer);
		}
		Thread.sleep(200);
		for (final File file : fmus)
		{
			final Thread wokrer = createWorker(threads, file, tid++);
			wokrer.start();
			threads.add(wokrer);
		}

		boolean finished = false;
		while (!finished)
		{
			Thread.sleep(1000);
			synchronized (threads)
			{
				finished = threads.isEmpty();
				if (!finished)
				{
					System.out.println("Waiting for " + threads.size()
							+ " threads");
				}
			}
		}

		System.out.println(" ### DONE ###");
	}

	private static Thread createWorker(final List<Thread> threads,
			final File file, final int ftid)
	{
		System.out.println("\tForking thread (" + ftid + ") for: "
				+ file.getPath());
		final Thread wokrer = new Thread(new Runnable()
		{

			@Override
			public void run()
			{
				try
				{
					final IFmu fmu = Factory.create(file);
					fmu.load();
					fmu.getVersion();

					final int INSTANCE_COUNT = 1000;

					Thread helper = new Thread(new Runnable()
					{

						@Override
						public void run()
						{
							try
							{
								for (int i = 0; i < INSTANCE_COUNT / 2; i++)
								{
									IFmiComponent comp = fmu.instantiate("{8c4e810f-3df3-4a00-8276-176fa3c9f000}", "n", false, false, createCallback());

									if (comp != null)
									{
										comp.enterInitializationMode();
										comp.exitInitializationMode();
										comp.freeInstance();
									} else
									{
										System.err.println("No instance created("
												+ ftid
												+ "). Aborting: "
												+ file.getPath());
										return;
									}
								}
							} catch (FmiInvalidNativeStateException e)
							{
								handleError(e);
							} catch (FmuInvocationException e)
							{
								handleError(e);
							} catch (XPathExpressionException e)
							{
								handleError(e);
							}
						}
					});
					helper.start();

					for (int i = 0; i < INSTANCE_COUNT / 2; i++)
					{
						IFmiComponent comp = fmu.instantiate("{8c4e810f-3df3-4a00-8276-176fa3c9f000}", "n", false, false, createCallback());

						if (comp != null)
						{
							comp.enterInitializationMode();
							comp.exitInitializationMode();
							comp.freeInstance();
						} else
						{
							System.err.println("No instance created(" + ftid
									+ "). Aborting: " + file.getPath());
							return;
						}
					}

					helper.join();
					fmu.unLoad();
					System.out.println("\tComplete (" + ftid + "): "
							+ file.getPath());
				} catch (IOException | FmuInvocationException e)
				{
					handleError(e);
				} catch (XPathExpressionException e)
				{
					handleError(e);
				} catch (InterruptedException e)
				{
					handleError(e);
				} finally
				{
					synchronized (threads)
					{
						threads.remove(Thread.currentThread());
					}
				}
			}
		});
		return wokrer;
	}

	private static void handleError(Throwable t)
	{
		t.printStackTrace();
	}

	private static IFmuCallback createCallback()
	{
		return new IFmuCallback()
		{

			@Override
			public void stepFinished(Fmi2Status status)
			{
				// ignore
			}

			@Override
			public void log(String instanceName, Fmi2Status status,
					String category, String message)
			{
				System.out.println(message);
			}
		};
	}

}
