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

import org.intocps.fmi.Fmi2Status;
import org.intocps.fmi.FmuInvocationException;
import org.intocps.fmi.FmuResult;
import org.intocps.fmi.IFmiComponentState;
import org.intocps.fmi.InvalidParameterException;
import org.junit.Assert;
import org.junit.Test;

public class FmuComponentTest extends BaseFmuComponent
{
	@Test
	public void setDebugLogging() throws FmuInvocationException
	{
		comp.setDebugLogging(true, new String[] { "loglevel1", "loglevel2" });
	}

	@Test
	public void setReset() throws FmuInvocationException
	{
		comp.reset();
	}

	@Test
	public void initModesTest() throws FmuInvocationException
	{
		comp.enterInitializationMode();
		comp.exitInitializationMode();
	}

	@Test
	public void setupExperimentTest() throws FmuInvocationException
	{
		boolean toleranceDefined = true;
		double tolerance = 0.1;
		double startTime = 0;
		boolean stopTimeDefined = true;
		double stopTime = 10;
		comp.setupExperiment(toleranceDefined, tolerance, startTime, stopTimeDefined, stopTime);
	}

	@Test
	public void testDoStep() throws FmuInvocationException
	{
		initModesTest();
		double currentCommunicationPoint = 0;
		double communicationStepSize = 0.1;
		boolean noSetFMUStatePriorToCurrentPoint = true;
		Assert.assertEquals(Fmi2Status.OK, comp.doStep(currentCommunicationPoint, communicationStepSize, noSetFMUStatePriorToCurrentPoint));
	}

	@Test
	public void testGetOutputDerivatives() throws FmuInvocationException
	{
		// initModesTest();
		testSetRealInputDerivatives();
		//
		double currentCommunicationPoint = 0;
		double communicationStepSize = 0.1;
		boolean noSetFMUStatePriorToCurrentPoint = true;
		;
		Assert.assertEquals(Fmi2Status.OK, comp.doStep(currentCommunicationPoint, communicationStepSize, noSetFMUStatePriorToCurrentPoint));
		//
		long[] scalarValueIndices = new long[] { TestUtils.REAL_ID };
		FmuResult<double[]> res = comp.getRealOutputDerivatives(scalarValueIndices, new int[]{1});
		// System.out.println(res.result);
		Assert.assertEquals(Fmi2Status.OK, res.status);
		Assert.assertEquals(1, res.result.length);
		Assert.assertTrue(Math.abs(99.12 - res.result[0]) <= 10);
	}

	@Test
	public void testSetRealInputDerivatives() throws FmuInvocationException
	{
		initModesTest();
		long[] scalarValueIndices = new long[] { TestUtils.REAL_ID };
		double[] derivatives = new double[] { 99.12 };
		Fmi2Status res = comp.setRealInputDerivatives(scalarValueIndices, new int[]{1}, derivatives);
		// System.out.println(res);
		Assert.assertEquals(Fmi2Status.OK, res);
	}

	@Test
	public void testGetDirectionalDerivative() throws FmuInvocationException
	{
		comp.enterInitializationMode();
		long[] vUnknownRef = new long[] { TestUtils.REAL_ID };
		long[] vKnownRef = new long[] { TestUtils.REAL_ID };
		double[] dvKnown = new double[] { 2 };
		FmuResult<double[]> res = comp.getDirectionalDerivative(vUnknownRef, vKnownRef, dvKnown);
		// System.out.println(res);
		Assert.assertEquals(Fmi2Status.OK, res.status);
		Assert.assertTrue(Math.abs(2 - res.result[0]) <= 10);
	}

	@Test
	public void testTerminate() throws FmuInvocationException
	{
		testDoStep();
		Assert.assertEquals(Fmi2Status.OK, comp.terminate());
	}

	@Test
	public void setGetReal() throws InvalidParameterException,
			FmuInvocationException
	{
		double value = 99.99;
		long[] scalarValueIndices = new long[] { TestUtils.REAL_ID };
		Assert.assertEquals(Fmi2Status.OK, comp.setReals(scalarValueIndices, new double[] { value }));

		Assert.assertEquals(Fmi2Status.OK, comp.enterInitializationMode());

		FmuResult<double[]> ret = comp.getReal(scalarValueIndices);
		Assert.assertEquals(Fmi2Status.OK, ret.status);
		Assert.assertEquals(value, ret.result[0], 0.1 / 10E307);
	}

	@Test
	public void setGetInteger() throws InvalidParameterException,
			FmuInvocationException
	{
		int value = 99;
		long[] scalarValueIndices = new long[] { TestUtils.INTEGER_ID };
		Assert.assertEquals(Fmi2Status.OK, comp.setIntegers(scalarValueIndices, new int[] { value }));

		Assert.assertEquals(Fmi2Status.OK, comp.enterInitializationMode());

		FmuResult<int[]> ret = comp.getInteger(scalarValueIndices);
		Assert.assertEquals(Fmi2Status.OK, ret.status);
		Assert.assertEquals(value, ret.result[0]);
	}

	@Test
	public void setGetBoolean() throws InvalidParameterException,
			FmuInvocationException
	{
		boolean value = true;
		long[] scalarValueIndices = new long[] { TestUtils.BOOLEAN_ID };
		Assert.assertEquals(Fmi2Status.OK, comp.setBooleans(scalarValueIndices, new boolean[] { value }));

		Assert.assertEquals(Fmi2Status.OK, comp.enterInitializationMode());

		FmuResult<boolean[]> ret = comp.getBooleans(scalarValueIndices);
		Assert.assertEquals(Fmi2Status.OK, ret.status);
		Assert.assertEquals(value, ret.result[0]);
	}

	@Test
	public void getState() throws FmuInvocationException,
			InvalidParameterException
	{
		FmuResult<IFmiComponentState> res = comp.getState();
		System.out.println(res.result);
		Assert.assertEquals(Fmi2Status.OK, res.status);

		// change the state and update reaal 0 = 99.99
		long[] scalarValueIndices = new long[] { 0 };

		double[] values = new double[] { 99.99 };
		comp.setReals(scalarValueIndices, values);

		// enable get
		Assert.assertEquals(Fmi2Status.OK, comp.enterInitializationMode());
		FmuResult<double[]> real = comp.getReal(scalarValueIndices);

		Assert.assertEquals(values[0], real.result[0], 0.001);
		// 0 = 99.99 confirmed

		FmuResult<IFmiComponentState> res2 = comp.getState();
		System.out.println(res2.result);
		Assert.assertEquals(Fmi2Status.OK, res2.status);

		// set state 0 = 0
		comp.setState(res.result);

		real = comp.getReal(scalarValueIndices);
		Assert.assertEquals(0, real.result[0], 0.001);

		// set state 0 = 99.99
		comp.setState(res2.result);

		real = comp.getReal(scalarValueIndices);
		Assert.assertEquals(values[0], real.result[0], 0.001);
		
		
		res.result.freeState();
		res2.result.freeState();

	}
}
