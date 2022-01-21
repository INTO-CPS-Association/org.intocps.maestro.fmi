package org.intocps.fmi.jnifmuapi.fmi3;


import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public abstract class NativeFmu3 {
    final static Logger logger = LoggerFactory.getLogger(NativeFmu3.class);

    /* Type definitions */



    /* tag::DependencyKind[] */
    //    typedef enum {
    //        /* fmi3Independent = 0, not needed but reserved for future use */
    //        fmi3Constant  = 1,
    //                fmi3Fixed     = 2,
    //                fmi3Tunable   = 3,
    //                fmi3Discrete  = 4,
    //                fmi3Dependent = 5
    //    } fmi3DependencyKind;
    /* end::DependencyKind[] */





    /* Define fmi3 function pointer types to simplify dynamic loading */

    /***************************************************
     Types for Common Functions
     ****************************************************/

    /* Inquire version numbers and setting logging status */
    /* tag::GetVersion[] */
    protected native String nGetVersion(long fmuPtr);
    /* end::GetVersion[] */


    /* Creation and destruction of FMU instances and setting debug status */
    /* tag::Instantiate[] */
    protected native long nInstantiateModelExchange(long fmuPtr, String instanceName, String instantiationToken, String resourceLocation,
            boolean visible, boolean loggingOn, long instanceEnvironment, ILogMessageCallback logMessage);

    @SuppressWarnings("CStyleArrayDeclaration")
    protected native long nInstantiateCoSimulation(long fmuPtr, String instanceName, String instantiationToken, String resourceLocation,
            boolean visible, boolean loggingOn, boolean eventModeUsed, boolean earlyReturnAllowed, long requiredIntermediateVariables[],
            long nRequiredIntermediateVariables, long instanceEnvironment, ILogMessageCallback logMessage,
            IIntermediateUpdateCallback intermediateUpdate);

    @SuppressWarnings("CStyleArrayDeclaration")
    protected native long nInstantiateScheduledExecution(long fmuPtr, String instanceName, String instantiationToken, String resourceLocation,
            boolean visible, boolean loggingOn, long instanceEnvironment, ILogMessageCallback logMessage, IClockUpdateCallback clockUpdate,
            ILockPreemptionCallback lockPreemption, IUnlockPreemptionCallback unlockPreemption);
    /* end::Instantiate[] */


    /***************************************************
     * C Utils
     ****************************************************/
    protected synchronized native long nLoadLibrary(String libraryPath);

    protected synchronized native void nUnLoad(long ptr);

    public static native String getJniApiVersion();

}
