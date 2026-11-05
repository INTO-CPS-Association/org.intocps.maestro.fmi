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
import java.lang.reflect.Field;
import java.util.Arrays;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.xpath.XPathExpressionException;

import org.intocps.fmi.Fmi2Status;
import org.intocps.fmi.Fmi2StatusKind;
import org.intocps.fmi.FmuInvocationException;
import org.intocps.fmi.FmuResult;
import org.intocps.fmi.IFmiComponent;
import org.intocps.fmi.IFmuCallback;
import org.intocps.fmi.InvalidParameterException;
import org.intocps.fmi.jnifmuapi.shared.NativeFmuApi;
import org.intocps.fmi.jnifmuapi.shared.NativeLibraryLoader;
import org.xml.sax.SAXException;

public class Main
{

	public static void main(String[] args) throws Exception
	{

		String libPath = NativeFmuApi.getPlatformSpecificLibraryPath("fmuapi");
		System.out.println("Loading library: " + libPath);
		File lib = new NativeLibraryLoader().loadLibrary(libPath);
		System.out.println("Loaded Library file: " + lib);

		long sbase = System.currentTimeMillis();
		Fmu f = new Fmu(new File("fmu2test1.fmu"), new File(new File("Default"), "fmus"));

		f.unPack();

		f.load();

		System.out.println("FMU version: " + f.getVersion());

		long s = System.currentTimeMillis();
		for (int i = 0; i < 10; i++)
		{
			testInstance(f);
		}

		long end = System.currentTimeMillis() - s;
		System.out.println("####\n\nit took " + end + " milis");

		long endTotal = System.currentTimeMillis() - sbase;
		System.out.println("Total " + endTotal + " milis");
	}

	public static FmuModelDescription getModelDescription(Fmu fmu)
	{
		try
		{
			return new FmuModelDescription(new File(fmu.dir, "modelDescription.xml"));
		} catch (ParserConfigurationException | SAXException | IOException e)
		{
			e.printStackTrace();
			return null;
		}

	}

	private static void testInstance(Fmu f) throws XPathExpressionException,
			FmuInvocationException
	{
		boolean visible = false;
		boolean loggingOn = false;

		// instantiate component
		FmuModelDescription description = getModelDescription(f);
		IFmiComponent instnace = f.instantiate(description.getGuid(), f.name, visible, loggingOn, new IFmuCallback()
		{

			@Override
			public void log(String instanceName, Fmi2Status status,
					String category, String message)
			{
				System.out.println("log: " + instanceName + " " + status + " "
						+ category + " " + message);
			}

			@Override
			public void stepFinished(Fmi2Status status)
			{

			}

		});

		/**
		 * if (nCategories > 0) <br>
		 * { fmi2Flag = fmu->setDebugLogging(c, fmi2True, nCategories, categories);<br>
		 * if (fmi2Flag > fmi2Warning)<br>
		 * { return error("could not initialize model; failed FMI set debug logging"); } }
		 */
		Fmi2Status fmi2Flag; // return code of the fmu functions

		boolean toleranceDefined = false;
		double tolerance = 1;
		double tStart = 0;
		double tEnd = 20;

		fmi2Flag = instnace.setupExperiment(toleranceDefined, tolerance, tStart, true, tEnd);
		if (fmi2Flag.value > Fmi2Status.Warning.value)
		{
			error("could not initialize model; failed FMI setup experiment");
		}

		fmi2Flag = instnace.enterInitializationMode();
		if (fmi2Flag.value > Fmi2Status.Warning.value)
		{
			error("could not initialize model; failed FMI enter initialization mode");
		}

		fmi2Flag = instnace.exitInitializationMode();
		if (fmi2Flag.value > Fmi2Status.Warning.value)
		{
			error("could not initialize model; failed FMI exit initialization mode");
		}

		Fmi2Status status = null;

		double h = 0.01;
		int nSteps = 0;
		// enter the simulation loop
		double time = tStart;
		while (time < tEnd)
		{
			System.out.println("do step: " + time);
			status = instnace.doStep(time, h, true);
			if (status == Fmi2Status.Discard)
			{
				// check if model requests to end simulation
				FmuResult<Boolean> terminateRes = instnace.getBooleanStatus(Fmi2StatusKind.Terminated);
				if (Fmi2Status.OK != terminateRes.status)
				{
					error("could not complete simulation of the model. getBooleanStatus return other than fmi2OK");
				}
				if (terminateRes.result)
				{
					error("the model requested to end the simulation");
				}
				error("could not complete simulation of the model");
			}
			if (status != Fmi2Status.OK)
			{
				error("could not complete simulation of the model");
			}
			time += h;

			long[] realIndices = new long[] { 0, 1, 2 };

			FmuResult<double[]> reals = instnace.getReal(realIndices);
			if (reals == null)
			{
				System.err.println("failed to get reals");
			}

			try
			{
				instnace.setReals(realIndices, new double[] { 1.1, 1.2, 1.3 });
			} catch (InvalidParameterException e)
			{
				e.printStackTrace();
			}

			//
			nSteps++;
		}

		System.out.println("Simulated with " + nSteps + " steps");

		// end simulation
		instnace.terminate();
	}

	static private void error(String msg)
	{
		System.err.println(msg);
	}

	/**
	 * Adds the specified path to the java library path
	 *
	 * @param pathToAdd
	 *            the path to add
	 * @throws Exception
	 */
	public static void addLibraryPath(String pathToAdd) throws Exception
	{
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
