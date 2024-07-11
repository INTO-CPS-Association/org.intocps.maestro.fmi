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

import java.lang.reflect.Array;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;

import org.intocps.fmi.Fmi2Status;
import org.intocps.fmi.Fmi2StatusKind;
import org.intocps.fmi.FmiInvalidNativeStateException;
import org.intocps.fmi.FmuInvocationException;
import org.intocps.fmi.FmuResult;
import org.intocps.fmi.IFmiComponent;
import org.intocps.fmi.IFmiComponentState;
import org.intocps.fmi.IFmu;
import org.intocps.fmi.InvalidParameterException;

public class FmuComponent extends NativeFmuComponent implements IFmiComponent
{

	final private long componentPtr;
	final private long fmuPtr;
	final private IFmu fmu;
	final private String guid;
	final private String name;

	public FmuComponent(DirectoryFmu fmu, long componentPtr, String guid,
			String name)
	{
		this.fmu = fmu;
		this.fmuPtr = fmu.getFmuPtr();
		this.componentPtr = componentPtr;
		this.guid = guid;
		this.name = name;
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#getFmu()
	 */
	@Override
	public IFmu getFmu()
	{
		return this.fmu;
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#setDebugLogging(boolean, java.lang.String[])
	 */
	@Override
	public Fmi2Status setDebugLogging(boolean loggingOn, String[] categories)
			throws FmiInvalidNativeStateException
	{
		checkState();
		return Fmi2Status.valueOf(nSetDebugLogging(fmuPtr, componentPtr, loggingOn, categories));
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#setupExperiment(boolean, double, double, boolean, double)
	 */
	@Override
	public Fmi2Status setupExperiment(boolean toleranceDefined,
			double tolerance, double startTime, boolean stopTimeDefined,
			double stopTime) throws FmiInvalidNativeStateException
	{
		checkState();
		return Fmi2Status.valueOf(nSetupExperiment(fmuPtr, componentPtr, toleranceDefined, tolerance, startTime, stopTimeDefined, stopTime));
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#enterInitializationMode()
	 */
	@Override
	public Fmi2Status enterInitializationMode()
			throws FmiInvalidNativeStateException
	{
		checkState();
		return Fmi2Status.valueOf(nEnterInitializationMode(fmuPtr, componentPtr));
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#exitInitializationMode()
	 */
	@Override
	public Fmi2Status exitInitializationMode()
			throws FmiInvalidNativeStateException
	{
		checkState();
		return Fmi2Status.valueOf(nExitInitializationMode(fmuPtr, componentPtr));
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#reset()
	 */
	@Override
	public Fmi2Status reset() throws FmiInvalidNativeStateException
	{
		checkState();
		return Fmi2Status.valueOf(nReset(fmuPtr, componentPtr));
	}

	public Fmi2Status setRealInputDerivatives(long[] scalarValueIndices,
			int[] order, double[] derivatives) throws FmuInvocationException
	{
		if (scalarValueIndices.length != derivatives.length)
		{
			throw new FmuInvocationException("Value count and derivative value count must match");
		}
		checkOrder(order);
		checkState();
		Fmi2Status status = Fmi2Status.valueOf(nSetRealInputDerivatives(fmuPtr, componentPtr, scalarValueIndices, scalarValueIndices.length, order, derivatives));
		return status;
	}

	private void checkOrder(int[] order) throws FmuInvocationException
	{
		if (order == null || order.length == 0)
		{
			throw new FmuInvocationException("Orders must not be null or empty");
		}

		for (int i : order)
		{
			if (i < 1)
			{
				throw new FmuInvocationException("Order must be 1 or greater: "
						+ i);
			}

		}
	}

	public FmuResult<double[]> getRealOutputDerivatives(
			long[] scalarValueIndices, int[] order)
			throws FmuInvocationException
	{
		checkOrder(order);

		double res[] = new double[scalarValueIndices.length];
		checkState();
		Fmi2Status status = Fmi2Status.valueOf(nGetRealOutputDerivatives(fmuPtr, componentPtr, scalarValueIndices, scalarValueIndices.length, order, res));
		return new FmuResult<double[]>(status, res);
	}

	public FmuResult<double[]> getDirectionalDerivative(long[] vUnknownRef,
			long[] vKnownRef, double[] dvKnown)
			throws FmiInvalidNativeStateException
	{
		long nUnknownSize = vUnknownRef.length;
		long nKnownSize = vKnownRef.length;
		double[] dvUnknown = new double[(int) nKnownSize];
		checkState();
		Fmi2Status status = Fmi2Status.valueOf(nGetDirectionalDerivative(fmuPtr, componentPtr, vUnknownRef, nUnknownSize, vKnownRef, nKnownSize, dvKnown, dvUnknown));
		return new FmuResult<double[]>(status, dvUnknown);
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#doStep(double, double, boolean)
	 */
	@Override
	public Fmi2Status doStep(double currentCommunicationPoint,
			double communicationStepSize,
			boolean noSetFMUStatePriorToCurrentPoint)
			throws FmiInvalidNativeStateException
	{
		checkState();
		return Fmi2Status.valueOf(nDoStep(fmuPtr, componentPtr, currentCommunicationPoint, communicationStepSize, noSetFMUStatePriorToCurrentPoint));
	}

	// /*
	// * (non-Javadoc)
	// * @see intocps.fmuapi.IFmuComponent#cancelStep()
	// */
	// @Override
	// public Fmi2Status cancelStep()
	// {
	// return Fmi2Status.valueOf(nCancelStep(fmuPtr, componentPtr));
	// }

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#getReal(long[])
	 */
	@Override
	public FmuResult<double[]> getReal(long[] scalarValueIndices)
			throws FmiInvalidNativeStateException
	{
		checkState();
		double res[] = new double[scalarValueIndices.length];
		Fmi2Status status = Fmi2Status.valueOf(nGetReal(fmuPtr, componentPtr, scalarValueIndices, scalarValueIndices.length, res));
		return new FmuResult<double[]>(status, res);
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#getInteger(long[])
	 */
	@Override
	public FmuResult<int[]> getInteger(long[] scalarValueIndices)
			throws FmiInvalidNativeStateException
	{
		checkState();
		int res[] = new int[scalarValueIndices.length];
		Fmi2Status status = Fmi2Status.valueOf(nGetInteger(fmuPtr, componentPtr, scalarValueIndices, scalarValueIndices.length, res));
		return new FmuResult<int[]>(status, res);
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#getBooleans(long[])
	 */
	@Override
	public FmuResult<boolean[]> getBooleans(long[] scalarValueIndices)
			throws FmiInvalidNativeStateException
	{
		checkState();
		boolean res[] = new boolean[scalarValueIndices.length];
		Fmi2Status status = Fmi2Status.valueOf(nGetBoolean(fmuPtr, componentPtr, scalarValueIndices, scalarValueIndices.length, res));
		return new FmuResult<boolean[]>(status, res);
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#getStrings(long[])
	 */
	@Override
	public FmuResult<String[]> getStrings(long[] scalarValueIndices)
			throws FmuInvocationException
	{
		checkState();
		String res[] = new String[scalarValueIndices.length];
		Fmi2Status status = Fmi2Status.valueOf(nGetString(fmuPtr, componentPtr, scalarValueIndices, scalarValueIndices.length, res));
		return new FmuResult<String[]>(status, res);
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#setBooleans(long[], boolean[])
	 */
	@Override
	public Fmi2Status setBooleans(long[] scalarValueIndices, boolean[] values)
			throws InvalidParameterException, FmiInvalidNativeStateException
	{
		if (scalarValueIndices.length != values.length)
		{
			throw new InvalidParameterException("Length of index and value array does not match");
		}
		checkState();
		return Fmi2Status.valueOf(nSetBoolean(fmuPtr, componentPtr, scalarValueIndices, scalarValueIndices.length, values));
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#setReals(long[], double[])
	 */
	@Override
	public Fmi2Status setReals(long[] scalarValueIndices, double[] values)
			throws InvalidParameterException, FmiInvalidNativeStateException
	{
		if (scalarValueIndices.length != values.length)
		{
			throw new InvalidParameterException("Length of index and value array does not match");
		}
		checkState();
		return Fmi2Status.valueOf(nSetReal(fmuPtr, componentPtr, scalarValueIndices, scalarValueIndices.length, values));
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#setIntegers(long[], int[])
	 */
	@Override
	public Fmi2Status setIntegers(long[] scalarValueIndices, int[] values)
			throws InvalidParameterException, FmiInvalidNativeStateException
	{
		if (scalarValueIndices.length != values.length)
		{
			throw new InvalidParameterException("Length of index and value array does not match");
		}
		checkState();
		return Fmi2Status.valueOf(nSetInteger(fmuPtr, componentPtr, scalarValueIndices, scalarValueIndices.length, values));
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#setStrings(long[], String[])
	 */
	@Override
	public Fmi2Status setStrings(long[] scalarValueIndices, String[] values)
			throws InvalidParameterException, FmiInvalidNativeStateException
	{
		if (scalarValueIndices.length != values.length)
		{
			throw new InvalidParameterException("Length of index and value array does not match");
		}
		checkState();
		return Fmi2Status.valueOf(nSetString(fmuPtr, componentPtr, scalarValueIndices, scalarValueIndices.length, values));
	}

	@SuppressWarnings({ "rawtypes", "unchecked" })
	public static String convertToString(Object arr)
	{
		List<Object> l = new Vector<Object>();

		if (arr.getClass().isArray())
		{
			int length = Array.getLength(arr);
			for (int i = 0; i < length; i++)
			{
				Object arrayElement = Array.get(arr, i);
				l.add(arrayElement);
			}
		}

		if (arr instanceof List)
		{
			l = (List<Object>) arr;
		}

		String s = "[ ";
		for (Iterator itr = l.iterator(); itr.hasNext();)
		{
			Object o = itr.next();
			s += o.toString();
			if (itr.hasNext())
			{
				s += ", ";
			}

		}
		return s + " ]";
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#getStatus(intocps.fmuapi.Fmi2StatusKind)
	 */
	@Override
	public FmuResult<Fmi2Status> getStatus(Fmi2StatusKind kind)
			throws FmiInvalidNativeStateException
	{
		checkState();
		byte b[] = new byte[1];
		Fmi2Status status = Fmi2Status.valueOf(nGetStatus(fmuPtr, componentPtr, kind.value, b));
		return new FmuResult<Fmi2Status>(status, Fmi2Status.valueOf(b[0]));
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#getBooleanStatus(intocps.fmuapi.Fmi2StatusKind)
	 */
	@Override
	public FmuResult<Boolean> getBooleanStatus(Fmi2StatusKind kind)
			throws FmiInvalidNativeStateException
	{
		checkState();
		boolean b[] = new boolean[1];
		Fmi2Status status = Fmi2Status.valueOf(nGetBooleanStatus(fmuPtr, componentPtr, kind.value, b));
		return new FmuResult<Boolean>(status, b[0]);
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#getIntegerStatus(intocps.fmuapi.Fmi2StatusKind)
	 */
	@Override
	public FmuResult<Integer> getIntegerStatus(Fmi2StatusKind kind)
			throws FmiInvalidNativeStateException
	{
		checkState();
		int b[] = new int[1];
		Fmi2Status status = Fmi2Status.valueOf(nGetIntegerStatus(fmuPtr, componentPtr, kind.value, b));
		return new FmuResult<Integer>(status, b[0]);
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#getRealStatus(intocps.fmuapi.Fmi2StatusKind)
	 */
	@Override
	public FmuResult<Double> getRealStatus(Fmi2StatusKind kind)
			throws FmiInvalidNativeStateException
	{
		checkState();
		double b[] = new double[1];
		Fmi2Status status = Fmi2Status.valueOf(nGetRealStatus(fmuPtr, componentPtr, kind.value, b));
		return new FmuResult<Double>(status, b[0]);
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#getStringStatus(intocps.fmuapi.Fmi2StatusKind)
	 */
	@Override
	public FmuResult<String> getStringStatus(Fmi2StatusKind kind)
			throws FmiInvalidNativeStateException
	{
		checkState();
		String b[] = new String[1];
		Fmi2Status status = Fmi2Status.valueOf(nGetStringStatus(fmuPtr, componentPtr, kind.value, b));
		return new FmuResult<String>(status, b[0]);
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#terminate()
	 */
	@Override
	public Fmi2Status terminate() throws FmiInvalidNativeStateException
	{
		checkState();
		return Fmi2Status.valueOf(nTerminate(fmuPtr, componentPtr));
	}

	/*
	 * (non-Javadoc)
	 * @see intocps.fmuapi.IFmuComponent#freeInstance()
	 */
	@Override
	public void freeInstance() throws FmiInvalidNativeStateException
	{
		checkState();
		if (this.fmu instanceof DirectoryFmu)
		{
			((DirectoryFmu) this.fmu).synchronizedFree(this);
		} else
		{
			internalFreeInstance();
		}
	}

	void internalFreeInstance() throws FmiInvalidNativeStateException
	{
		nFreeInstance(fmuPtr, componentPtr);
	}

	@Override
	public String toString()
	{
		return guid + "." + name;
	}

	@Override
	public boolean isValid()
	{
		return fmuPtr != 0 && componentPtr != 0;
	}

	private void checkState() throws FmiInvalidNativeStateException
	{
		if (!isValid())
		{
			throw new FmiInvalidNativeStateException("The internal "
					+ (fmuPtr != 0 ? "" : " FMU state is invalid. ")
					+ (componentPtr != 0 ? "" : " Component state is invalid"));
		}
	}

	@Override
	public FmuResult<IFmiComponentState> getState()
			throws FmuInvocationException
	{
		long statePtr[] = new long[1];
		Fmi2Status res = Fmi2Status.valueOf(nGetFmuState(fmuPtr, componentPtr, statePtr));

		if (res == Fmi2Status.OK)
		{
			return new FmuResult<IFmiComponentState>(res, new FmuComponentState(this, statePtr[0]));
		}

		return new FmuResult<IFmiComponentState>(res, null);

	}

	@Override
	public Fmi2Status setState(IFmiComponentState state)
			throws FmuInvocationException
	{
		if (state != null && state instanceof FmuComponentState)
		{
			FmuComponentState st = (FmuComponentState) state;
			if (st.comp == this && st.allocated)
			{
				Fmi2Status res = Fmi2Status.valueOf(nSetFmuState(fmuPtr, componentPtr, st.ptr));

				return res;
			}
		}

		throw new FmuInvocationException("Invalid state");
	}

	@Override
	public Fmi2Status freeState(IFmiComponentState state)
			throws FmuInvocationException
	{
		if (state != null && state instanceof FmuComponentState)
		{
			FmuComponentState st = (FmuComponentState) state;
			if (st.comp == this && st.allocated)
			{
				Fmi2Status res = Fmi2Status.valueOf(nFreeFmuState(fmuPtr, componentPtr, st.ptr));

				if (res == Fmi2Status.OK)
				{
					st.allocated = false;// Unallocated making it unusable
				}
				return res;
			}
		}

		throw new FmuInvocationException("Invalid state");
	}

	public FmuResult<Long> getSerializedFMUstateSize(IFmiComponentState state) throws FmuInvocationException{
		if (state != null && state instanceof FmuComponentState)
		{
			FmuComponentState st = (FmuComponentState) state;
			if (st.comp == this && st.allocated)
			{
				long[] byteSize= new long[1];
				Fmi2Status res = Fmi2Status.valueOf(nSerializedFMUstateSize(fmuPtr, componentPtr, st.ptr,byteSize));

				return new FmuResult<>(res,byteSize[0]);
			}
		}

		throw new FmuInvocationException("Invalid state");

	}
	public	FmuResult<byte[]> serializeFMUstate(IFmiComponentState state, long byteSize) throws FmuInvocationException{
		if (state != null && state instanceof FmuComponentState)
		{
			FmuComponentState st = (FmuComponentState) state;
			if (st.comp == this && st.allocated && byteSize>=0)
			{
				byte[] bytes= new byte[(int) byteSize];
				Fmi2Status res = Fmi2Status.valueOf(nSerializeFMUstate(fmuPtr, componentPtr, st.ptr,bytes,byteSize));

				return new FmuResult<>(res,bytes);
			}
		}

		throw new FmuInvocationException("Invalid state");

	}
	public	FmuResult<IFmiComponentState> serializeFMUstate(IFmiComponentState state, byte[] bytes, long size) throws FmuInvocationException{
		long statePtr[] = new long[1];
		Fmi2Status res = Fmi2Status.valueOf(nDeSerializeFMUstate(fmuPtr, componentPtr, bytes,size,statePtr));

		if (res == Fmi2Status.OK)
		{
			return new FmuResult<IFmiComponentState>(res, new FmuComponentState(this, statePtr[0]));
		}

		return new FmuResult<IFmiComponentState>(res, null);
	}

	@Override
	public FmuResult<Double> getMaxStepSize()
			throws FmiInvalidNativeStateException
	{
		checkState();
		double res[] = new double[1];
		Fmi2Status status = Fmi2Status.valueOf(nGetMaxStepsize(fmuPtr, componentPtr, res));
		return new FmuResult<Double>(status, status == Fmi2Status.Error ? Double.MAX_VALUE
				: res[0]);
	}

}
