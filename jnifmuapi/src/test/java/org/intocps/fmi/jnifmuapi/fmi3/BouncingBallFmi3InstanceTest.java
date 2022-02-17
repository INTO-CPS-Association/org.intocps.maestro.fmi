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

package org.intocps.fmi.jnifmuapi.fmi3;

import org.intocps.fmi.FmiInvalidNativeStateException;
import org.intocps.fmi.FmuInvocationException;
import org.intocps.fmi.FmuMissingLibraryException;
import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import java.io.File;
import java.util.List;
import java.util.Vector;

public class BouncingBallFmi3InstanceTest {
    static final String FMU_UNPACKED_PATH = "target/fmus/fmi3/bounchingball".replace('/', File.separatorChar);
    static final String FMU_GUID = "{8c4e810f-3df3-4a00-8276-176fa3c9f003}";

    IFmi3Fmu fmu = null;
    IFmi3Instance instance = null;
    List<Object[]> logData = new Vector<>();

    @Before
    public void before() throws FmuInvocationException, FmuMissingLibraryException {
        fmu = new DirectoryFmi3Fmu(new File(FMU_UNPACKED_PATH), "bouncingball");
        fmu.load();
        ILogMessageCallback lm = (instanceName, status, category, message) -> {

            logData.add(new Object[]{instanceName, status, category, message});
            System.out.printf("Received log message:\n" + "instanceName: '%s'\n" + "Status: '%s'\n" + "Category: '%s'\n" + "Message: '%s'%n",
                    instanceName, status.name(), category, message);
        };
        instance = fmu.instantiateCoSimulation("Bouncing Ball", FMU_GUID, null, false, true, false, false, null, lm, null);
        Assert.assertNotNull("Instantiate returned null", instance);
    }

    @After
    public void after() throws FmiInvalidNativeStateException {
        if (instance != null) {
            instance.freeInstance();
        }
        if (fmu != null) {
            fmu.unLoad();
        }
    }


    @Test
    public void terminate() throws Exception {

        Fmi3Status status = instance.terminate();
        Assert.assertEquals("Terminate result status did not match", Fmi3Status.fmi3Error, status);
        Assert.assertEquals("One log entry expected", 1, logData.size());
        Object[] logEntry = logData.get(0);
        Assert.assertEquals("Log Instance name did not match", "Bouncing Ball", logEntry[0]);
        Assert.assertEquals("Log Fmi3Status did not match", Fmi3Status.fmi3Error, logEntry[1]);
        Assert.assertEquals("Log Category", "logStatusError", logEntry[2]);
        Assert.assertEquals("Log Message", "fmi3Terminate: Illegal call sequence.", logEntry[3]);

    }

    @Test
    public void reset() throws Exception {
        Fmi3Status status = instance.reset();
        Assert.assertEquals(Fmi3Status.fmi3OK, status);
    }

    @Test
    public void enterInitializationMode() throws Exception {
        Assert.assertEquals(Fmi3Status.fmi3OK, instance.enterInitializationMode(null, 0, null));
    }

    @Test
    public void exitInitializationMode() throws Exception {
        enterInitializationMode();
        Assert.assertEquals(Fmi3Status.fmi3OK, instance.exitInitializationMode());

    }

    @Test
    public void enterEventMode() throws Exception {
        exitInitializationMode();
        Assert.assertEquals(Fmi3Status.fmi3OK, instance.enterEventMode(false, false, null, false));
    }

    @Test
    public void enterConfigurationMode() throws Exception {
        Assert.assertEquals(Fmi3Status.fmi3OK, instance.enterConfigurationMode());
    }

    @Test
    public void exitConfigurationMode() throws Exception {
        enterConfigurationMode();
        Assert.assertEquals(Fmi3Status.fmi3OK, instance.exitConfigurationMode());
    }

    @Test
    public void doStep() throws Exception {
        exitInitializationMode();
        FmuResult<IFmi3Instance.DoStepResult> result = instance.doStep(0.0, 0.1, false);
        Assert.assertEquals(Fmi3Status.fmi3OK, result.status);

    }





    /* native Fmi3Status nGetFMUState(long fmu, long instance, long[] FMUState);

    native Fmi3Status nSetFMUState(long instance, long FMUState);

    native Fmi3Status nFreeFMUState(long instance, long FMUState);

    native Fmi3Status nSerializedFMUStateSize(long fmu, long instance, long FMUState, long[] size);*/





