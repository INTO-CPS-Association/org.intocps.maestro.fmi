//
// Created by Kenneth Guldbrandt Lausdahl on 17/12/2020.
//
#include "org_intocps_fmi3_jnifmuapi_NativeFmuInstance3.h"
#include "Fmi3Manager.h"
#include "../shared/utilities.h"

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetDebugLogging
 * Signature: (JZI[Ljava/lang/String;)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetDebugLogging
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jboolean loggingOn, jobjectArray categories) {

    size_t size = env->GetArrayLength(categories);

    DECLARE_ARRAY(fmi2String, strings, size);
    copyJniArrayToNativeString(env, categories, strings, size);

    fmi3Status status = getFmuPtr(fmuPtr)->fmi3SetDebugLogging(
            getInstancePtr(instancePtr), loggingOn, size, strings);

    int i;

    for (i = 0; i < size; i++) {
        free((char *) strings[i]);
    }
    free(strings);
    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nFreeInstance
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nFreeInstance
        (JNIEnv *, jobject, jlong fmuPtr, jlong instancePtr) {
    fmi3Instance instance = getInstancePtr(instancePtr);
    getFmuPtr(fmuPtr)->fmi3FreeInstance(instance);
    Fmi3Manager::getInstance()->freeInstance(instance);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nEnterInitializationMode
 * Signature: (JZDDZD)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nEnterInitializationMode
        (JNIEnv *env, jobject, jlong fmuPtr, jlong instancePtr,
         jboolean toleranceDefined, jdouble tolerance, jdouble startTime,
         jboolean stopTimeDefined, jdouble stopTime) {
    fmi3Instance instance = getInstancePtr(instancePtr);
    return convertStatus(env, getFmuPtr(fmuPtr)->fmi3EnterInitializationMode(instance, toleranceDefined, tolerance, startTime, stopTimeDefined,
                                                          stopTime));
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nExitInitializationMode
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nExitInitializationMode
        (JNIEnv *env, jobject, jlong fmuPtr, jlong instancePtr) {
    fmi3Instance instance = getInstancePtr(instancePtr);
    return convertStatus(env, getFmuPtr(fmuPtr)->fmi3ExitInitializationMode(instance));
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nEnterEventMode
 * Signature: (JZ[IIZ)Lorg/intocps/fmi3/Fmi3Status;
 * fmi3Instance instance,
                                          fmi3Boolean stepEvent,
                                          const fmi3Int32 rootsFound[],
                                          size_t nEventIndicators,
                                          fmi3Boolean timeEvent
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nEnterEventMode
        (JNIEnv *env, jobject, jlong fmuPtr, jlong instancePtr, jboolean stepEvent, jintArray rootsFound, jint nEventIndicators, jboolean timeEvent) {
    fmi3Instance instance = getInstancePtr(instancePtr);
    fmi3Int32 rootsFound_[nEventIndicators];
    fmi3Status status = getFmuPtr(fmuPtr)->fmi3EnterEventMode(instance, stepEvent, rootsFound_, nEventIndicators, timeEvent);
    if (status == fmi3OK || status == fmi3Warning)
        copyArray_fmi3Int32_to_jint(env, rootsFound_, rootsFound, nEventIndicators);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nTerminate
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nTerminate
        (JNIEnv *, jobject, jlong) { return nullptr; }

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nReset
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nReset
        (JNIEnv *, jobject, jlong) { return nullptr; }


/**************************************************************
 *
 *          GET
 *
 **************************************************************/

#define GET_TEMPLATE(fmiType, api, jniType) JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGet##fmiType\
        (JNIEnv *env, jobject obj, jlong instancePtr, jlongArray valueReferences, jint nValueReferences,\
         jniType##Array values, jint nValues) {\
\
    Fmi3InstanceNode *instance = (Fmi3InstanceNode *) instancePtr;\
\
    jsize len = env->GetArrayLength(valueReferences);\
\
    fmi3ValueReference *vr_arr = createArray_uint_from_jlong(env, valueReferences, len);\
\
    fmi3 ## fmiType outputValues[len];\
\
    auto status = instance->owner->fmu.fmi3Get##fmiType(instance, vr_arr, len, outputValues,len);\
\
    if (status == fmi3OK || status == fmi3Warning) {\
        copyArray_##fmi3##fmiType##_to_##jniType(env, outputValues,values, len);\
    }\
\
    delete (vr_arr);\
\
    return convertStatus(env, status);\
}                                                                                                              \


GET_TEMPLATE(Float32, Float, jfloat)

GET_TEMPLATE(Float64, Double, jdouble)

GET_TEMPLATE(Int8, Int, jint)

GET_TEMPLATE(UInt8, Int, jint)

GET_TEMPLATE(Int16, Int, jint)

GET_TEMPLATE(UInt16, Int, jint)

GET_TEMPLATE(Int32, Int, jint)

GET_TEMPLATE(UInt32, Int, jint)

GET_TEMPLATE(Int64, Long, jlong)

GET_TEMPLATE(UInt64, Long, jlong)

GET_TEMPLATE(Boolean, Boolean, jboolean)
//GET_TEMPLATE(Binary,Long,jlong)


// The functions below are all implemented via the templated version above.

//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetFloat32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetFloat64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);

//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetInt8
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetUInt8
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetInt16
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetUInt16
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetInt32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetUInt32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetInt64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetUInt64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jlongArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetBoolean
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jbooleanArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetBinary
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jbooleanArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetString
 * Signature: (J[JI[Ljava/lang/String;I)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetString
        (JNIEnv *, jobject, jlong, jlongArray, jint, jobjectArray, jint);

/**************************************************************
 *
 *          SET
 *
 **************************************************************/

// The commented functions below are implemented via the templated function

//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetFloat32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetFloat64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetInt8
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetUInt8
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetInt16
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetUInt16
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetInt32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetUInt32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetInt64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetUInt64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jlongArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetBoolean
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jbooleanArray, jint);

JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetBinary
        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jbooleanArray, jint);


#define SET_TEMPLATE(fmiType, api, jniType) JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSet##fmiType\
(JNIEnv *env, jobject obj, jlong instancePtr, jlongArray valueReferences, jint nValueReferences,\
         jniType##Array values, jint nValues)\
{\
    Fmi3InstanceNode *instance = (Fmi3InstanceNode *) instancePtr;\
\
    jsize len = env->GetArrayLength( valueReferences);\
\
    fmi3ValueReference *vr_arr = createArray_uint_from_jlong(env, valueReferences, len);\
\
    fmi3##fmiType inputs[len]; \
    copyArray_##jniType##_to_##fmi3##fmiType(env, values,inputs, len);\
\
    auto status = instance->owner->fmu.fmi3Set##fmiType(instance, vr_arr, len, inputs,len);\
\
    delete vr_arr;\
\
    return convertStatus(env, status);\
}

SET_TEMPLATE(Float32, Float, jfloat)

SET_TEMPLATE(Float64, Double, jdouble)

SET_TEMPLATE(Int8, Int, jint)

SET_TEMPLATE(UInt8, Int, jint)

SET_TEMPLATE(Int16, Int, jint)

SET_TEMPLATE(UInt16, Int, jint)

SET_TEMPLATE(Int32, Int, jint)

SET_TEMPLATE(UInt32, Int, jint)

SET_TEMPLATE(Int64, Long, jlong)

SET_TEMPLATE(UInt64, Long, jlong)

SET_TEMPLATE(Boolean, Boolean, jboolean)


JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetString
        (JNIEnv *, jobject, jlong, jlongArray, jint, jobjectArray, jint);


/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetNumberOfVariableDependencies
 * Signature: (JJ[I)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetNumberOfVariableDependencies
        (JNIEnv *, jobject, jlong, jlong, jintArray);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetVariableDependencies
 * Signature: (JJ[I[J[I[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetVariableDependencies
        (JNIEnv *, jobject, jlong, jlong, jintArray, jlongArray, jintArray, jintArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetFMUState
 * Signature: (J[J)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetFMUState
        (JNIEnv *, jobject, jlong, jlongArray);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetFMUState
 * Signature: (JJ)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetFMUState
        (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nFreeFMUState
 * Signature: (JJ)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nFreeFMUState
        (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSerializedFMUStateSize
 * Signature: (JJ[I)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSerializedFMUStateSize
        (JNIEnv *, jobject, jlong, jlong, jintArray);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetDirectionalDerivative
 * Signature: (J[JI[JI[DI[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetDirectionalDerivative
        (JNIEnv *, jobject, jlong, jlongArray, jint, jlongArray, jint, jdoubleArray, jint, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetAdjointDerivative
 * Signature: (J[JI[JI[DI[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetAdjointDerivative
        (JNIEnv *, jobject, jlong, jlongArray, jint, jlongArray, jint, jdoubleArray, jint, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nEnterConfigurationMode
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nEnterConfigurationMode
        (JNIEnv *, jobject, jlong);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nExitConfigurationMode
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nExitConfigurationMode
        (JNIEnv *, jobject, jlong);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetClock
 * Signature: (J[JI[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetClock
        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetClock
 * Signature: (J[JI[I[ZI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetClock
        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jbooleanArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetIntervalDecimal
 * Signature: (J[JI[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetIntervalDecimal
        (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetIntervalFraction
 * Signature: (J[JI[J[JI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetIntervalFraction
        (JNIEnv *, jobject, jlong, jlongArray, jint, jlongArray, jlongArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetIntervalDecimal
 * Signature: (J[JI[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetIntervalDecimal
        (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetIntervalFraction
 * Signature: (J[JI[J[JI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetIntervalFraction
        (JNIEnv *, jobject, jlong, jlongArray, jint, jlongArray, jlongArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nUpdateDiscreteStates
 * Signature: (J[Z[Z[Z[Z[Z[D)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nUpdateDiscreteStates
        (JNIEnv *, jobject, jlong, jbooleanArray, jbooleanArray, jbooleanArray, jbooleanArray, jbooleanArray,
         jdoubleArray);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nEnterContinuousTimeMode
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nEnterContinuousTimeMode
        (JNIEnv *, jobject, jlong);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nCompletedIntegratorStep
 * Signature: (JZ[Z[Z)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nCompletedIntegratorStep
        (JNIEnv *, jobject, jlong, jboolean, jbooleanArray, jbooleanArray);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetTime
 * Signature: (JD)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetTime
        (JNIEnv *, jobject, jlong, jdouble);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetContinuousStates
 * Signature: (J[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetContinuousStates
        (JNIEnv *, jobject, jlong, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetDerivatives
 * Signature: (J[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetDerivatives
        (JNIEnv *, jobject, jlong, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetEventIndicators
 * Signature: (J[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetEventIndicators
        (JNIEnv *, jobject, jlong, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetContinuousStates
 * Signature: (J[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetContinuousStates
        (JNIEnv *, jobject, jlong, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetNominalsOfContinuousStates
 * Signature: (J[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetNominalsOfContinuousStates
        (JNIEnv *, jobject, jlong, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetNumberOfEventIndicators
 * Signature: (J[I)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetNumberOfEventIndicators
        (JNIEnv *, jobject, jlong, jintArray);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetNumberOfContinuousStates
 * Signature: (J[I)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetNumberOfContinuousStates
        (JNIEnv *, jobject, jlong, jintArray);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nEnterStepMode
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nEnterStepMode
        (JNIEnv *, jobject, jlong);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetOutputDerivatives
 * Signature: (J[JI[I[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetOutputDerivatives
        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nDoStep
 * Signature: (JDDZ[Z[Z[Z[Z[D)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nDoStep
        (JNIEnv *, jobject, jlong, jdouble, jdouble, jboolean, jbooleanArray, jbooleanArray, jbooleanArray,
         jbooleanArray, jdoubleArray);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nActivateModelPartition
 * Signature: (JJID)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nActivateModelPartition
        (JNIEnv *, jobject, jlong, jlong, jint, jdouble);