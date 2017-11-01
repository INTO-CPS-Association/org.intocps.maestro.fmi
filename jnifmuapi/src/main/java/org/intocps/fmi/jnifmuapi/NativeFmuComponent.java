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

class NativeFmuComponent
{
	/* Define fmi2 function pointer types to simplify dynamic loading */

	/***************************************************
	 * Types for Common Functions
	 ****************************************************/
	/* Inquire version numbers of header files and setting logging status */

	// typedef fmi2Status fmi2SetDebugLoggingTYPE(fmi2Component, fmi2Boolean, size_t, const fmi2String[]);
	protected native byte nSetDebugLogging(long fmu, long component,
			boolean loggingOn, String[] categories);

	/* Creation and destruction of FMU instances and setting debug status */
	// typedef void fmi2FreeInstanceTYPE(fmi2Component);
	protected native void nFreeInstance(long fmu, long component);

	/* Enter and exit initialization mode, terminate and reset */
	// typedef fmi2Status fmi2SetupExperimentTYPE (fmi2Component, fmi2Boolean, fmi2Real, fmi2Real, fmi2Boolean,
	// fmi2Real);
	protected native byte nSetupExperiment(long fmu, long component,
			boolean toleranceDefined, double tolerance, double tStart,
			boolean fmi2True, double tEnd);

	// typedef fmi2Status fmi2EnterInitializationModeTYPE(fmi2Component);
	protected native byte nEnterInitializationMode(long fmu, long component);

	// typedef fmi2Status fmi2ExitInitializationModeTYPE (fmi2Component);
	protected native byte nExitInitializationMode(long fmu, long component);

	// typedef fmi2Status fmi2TerminateTYPE (fmi2Component);
	protected native byte nTerminate(long fmu, long component);

	// typedef fmi2Status fmi2ResetTYPE (fmi2Component);
	protected native byte nReset(long fmu, long component);

	/* Getting and setting variable values */
	// typedef fmi2Status fmi2GetRealTYPE (fmi2Component, const fmi2ValueReference[], size_t, fmi2Real []);
	protected native byte nGetReal(long fmu, long component, long[] vr,
			long size, double[] values);

	// typedef fmi2Status fmi2GetIntegerTYPE(fmi2Component, const fmi2ValueReference[], size_t, fmi2Integer[]);
	protected native byte nGetInteger(long fmu, long component, long[] vr,
			long size, int[] values);

	// typedef fmi2Status fmi2GetBooleanTYPE(fmi2Component, const fmi2ValueReference[], size_t, fmi2Boolean[]);
	protected native byte nGetBoolean(long fmu, long component, long[] vr,
			long size, boolean[] values);

	// typedef fmi2Status fmi2GetStringTYPE (fmi2Component, const fmi2ValueReference[], size_t, fmi2String []);
	protected native byte nGetString(long fmu, long component, long[] vr,
			long size, String[] values);

	// typedef fmi2Status fmi2SetRealTYPE (fmi2Component, const fmi2ValueReference[], size_t, const fmi2Real []);
	protected native byte nSetReal(long fmu, long component, long[] vr,
			long size, double[] values);

	// typedef fmi2Status fmi2SetIntegerTYPE(fmi2Component, const fmi2ValueReference[], size_t, const fmi2Integer[]);
	protected native byte nSetInteger(long fmu, long component, long[] vr,
			long size, int[] values);

	// typedef fmi2Status fmi2SetBooleanTYPE(fmi2Component, const fmi2ValueReference[], size_t, const fmi2Boolean[]);
	protected native byte nSetBoolean(long fmu, long component, long[] vr,
			long size, boolean[] values);

	// typedef fmi2Status fmi2SetStringTYPE (fmi2Component, const fmi2ValueReference[], size_t, const fmi2String []);
	protected native byte nSetString(long fmu, long component, long[] vr,
			long size, String[] values);

	/* Getting and setting the internal FMU state */
	// typedef fmi2Status fmi2GetFMUstateTYPE (fmi2Component, fmi2FMUstate*);
	protected native byte nGetFmuState(long fmu, long component, long[] statePtr);

