package org.intocps.fmi3.jnifmuapi;

import org.intocps.fmi3.Fmi3Status;

public class NativeFmu3 {

    /* Type definitions */

    /* end::Status[] */

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

    interface ICallbackLogMessage {
        void logMessage(long instanceEnvironment, String instanceName, Fmi3Status status, String category, String message);
    }

    interface ICallbackIntermediateUpdate {
        void intermediateUpdate(long instanceEnvironment, double intermediateUpdateTime, boolean clocksTicked,
                boolean intermediateVariableSetRequested, boolean intermediateVariableGetAllowed, boolean intermediateStepFinished,
                boolean canReturnEarly, boolean[] earlyReturnRequested, double[] earlyReturnTime);
    }

    interface ICallbackLockPreemption {
        void fmi3CallbackLockPreemption();
    }

    interface ICallbackUnlockPreemption {
        void fmi3CallbackUnlockPreemption();
    }




    /* Define fmi3 function pointer types to simplify dynamic loading */

    /***************************************************
     Types for Common Functions
     ****************************************************/

    /* Inquire version numbers and setting logging status */
    /* tag::GetVersion[] */
    native String nGetVersion(long fmuPtr);
    /* end::GetVersion[] */


    /* Creation and destruction of FMU instances and setting debug status */
    /* tag::Instantiate[] */
    native long nInstantiateModelExchange(long fmuPtr, String instanceName, String instantiationToken, String resourceLocation, boolean visible,
            boolean loggingOn, long instanceEnvironment, ICallbackLogMessage logMessage);

    native long nInstantiateCoSimulation(long fmuPtr, String instanceName, String instantiationToken, String resourceLocation, boolean visible,
            boolean loggingOn, boolean eventModeUsed, long requiredIntermediateVariables[], long nRequiredIntermediateVariables,
            long instanceEnvironment, ICallbackLogMessage logMessage, ICallbackIntermediateUpdate intermediateUpdate);

    native long n3InstantiateScheduledExecution(long fmuPtr, String instanceName, String instantiationToken, String resourceLocation, boolean visible,
            boolean loggingOn, long requiredIntermediateVariables[], long nRequiredIntermediateVariables, long instanceEnvironment,
            ICallbackLogMessage logMessage, ICallbackIntermediateUpdate intermediateUpdate, ICallbackLockPreemption lockPreemption,
            ICallbackUnlockPreemption unlockPreemption);
    /* end::Instantiate[] */


    /***************************************************
     * C Utils
     ****************************************************/
    protected synchronized native long nLoadLibrary(String libraryPath);

    protected synchronized native void nUnLoad(long ptr);

    public static native String getJniApiVersion();

}