    /*
      native Fmi3Status nGetFloat32(long instance, long valueReferences[], int nValueReferences, double values[], int nValues);

    native Fmi3Status nGetFloat64(long instance, long valueReferences[], int nValueReferences, double values[], int nValues);

    native Fmi3Status nGetInt8(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nGetUInt8(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nGetInt16(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nGetUInt16(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nGetInt32(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nGetUInt32(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nGetInt64(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nGetUInt64(long instance, long valueReferences[], int nValueReferences, long values[], int nValues);

    native Fmi3Status nGetBoolean(long instance, long valueReferences[], int nValueReferences, boolean values[], int nValues);

    native Fmi3Status nGetString(long instance, long valueReferences[], int nValueReferences, String values[], int nValues);

    native Fmi3Status nGetBinary(long instance, long valueReferences[], int nValueReferences, long sizes[], byte values[], int nValues);

    native Fmi3Status nSetFloat32(long instance, long valueReferences[], int nValueReferences, double values[], int nValues);

    native Fmi3Status nSetFloat64(long instance, long valueReferences[], int nValueReferences, double values[], int nValues);

    native Fmi3Status nSetInt8(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nSetUInt8(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nSetInt16(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nSetUInt16(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nSetInt32(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nSetUInt32(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nSetInt64(long instance, long valueReferences[], int nValueReferences, int values[], int nValues);

    native Fmi3Status nSetUInt64(long instance, long valueReferences[], int nValueReferences, long values[], int nValues);

    native Fmi3Status nSetBoolean(long instance, long valueReferences[], int nValueReferences, boolean values[], int nValues);

    native Fmi3Status nSetString(long instance, long valueReferences[], int nValueReferences, String values[], int nValues);

    native Fmi3Status nSetBinary(long instance, long valueReferences[], int nValueReferences, long sizes[], byte array_name[][], int nValues);

    native Fmi3Status nGetNumberOfVariableDependencies(long instance, long valueReference, long[] nDependencies);

    native Fmi3Status nGetVariableDependencies(long fmu, long instance, long dependent, long elementIndicesOfDependent[], long independents[],
            long elementIndicesOfIndependents[], Fmi3DependencyKind dependencyKinds[], int nDependencies);



    native Fmi3Status nGetDirectionalDerivative(long fmu, long instance, long unknowns[], int nUnknowns, long knowns[], int nKnowns, double seed[],
            int nSeed, double sensitivity[], int nSensitivity);

    native Fmi3Status nGetAdjointDerivative(long fmu, long instance, long unknowns[], int nUnknowns, long knowns[], int nKnowns, double seed[],
            int nSeed, double sensitivity[], int nSensitivity);



    native Fmi3Status nGetClock(long fmu, long instance, long valueReferences[], int nValueReferences, boolean values[]);

    native Fmi3Status nSetClock(long fmu, long instance, long valueReferences[], int nValueReferences, boolean values[]);

    native Fmi3Status nGetIntervalDecimal(long fmu, long instance, long valueReferences[], int nValueReferences, double intervals[],
            Fmi3IntervalQualifier qualifiers[]);

    native Fmi3Status nGetIntervalFraction(long fmu, long instance, long valueReferences[], int nValueReferences, long intervalCounters[],
            long resolutions[]);

    native Fmi3Status nGetShiftDecimal(long fmu, long instance, long valueReferences[], int nValueReferences, double shifts[]);

    native Fmi3Status nGetShiftFraction(long fmu, long instance, int valueReferences[], int nValueReferences, double shiftCounters[],
            double resolutions[]);

    native Fmi3Status nSetIntervalDecimal(long fmu, long instance, long valueReferences[], int nValueReferences, double interval[]);

    native Fmi3Status nSetIntervalFraction(long fmu, long instance, long valueReferences[], int nValueReferences, long intervalCounter[],
            long resolution[]);

    native Fmi3Status nUpdateDiscreteStates(long fmu, long instance, boolean[] discreteStatesNeedUpdate, boolean[] terminateSimulation,
            boolean[] nominalsOfContinuousStatesChanged, boolean[] valuesOfContinuousStatesChanged, boolean[] nextEventTimeDefined,
            double[] nextEventTime);



     Types for Functions for Model Exchange



    native Fmi3Status nEnterContinuousTimeMode(long fmu, long instance);

    native Fmi3Status nCompletedIntegratorStep(long fmu, long instance, boolean noSetFMUStatePriorToCurrentPoint, boolean[] enterEventMode,
            boolean[] terminateSimulation);

    native Fmi3Status nSetTime(long fmu, long instance, double time);

    native Fmi3Status nSetContinuousStates(long fmu, long instance, double continuousStates[], int nContinuousStates);

    native Fmi3Status nGetDerivatives(long instance, double derivatives[], int nContinuousStates); //TODO: Is this a FMI3 method?

    native Fmi3Status nGetEventIndicators(long fmu, long instance, double eventIndicators[], int nEventIndicators);

    native Fmi3Status nGetContinuousStates(long fmu, long instance, double continuousStates[], int nContinuousStates);

    native Fmi3Status nGetNominalsOfContinuousStates(long fmu, long instance, double nominals[], int nContinuousStates);

    native Fmi3Status nGetNumberOfEventIndicators(long fmu, long instance, long[] nEventIndicators);

    native Fmi3Status nGetNumberOfContinuousStates(long fmu, long instance, long[] nContinuousStates);

     Types for Functions for Co-Simulation

    native Fmi3Status nEnterStepMode(long fmu, long instance);

    native Fmi3Status nGetOutputDerivatives(long fmu, long instance, long valueReferences[], int nValueReferences, int orders[], double values[],
            int nValues);

    native Fmi3Status nDoStep(long fmu, long instance, double currentCommunicationPoint, double communicationStepSize,
            boolean noSetFMUStatePriorToCurrentPoint, boolean[] eventEncountered, boolean[] terminate, boolean[] earlyReturn,
            double[] lastSuccessfulTime);

     Types for Functions for Scheduled Execution

    native Fmi3Status nActivateModelPartition(long fmu, long instance, long clockReference, double activationTime);
     */
}