	// typedef fmi2Status fmi2SetFMUstateTYPE (fmi2Component, fmi2FMUstate);
	protected native byte nSetFmuState(long fmu, long component, long statePtr);

	// typedef fmi2Status fmi2FreeFMUstateTYPE (fmi2Component, fmi2FMUstate*);
	protected native byte nFreeFmuState(long fmu, long component, long statePtr);

	// We will not support serialization, it is too slow anyway
	// typedef fmi2Status fmi2SerializedFMUstateSizeTYPE(fmi2Component, fmi2FMUstate, size_t*);
	// typedef fmi2Status fmi2SerializeFMUstateTYPE (fmi2Component, fmi2FMUstate, fmi2Byte[], size_t);
	// typedef fmi2Status fmi2DeSerializeFMUstateTYPE (fmi2Component, const fmi2Byte[], size_t, fmi2FMUstate*);

	/* Getting partial derivatives */
	// typedef fmi2Status fmi2GetDirectionalDerivativeTYPE(fmi2Component, const fmi2ValueReference[], size_t,
	// const fmi2ValueReference[], size_t,
	// const fmi2Real[], fmi2Real[]);
	protected native byte nGetDirectionalDerivative(long fmu, long component,
			long[] vUnknownRef, long nUnknownSize, long[] vKnownRef,
			long nKnownSize, double[] dvKnown, double[] dvUnknown);

	/***************************************************
	 * Types for Functions for FMI2 for Co-Simulation
	 ****************************************************/

	/* Simulating the slave */
	// typedef fmi2Status fmi2SetRealInputDerivativesTYPE (fmi2Component, const fmi2ValueReference [], size_t, const
	// fmi2Integer [], const fmi2Real []);
	protected native byte nSetRealInputDerivatives(long fmu, long component,
			long[] vr, long size, int[] order, double[] values);

	// typedef fmi2Status fmi2GetRealOutputDerivativesTYPE(fmi2Component, const fmi2ValueReference [], size_t, const
	// fmi2Integer [], fmi2Real []);
	protected native byte nGetRealOutputDerivatives(long fmu, long component,
			long[] vr, long size, int[] order, double[] values);

	// typedef fmi2Status fmi2DoStepTYPE (fmi2Component, fmi2Real, fmi2Real, fmi2Boolean);
	protected native byte nDoStep(long fmu, long component,
			double currentCommunicationPoint, double communicationStepSize,
			boolean noSetFMUStatePriorToCurrentPoint);

	// typedef fmi2Status fmi2CancelStepTYPE (fmi2Component);
	protected native byte nCancelStep(long fmu, long component);

	/* Inquire slave status */
	// status the last parameter array must always be called allocated with size 1 such that the value can be places at
	// index 0 by the c code

	// typedef fmi2Status fmi2GetStatusTYPE (fmi2Component, const fmi2StatusKind, fmi2Status* );
	protected native byte nGetStatus(long fmu, long component,
			byte fmi2StatusKind, byte[] fmi2StatusValue);

	// typedef fmi2Status fmi2GetRealStatusTYPE (fmi2Component, const fmi2StatusKind, fmi2Real* );
	protected native byte nGetRealStatus(long fmu, long component,
			byte fmi2StatusKind, double[] fmi2RealValue);

	// typedef fmi2Status fmi2GetIntegerStatusTYPE(fmi2Component, const fmi2StatusKind, fmi2Integer*);
	protected native byte nGetIntegerStatus(long fmu, long component,
			byte fmi2StatusKind, int[] value);

	// typedef fmi2Status fmi2GetBooleanStatusTYPE(fmi2Component, const fmi2StatusKind, fmi2Boolean*);
	protected native byte nGetBooleanStatus(long fmu, long component,
			byte fmi2StatusKind, boolean[] value);

	// typedef fmi2Status fmi2GetStringStatusTYPE (fmi2Component, const fmi2StatusKind, fmi2String* );
	protected native byte nGetStringStatus(long fmu, long component,
			byte fmi2StatusKind, String[] value);

	/*
	 * INTO CPS specific
	 */
	protected native byte nGetMaxStepsize(long fmu, long component,
			double[] size);
}
