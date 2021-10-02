/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_intocps_fmi3_jnifmuapi_NativeFmuInstance3 */

#ifndef _Included_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
#define _Included_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetDebugLogging
 * Signature: (JJZ[Ljava/lang/String;)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetDebugLogging
  (JNIEnv *, jobject, jlong, jlong, jboolean, jobjectArray);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nFreeInstance
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nFreeInstance
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nEnterInitializationMode
 * Signature: (JJZDDZD)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nEnterInitializationMode
  (JNIEnv *, jobject, jlong, jlong, jboolean, jdouble, jdouble, jboolean, jdouble);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nExitInitializationMode
 * Signature: (JJ)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nExitInitializationMode
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nEnterEventMode
 * Signature: (JJZ[IIZ)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nEnterEventMode
  (JNIEnv *, jobject, jlong, jlong, jboolean, jintArray, jint, jboolean);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nTerminate
 * Signature: (JJ)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nTerminate
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nReset
 * Signature: (JJ)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nReset
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetFloat32
 * Signature: (J[JI[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetFloat32
  (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetFloat64
 * Signature: (J[JI[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetFloat64
  (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetInt8
 * Signature: (J[JI[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetInt8
  (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetUInt8
 * Signature: (J[JI[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetUInt8
  (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetInt16
 * Signature: (J[JI[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetInt16
  (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetUInt16
 * Signature: (J[JI[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetUInt16
  (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetInt32
 * Signature: (J[JI[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetInt32
  (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetUInt32
 * Signature: (J[JI[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetUInt32
  (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetInt64
 * Signature: (J[JI[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetInt64
  (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetUInt64
 * Signature: (J[JI[JI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetUInt64
  (JNIEnv *, jobject, jlong, jlongArray, jint, jlongArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetBoolean
 * Signature: (J[JI[ZI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetBoolean
  (JNIEnv *, jobject, jlong, jlongArray, jint, jbooleanArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetString
 * Signature: (J[JI[Ljava/lang/String;I)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetString
  (JNIEnv *, jobject, jlong, jlongArray, jint, jobjectArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetBinary
 * Signature: (J[JI[I[JI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetBinary
  (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jlongArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetFloat32
 * Signature: (J[JI[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetFloat32
  (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetFloat64
 * Signature: (J[JI[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetFloat64
  (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetInt8
 * Signature: (J[JI[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetInt8
  (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetUInt8
 * Signature: (J[JI[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetUInt8
  (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetInt16
 * Signature: (J[JI[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetInt16
  (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetUInt16
 * Signature: (J[JI[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetUInt16
  (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetInt32
 * Signature: (J[JI[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetInt32
  (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetUInt32
 * Signature: (J[JI[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetUInt32
  (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetInt64
 * Signature: (J[JI[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetInt64
  (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetUInt64
 * Signature: (J[JI[JI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetUInt64
  (JNIEnv *, jobject, jlong, jlongArray, jint, jlongArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetBoolean
 * Signature: (J[JI[ZI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetBoolean
  (JNIEnv *, jobject, jlong, jlongArray, jint, jbooleanArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetString
 * Signature: (J[JI[Ljava/lang/String;I)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetString
  (JNIEnv *, jobject, jlong, jlongArray, jint, jobjectArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetBinary
 * Signature: (J[JI[I[[BI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetBinary
  (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jobjectArray, jint);

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
 * Signature: (JJJ[I[J[I[Lorg/intocps/fmi3/Fmi3DependencyKind;I)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetVariableDependencies
  (JNIEnv *, jobject, jlong, jlong, jlong, jintArray, jlongArray, jintArray, jobjectArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetFMUState
 * Signature: (JJ[J)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetFMUState
  (JNIEnv *, jobject, jlong, jlong, jlongArray);

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
 * Signature: (JJJ[I)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSerializedFMUStateSize
  (JNIEnv *, jobject, jlong, jlong, jlong, jintArray);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetDirectionalDerivative
 * Signature: (JJ[JI[JI[DI[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetDirectionalDerivative
  (JNIEnv *, jobject, jlong, jlong, jlongArray, jint, jlongArray, jint, jdoubleArray, jint, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetAdjointDerivative
 * Signature: (JJ[JI[JI[DI[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetAdjointDerivative
  (JNIEnv *, jobject, jlong, jlong, jlongArray, jint, jlongArray, jint, jdoubleArray, jint, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nEnterConfigurationMode
 * Signature: (JJ)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nEnterConfigurationMode
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nExitConfigurationMode
 * Signature: (JJ)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nExitConfigurationMode
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetClock
 * Signature: (JJ[JI[ZI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetClock
  (JNIEnv *, jobject, jlong, jlong, jlongArray, jint, jbooleanArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetClock
 * Signature: (JJ[JI[ZI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetClock
  (JNIEnv *, jobject, jlong, jlong, jlongArray, jint, jbooleanArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetIntervalDecimal
 * Signature: (JJ[JI[D[Lorg/intocps/fmi3/Fmi3IntervalQualifier;I)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetIntervalDecimal
  (JNIEnv *, jobject, jlong, jlong, jlongArray, jint, jdoubleArray, jobjectArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetIntervalFraction
 * Signature: (JJ[JI[J[JI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetIntervalFraction
  (JNIEnv *, jobject, jlong, jlong, jlongArray, jint, jlongArray, jlongArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetIntervalDecimal
 * Signature: (JJ[JI[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetIntervalDecimal
  (JNIEnv *, jobject, jlong, jlong, jlongArray, jint, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetIntervalFraction
 * Signature: (JJ[JI[J[JI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetIntervalFraction
  (JNIEnv *, jobject, jlong, jlong, jlongArray, jint, jlongArray, jlongArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nUpdateDiscreteStates
 * Signature: (JJ[Z[Z[Z[Z[Z[D)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nUpdateDiscreteStates
  (JNIEnv *, jobject, jlong, jlong, jbooleanArray, jbooleanArray, jbooleanArray, jbooleanArray, jbooleanArray, jdoubleArray);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nEnterContinuousTimeMode
 * Signature: (JJ)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nEnterContinuousTimeMode
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nCompletedIntegratorStep
 * Signature: (JJZ[Z[Z)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nCompletedIntegratorStep
  (JNIEnv *, jobject, jlong, jlong, jboolean, jbooleanArray, jbooleanArray);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetTime
 * Signature: (JJD)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetTime
  (JNIEnv *, jobject, jlong, jlong, jdouble);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetContinuousStates
 * Signature: (JJ[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nSetContinuousStates
  (JNIEnv *, jobject, jlong, jlong, jdoubleArray, jint);

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
 * Signature: (JJ[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetEventIndicators
  (JNIEnv *, jobject, jlong, jlong, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetContinuousStates
 * Signature: (JJ[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetContinuousStates
  (JNIEnv *, jobject, jlong, jlong, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetNominalsOfContinuousStates
 * Signature: (JJ[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetNominalsOfContinuousStates
  (JNIEnv *, jobject, jlong, jlong, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetNumberOfEventIndicators
 * Signature: (JJ[I)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetNumberOfEventIndicators
  (JNIEnv *, jobject, jlong, jlong, jintArray);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetNumberOfContinuousStates
 * Signature: (JJ[I)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetNumberOfContinuousStates
  (JNIEnv *, jobject, jlong, jlong, jintArray);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nEnterStepMode
 * Signature: (JJ)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nEnterStepMode
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetOutputDerivatives
 * Signature: (JJ[JI[I[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nGetOutputDerivatives
  (JNIEnv *, jobject, jlong, jlong, jlongArray, jint, jintArray, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nDoStep
 * Signature: (JJDDZ[Z[Z[Z[D)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nDoStep
  (JNIEnv *, jobject, jlong, jlong, jdouble, jdouble, jboolean, jbooleanArray, jbooleanArray, jbooleanArray, jdoubleArray);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nActivateModelPartition
 * Signature: (JJJID)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmuInstance3_nActivateModelPartition
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jdouble);

#ifdef __cplusplus
}
#endif
#endif
