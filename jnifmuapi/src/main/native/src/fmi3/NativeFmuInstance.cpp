//
// Created by Kenneth Guldbrandt Lausdahl on 17/12/2020.
//
#include "org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance.h"
#include "Fmi3Manager.h"
#include "../shared/utilities.h"

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetDebugLogging
 * Signature: (JZI[Ljava/lang/String;)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetDebugLogging
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jboolean loggingOn, jobjectArray categories) {

    size_t size = env->GetArrayLength(categories);

    DECLARE_ARRAY(fmi3String, strings, size);
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
JNIEXPORT void JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nFreeInstance
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
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nEnterInitializationMode
        (JNIEnv *env, jobject, jlong fmuPtr, jlong instancePtr,
         jboolean toleranceDefined, jdouble tolerance, jdouble startTime,
         jboolean stopTimeDefined, jdouble stopTime) {
    fmi3Instance instance = getInstancePtr(instancePtr);
    return convertStatus(env, getFmuPtr(fmuPtr)->fmi3EnterInitializationMode(instance, toleranceDefined, tolerance,
                                                                             startTime, stopTimeDefined,
                                                                             stopTime));
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nExitInitializationMode
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nExitInitializationMode
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
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nEnterEventMode
        (JNIEnv *env, jobject, jlong fmuPtr, jlong instancePtr, jboolean stepEvent, jboolean stateEvent, jintArray rootsFound,
         jint nEventIndicators, jboolean timeEvent) {
    fmi3Instance instance = getInstancePtr(instancePtr);
    fmi3Int32 rootsFound_[nEventIndicators];
    fmi3Status status = getFmuPtr(fmuPtr)->fmi3EnterEventMode(instance, stepEvent, stateEvent, rootsFound_, nEventIndicators,
                                                              timeEvent);
    if (status == fmi3OK || status == fmi3Warning)
        copyArray_fmi3Int32_to_jint(env, rootsFound_, rootsFound, nEventIndicators);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nTerminate
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nTerminate
        (JNIEnv *env, jobject, jlong fmuPtr, jlong instancePtr) {
    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmu = getFmuPtr(fmuPtr);
    return convertStatus(env, fmu->fmi3Terminate(instancePtr_));
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nReset
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nReset
        (JNIEnv *env, jobject, jlong fmuPtr, jlong instancePtr) {
    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmu = getFmuPtr(fmuPtr);
    return convertStatus(env, fmu->fmi3Reset(instancePtr_));
}


/**************************************************************
 *
 *          GET
 *
 **************************************************************/

#define GET_TEMPLATE(fmiType, api, jniType) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGet##fmiType\
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

// The functions below are all implemented via the templated version above.

//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetFloat32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetFloat64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);

//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetInt8
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetUInt8
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetInt16
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetUInt16
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetInt32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetUInt32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetInt64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetUInt64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jlongArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetBoolean
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jbooleanArray, jint);
//
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetBinary
        (JNIEnv *env, jobject obj, jlong instanceNodePtr, jlongArray valueReferences, jint nValueReferences,
         jlongArray valueSizes, jobjectArray values, jint nValues) {

    Fmi3InstanceNode *instanceNode = (Fmi3InstanceNode *) instanceNodePtr;

    DECLARE_ARRAY(fmi3Binary, nativeByteValues, nValues);
    fmi3ValueReference *nativeValueReferences = createArray_uint_from_jlong(env, valueReferences, nValueReferences);
    DECLARE_ARRAY(size_t, nativeValueSizes, nValues);

    fmi3Status status = instanceNode->owner->fmu.fmi3GetBinary(instanceNode, nativeValueReferences, nValueReferences,
                                                               nativeValueSizes, nativeByteValues, nValues);

    jlong *valueSizesElems = env->GetLongArrayElements(valueSizes, JNI_FALSE);
    for (int i = 0; i < nValues; i++) {
        const jbyte *bytesPtr = (jbyte *) nativeByteValues[i];
        jbyteArray row = (jbyteArray) env->GetObjectArrayElement(values, i);
        env->SetByteArrayRegion(row, 0, nativeValueSizes[i], bytesPtr);
        env->DeleteLocalRef(row);

        valueSizesElems[i] = (jlong) nativeValueSizes[i];
    }
    env->ReleaseLongArrayElements(valueSizes, valueSizesElems, 0);

    free(nativeValueReferences);
    free(nativeValueSizes);
    free(nativeByteValues);
    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetString
 * Signature: (J[JI[Ljava/lang/String;I)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetString
        (JNIEnv *env, jobject obj, jlong instanceNodePtr, jlongArray valueReferences, jint nValueReferences,
         jobjectArray values, jint nValues) {

    Fmi3InstanceNode *instanceNode = (Fmi3InstanceNode *) instanceNodePtr;

    DECLARE_ARRAY(fmi3String, nativeStrings, nValues);
    fmi3ValueReference *nativeValueReferences = createArray_uint_from_jlong(env, valueReferences, nValueReferences);

    fmi3Status status = instanceNode->owner->fmu.fmi3SetString(instanceNode, nativeValueReferences, nValueReferences,
                                                               nativeStrings, nValues);

    for (int i = 0; i < nValues; i++) {
        jstring jstrBuf = env->NewStringUTF(nativeStrings[i]);
        env->SetObjectArrayElement(values, i, jstrBuf);
        env->DeleteLocalRef(jstrBuf);
    }

    free(nativeStrings);
    free(nativeValueReferences);
    return convertStatus(env, status);
}

/**************************************************************
 *
 *          SET
 *
 **************************************************************/

// The commented functions below are implemented via the templated function

//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetFloat32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetFloat64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetInt8
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetUInt8
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetInt16
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetUInt16
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetInt32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetUInt32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetInt64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetUInt64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jlongArray, jint);
//
//JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetBoolean
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jbooleanArray, jint);

#define SET_TEMPLATE(fmiType, api, jniType) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSet##fmiType\
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

JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetBinary
        (JNIEnv *env, jobject obj, jlong instanceNodePtr, jlongArray valueReferences, jint nValueReferences,
         jlongArray valueSizes, jobjectArray values, jint nValues) {

    Fmi3InstanceNode *instanceNode = (Fmi3InstanceNode *) instanceNodePtr;

    fmi3Binary nativeByteValues[nValues];
    std::map<jbyteArray, jbyte *> reservedByteValues;
    fmi3ValueReference *nativeValueReferences = createArray_uint_from_jlong(env, valueReferences, nValueReferences);

    DECLARE_ARRAY(size_t, nativeValueSizes, nValues);

    auto elems = env->GetLongArrayElements(valueSizes, JNI_FALSE);
    for (int i = 0; i < nValues; i++) {
        nativeValueSizes[i] = elems[i];
    }
    env->ReleaseLongArrayElements(valueSizes, elems, 0);

    for (int i = 0; i < nValues; i++) {
        jbyteArray row = (jbyteArray) env->GetObjectArrayElement(values, i);
        jbyte *bytesPtr = env->GetByteArrayElements(row, JNI_FALSE);
        reservedByteValues.insert(pair<jbyteArray, jbyte *>(row, bytesPtr));
        nativeByteValues[i] = (fmi3Binary) bytesPtr;
        env->DeleteLocalRef(row);
    }

    fmi3Status status = instanceNode->owner->fmu.fmi3SetBinary(instanceNode, nativeValueReferences,
                                                               nValueReferences, nativeValueSizes, nativeByteValues,
                                                               nValues);

    for (auto const &entry: reservedByteValues) {
        env->ReleaseByteArrayElements(entry.first, entry.second, 0);
    }

    free(nativeValueReferences);
    free(nativeValueSizes);
    return convertStatus(env, status);
}


JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetString
        (JNIEnv *env, jobject obj, jlong instanceNodePtr, jlongArray valueReferences, jint nValueReferences,
         jobjectArray values, jint nValues) {

    Fmi3InstanceNode *instanceNode = (Fmi3InstanceNode *) instanceNodePtr;

    fmi3String nativeStrings[nValues];
    jstring reservedStrings[nValues];
    fmi3ValueReference *nativeValueReferences = createArray_uint_from_jlong(env, valueReferences, nValueReferences);

    for (int i = 0; i < nValues; i++) {
        jstring value = (jstring) (env->GetObjectArrayElement(values, i));
        reservedStrings[i] = value;
        nativeStrings[i] = env->GetStringUTFChars(value, JNI_FALSE);
        env->DeleteLocalRef(value);
    }

    fmi3Status status = instanceNode->owner->fmu.fmi3SetString(instanceNode, nativeValueReferences, nValueReferences,
                                                               nativeStrings, nValues);

    for (int i = 0; i < nValues; i++) {
        env->ReleaseStringUTFChars(reservedStrings[i], nativeStrings[i]);
    }

    free(nativeValueReferences);
    return convertStatus(env, status);
}


/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetNumberOfVariableDependencies
 * Signature: (JJ[I)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetNumberOfVariableDependencies
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlong valueReference,
         jlongArray nDependenciesArrPtr) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3ValueReference nativeValueReference = (fmi3ValueReference) valueReference;
    size_t nDependencies;
    fmi3Status status = fmuPtr_->fmi3GetNumberOfVariableDependencies(instancePtr_, nativeValueReference,
                                                                     &nDependencies);

    jlong *nDependenciesElem = env->GetLongArrayElements(nDependenciesArrPtr, JNI_FALSE);
    nDependenciesElem[0] = (jlong) nDependencies;
    env->ReleaseLongArrayElements(nDependenciesArrPtr, nDependenciesElem, 0);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetVariableDependencies
 * Signature: (JJ[I[J[I[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetVariableDependencies
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlong dependent,
         jlongArray elementIndicesOfDependent, jlongArray independents, jlongArray elementIndicesOfIndependents,
         jobjectArray dependencyKinds, jint nDependencies) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3ValueReference nativeDependent = (fmi3ValueReference) dependent;
    DECLARE_ARRAY(fmi3ValueReference, nativeIndependents, nDependencies);
    DECLARE_ARRAY(size_t, nativeElementIndicesOfDependent, nDependencies);
    DECLARE_ARRAY(size_t, nativeElementIndicesOfIndependents, nDependencies);
    DECLARE_ARRAY(fmi3DependencyKind, nativeDependencyKinds, nDependencies);

    fmi3Status status = fmuPtr_->fmi3GetVariableDependencies(instancePtr_, nativeDependent,
                                                             nativeElementIndicesOfDependent, nativeIndependents,
                                                             nativeElementIndicesOfIndependents, nativeDependencyKinds,
                                                             nDependencies);
    copyArray_fmi3DependencyKind_to_javaEnum(env, nativeDependencyKinds, dependencyKinds, nDependencies);

    jlong *independentsElems = env->GetLongArrayElements(independents, JNI_FALSE);
    jlong *elementIndicesOfDependentElems = env->GetLongArrayElements(elementIndicesOfDependent, JNI_FALSE);
    jlong *elementIndicesOfIndependentsElems = env->GetLongArrayElements(elementIndicesOfIndependents, JNI_FALSE);
    for (int i = 0; i < nDependencies; i++) {
        independentsElems[i] = (jlong) nativeIndependents[i];
        elementIndicesOfDependentElems[i] = (jlong) nativeElementIndicesOfDependent[i];
        elementIndicesOfIndependentsElems[i] = (jlong) nativeElementIndicesOfIndependents[i];
    }
    env->ReleaseLongArrayElements(independents, independentsElems, 0);
    env->ReleaseLongArrayElements(elementIndicesOfDependent, elementIndicesOfDependentElems, 0);
    env->ReleaseLongArrayElements(elementIndicesOfIndependents, elementIndicesOfIndependentsElems, 0);

    free(nativeDependencyKinds);
    free(nativeIndependents);
    free(nativeElementIndicesOfDependent);
    free(nativeElementIndicesOfIndependents);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetFMUState
 * Signature: (J[J)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetFMUState
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlongArray statePtrArr) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3FMUState state = nullptr;
    fmi3Status status = fmuPtr_->fmi3GetFMUState(instancePtr_, &state);

    jlong *vbody = env->GetLongArrayElements(statePtrArr, JNI_FALSE);
    vbody[0] = (jlong) (state);

    env->ReleaseLongArrayElements(statePtrArr, vbody, 0);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetFMUState
 * Signature: (JJ)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetFMUState
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlong statePtr) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3FMUState state = ((fmi3FMUState) statePtr);
    fmi3Status status = fmuPtr_->fmi3SetFMUState(instancePtr_, state);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nFreeFMUState
 * Signature: (JJ)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nFreeFMUState
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlong statePtr) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3FMUState state = ((fmi3FMUState) statePtr);
    fmi3Status status = fmuPtr_->fmi3FreeFMUState(instancePtr_, &state);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSerializedFMUStateSize
 * Signature: (JJ[I)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSerializedFMUStateSize
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlong statePtr, jlongArray sizeArrPtr) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3FMUState state = ((fmi3FMUState) statePtr);

    size_t size;
    fmi3Status status = fmuPtr_->fmi3SerializedFMUStateSize(instancePtr_, state, &size);

    jlong *sizeElem = env->GetLongArrayElements(sizeArrPtr, JNI_FALSE);
    sizeElem[0] = (jlong) size;
    env->ReleaseLongArrayElements(sizeArrPtr, sizeElem, 0);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetDirectionalDerivative
 * Signature: (J[JI[JI[DI[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetDirectionalDerivative
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlongArray unknowns, jint nUnknows,
         jlongArray knowns, jint nKnowns, jdoubleArray seed, jint nSeed, jdoubleArray sensitivity, jint nSensitivity) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3ValueReference *unknowns_refs = createArray_uint_from_jlong(env, unknowns, nUnknows);
    fmi3ValueReference *knowns_refs = createArray_uint_from_jlong(env, knowns, nKnowns);

    DECLARE_ARRAY(fmi3Float64, nativeSeed, nSeed);
    copyArray_jdouble_to_fmi3Float64(env, seed, nativeSeed, nSeed);

    DECLARE_ARRAY(fmi3Float64, nativeSensitivity, nSensitivity);

    fmi3Status status = fmuPtr_->fmi3GetDirectionalDerivative(
            instancePtr_, unknowns_refs, nUnknows, knowns_refs, nKnowns, nativeSeed, nSeed, nativeSensitivity,
            nSensitivity);

    copyArray_fmi3Float64_to_jdouble(env, nativeSensitivity, sensitivity, nSensitivity);

    free(unknowns_refs);
    free(knowns_refs);
    free(nativeSeed);
    free(nativeSensitivity);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetAdjointDerivative
 * Signature: (J[JI[JI[DI[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetAdjointDerivative
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlongArray unknowns, jint nUnknows,
         jlongArray knowns, jint nKnowns, jdoubleArray seed, jint nSeed, jdoubleArray sensitivity, jint nSensitivity) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3ValueReference *unknowns_refs = createArray_uint_from_jlong(env, unknowns, nUnknows);
    fmi3ValueReference *knowns_refs = createArray_uint_from_jlong(env, knowns, nKnowns);

    DECLARE_ARRAY(fmi3Float64, nativeSeed, nSeed);
    copyArray_jdouble_to_fmi3Float64(env, seed, nativeSeed, nSeed);

    DECLARE_ARRAY(fmi3Float64, nativeSensitivity, nSensitivity);

    fmi3Status status = fmuPtr_->fmi3GetAdjointDerivative(
            instancePtr_, unknowns_refs, nUnknows, knowns_refs, nKnowns, nativeSeed, nSeed, nativeSensitivity,
            nSensitivity);

    copyArray_fmi3Float64_to_jdouble(env, nativeSensitivity, sensitivity, nSensitivity);

    free(unknowns_refs);
    free(knowns_refs);
    free(nativeSeed);
    free(nativeSensitivity);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nEnterConfigurationMode
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nEnterConfigurationMode
        (JNIEnv *env, jobject obj, jlong instancePtr, jlong fmuPtr) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3Status status = fmuPtr_->fmi3EnterConfigurationMode(instancePtr_);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nExitConfigurationMode
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nExitConfigurationMode
        (JNIEnv *env, jobject obj, jlong instancePtr, jlong fmuPtr) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3Status status = fmuPtr_->fmi3ExitConfigurationMode(instancePtr_);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetClock
 * Signature: (J[JI[II)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetClock
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlongArray valueReferences, jint nValueReferences,
         jbooleanArray values, jint nValues) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3ValueReference *value_refs = createArray_uint_from_jlong(env, valueReferences, nValueReferences);
    DECLARE_ARRAY(fmi3Clock, nativeClocks, nValues);

    fmi3Status status = fmuPtr_->fmi3GetClock(instancePtr_, value_refs, nValueReferences, nativeClocks, nValues);

    copyArray_fmi3Clock_to_jboolean(env, nativeClocks, values, nValues);

    free(value_refs);
    free(nativeClocks);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetClock
 * Signature: (J[JI[I[ZI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetClock
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlongArray valueReferences, jint nValueReferences,
         jbooleanArray values, jint nValues) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3ValueReference *value_refs = createArray_uint_from_jlong(env, valueReferences, nValueReferences);
    DECLARE_ARRAY(fmi3Clock, nativeClocks, nValues);
    copyArray_jboolean_to_fmi3Clock(env, values, nativeClocks, nValues);

    fmi3Status status = fmuPtr_->fmi3GetClock(instancePtr_, value_refs, nValueReferences, nativeClocks, nValues);

    free(value_refs);
    free(nativeClocks);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetIntervalDecimal
 * Signature: (J[JI[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetIntervalDecimal
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlongArray valueReferences, jint nValueReferences,
         jdoubleArray intervals, jobjectArray qualifiers, jint nIntervals) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);
    fmi3ValueReference *value_refs = createArray_uint_from_jlong(env, valueReferences, nValueReferences);

    DECLARE_ARRAY(fmi3IntervalQualifier, nativeQualifiers, nIntervals);
    DECLARE_ARRAY(fmi3Float64, nativeIntervals, nIntervals);

    fmi3Status status = fmuPtr_->fmi3GetIntervalDecimal(instancePtr_, value_refs, nValueReferences, nativeIntervals,
                                                        nativeQualifiers, nIntervals);
    copyArray_fmi3IntervalQualifiers_to_javaEnum(env, nativeQualifiers, qualifiers, nIntervals);
    copyArray_fmi3Float64_to_jdouble(env, nativeIntervals, intervals, nIntervals);

    free(value_refs);
    free(nativeQualifiers);
    free(nativeIntervals);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetIntervalFraction
 * Signature: (J[JI[J[JI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetIntervalFraction
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlongArray valueReferences, jint nValueReferences,
         jlongArray intervalCounters, jlongArray resolutions, jobjectArray qualifiers, jint nIntervals) {

    //intervalCounters length == resolutions length == nIntervals
    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);
    fmi3ValueReference *value_refs = createArray_uint_from_jlong(env, valueReferences, nValueReferences);

    DECLARE_ARRAY(fmi3IntervalQualifier, nativeQualifiers, nIntervals);
    DECLARE_ARRAY(fmi3UInt64, nativeIntervalCounters, nIntervals);
    DECLARE_ARRAY(fmi3UInt64, nativeResolutions, nIntervals);


    fmi3Status status = fmuPtr_->fmi3GetIntervalFraction(instancePtr_, value_refs, nValueReferences,
                                                         nativeIntervalCounters, nativeResolutions, nativeQualifiers,
                                                         nIntervals);
    copyArray_fmi3IntervalQualifiers_to_javaEnum(env, nativeQualifiers, qualifiers, nIntervals);
    copyArray_fmi3UInt64_to_jlong(env, nativeIntervalCounters, intervalCounters, nIntervals);
    copyArray_fmi3UInt64_to_jlong(env, nativeResolutions, resolutions, nIntervals);

    free(value_refs);
    free(nativeQualifiers);
    free(nativeIntervalCounters);
    free(nativeResolutions);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetIntervalDecimal
 * Signature: (J[JI[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetIntervalDecimal
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlongArray valueReferences, jint nValueReferences,
         jdoubleArray intervals, jint nIntervals) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3ValueReference *value_refs = createArray_uint_from_jlong(env, valueReferences, nValueReferences);
    DECLARE_ARRAY(fmi3Float64, nativeIntervals, nIntervals);
    copyArray_jdouble_to_fmi3Float64(env, intervals, nativeIntervals, nIntervals);

    fmi3Status status = fmuPtr_->fmi3SetIntervalDecimal(instancePtr_, value_refs, nValueReferences, nativeIntervals,
                                                        nIntervals);

    free(value_refs);
    free(nativeIntervals);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetIntervalFraction
 * Signature: (J[JI[J[JI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetIntervalFraction
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlongArray valueReferences, jint nValueReferences,
         jlongArray intervalCounters, jlongArray resolutions, jint nIntervals) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3ValueReference *value_refs = createArray_uint_from_jlong(env, valueReferences, nValueReferences);
    DECLARE_ARRAY(fmi3UInt64, nativeIntervalCounters, nIntervals);
    DECLARE_ARRAY(fmi3UInt64, nativeResolutions, nIntervals);
    copyArray_jlong_to_fmi3UInt64(env, intervalCounters, nativeIntervalCounters, nIntervals);
    copyArray_jlong_to_fmi3UInt64(env, resolutions, nativeResolutions, nIntervals);

    fmi3Status status = fmuPtr_->fmi3SetIntervalFraction(instancePtr_, value_refs, nValueReferences,
                                                         nativeIntervalCounters,
                                                         nativeResolutions, nIntervals);

    free(value_refs);
    free(nativeIntervalCounters);
    free(nativeResolutions);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nUpdateDiscreteStates
 * Signature: (J[Z[Z[Z[Z[Z[D)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nUpdateDiscreteStates
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jbooleanArray discreteStatesNeedUpdateArrPtr,
         jbooleanArray terminateSimulationArrPtr, jbooleanArray nominalsOfContinuousStatesChangedArrPtr,
         jbooleanArray valuesOfContinuousStatesChangedArrPtr, jbooleanArray nextEventTimeDefinedArrPtr,
         jdoubleArray nextEventTimeArrPtr) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3Boolean discreteStatesNeedUpdate, terminateSimulation, nominalsOfContinuousStatesChanged, valuesOfContinuousStatesChanged, nextEventTimeDefined;
    fmi3Float64 nextEventTime;

    fmi3Status status = fmuPtr_->fmi3UpdateDiscreteStates(instancePtr_, &discreteStatesNeedUpdate, &terminateSimulation,
                                                          &nominalsOfContinuousStatesChanged,
                                                          &valuesOfContinuousStatesChanged, &nextEventTimeDefined,
                                                          &nextEventTime);

    jboolean *discreteStatesNeedUpdateElem = env->GetBooleanArrayElements(discreteStatesNeedUpdateArrPtr, JNI_FALSE);
    discreteStatesNeedUpdateElem[0] = (jboolean) discreteStatesNeedUpdate;
    env->ReleaseBooleanArrayElements(discreteStatesNeedUpdateArrPtr, discreteStatesNeedUpdateElem, 0);

    jboolean *terminateSimulationArrElem = env->GetBooleanArrayElements(terminateSimulationArrPtr, JNI_FALSE);
    terminateSimulationArrElem[0] = (jboolean) terminateSimulation;
    env->ReleaseBooleanArrayElements(terminateSimulationArrPtr, terminateSimulationArrElem, 0);

    jboolean *nominalsOfContinuousStatesChangedArrElem = env->GetBooleanArrayElements(
            nominalsOfContinuousStatesChangedArrPtr, JNI_FALSE);
    nominalsOfContinuousStatesChangedArrElem[0] = (jboolean) nominalsOfContinuousStatesChanged;
    env->ReleaseBooleanArrayElements(nominalsOfContinuousStatesChangedArrPtr, nominalsOfContinuousStatesChangedArrElem,
                                     0);

    jboolean *valuesOfContinuousStatesChangedArrElem = env->GetBooleanArrayElements(
            valuesOfContinuousStatesChangedArrPtr, JNI_FALSE);
    valuesOfContinuousStatesChangedArrElem[0] = (jboolean) valuesOfContinuousStatesChanged;
    env->ReleaseBooleanArrayElements(valuesOfContinuousStatesChangedArrPtr, valuesOfContinuousStatesChangedArrElem, 0);

    jboolean *nextEventTimeDefinedArrElem = env->GetBooleanArrayElements(nextEventTimeDefinedArrPtr, JNI_FALSE);
    nextEventTimeDefinedArrElem[0] = (jboolean) nextEventTimeDefined;
    env->ReleaseBooleanArrayElements(nextEventTimeDefinedArrPtr, nextEventTimeDefinedArrElem, 0);

    jdouble *nextEventTimeArrElem = env->GetDoubleArrayElements(nextEventTimeArrPtr, 0);
    nextEventTimeArrElem[0] = (jdouble) nextEventTime;
    env->ReleaseDoubleArrayElements(nextEventTimeArrPtr, nextEventTimeArrElem, 0);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nEnterContinuousTimeMode
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nEnterContinuousTimeMode
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3Status status = fmuPtr_->fmi3EnterContinuousTimeMode(instancePtr_);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nCompletedIntegratorStep
 * Signature: (JZ[Z[Z)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nCompletedIntegratorStep
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jboolean noSetFMUStatePriorToCurrentPoint,
         jbooleanArray enterEventModeArrPtr, jbooleanArray terminateSimulationArrPtr) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3Boolean enterEventMode, terminateSimulation, nativeNoSetFMUStatePriorToCurrentPoint = (fmi3Boolean) (
            noSetFMUStatePriorToCurrentPoint != JNI_FALSE);

    fmi3Status status = fmuPtr_->fmi3CompletedIntegratorStep(instancePtr_, nativeNoSetFMUStatePriorToCurrentPoint,
                                                             &enterEventMode, &terminateSimulation);

    jboolean *enterEventModeArrElem = env->GetBooleanArrayElements(enterEventModeArrPtr, JNI_FALSE);
    enterEventModeArrElem[0] = (jboolean) enterEventMode;
    env->ReleaseBooleanArrayElements(enterEventModeArrPtr, enterEventModeArrElem, 0);

    jboolean *terminateSimulationArrElem = env->GetBooleanArrayElements(terminateSimulationArrPtr, JNI_FALSE);
    terminateSimulationArrElem[0] = (jboolean) terminateSimulation;
    env->ReleaseBooleanArrayElements(terminateSimulationArrPtr, terminateSimulationArrElem, 0);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetTime
 * Signature: (JD)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetTime
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jdouble time) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3Status status = fmuPtr_->fmi3SetTime(instancePtr_, time);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetContinuousStates
 * Signature: (J[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetContinuousStates
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jdoubleArray continuousStates,
         jint nContinuousStates) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    DECLARE_ARRAY(fmi3Float64, nativeContinuousStates, nContinuousStates);
    copyArray_jdouble_to_fmi3Float64(env, continuousStates, nativeContinuousStates, nContinuousStates);
    fmi3Status status = fmuPtr_->fmi3SetContinuousStates(instancePtr_, nativeContinuousStates, nContinuousStates);

    free(nativeContinuousStates);
    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetDerivatives
 * Signature: (J[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetDerivatives
        (JNIEnv *, jobject, jlong, jdoubleArray, jint);

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetEventIndicators
 * Signature: (J[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetEventIndicators
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlong fmuInstance, jdoubleArray eventIndicators,
         jint nEventIndicators) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    DECLARE_ARRAY(fmi3Float64, nativeEventIndicators, nEventIndicators);
    fmi3Status status = fmuPtr_->fmi3GetEventIndicators(instancePtr_, nativeEventIndicators, nEventIndicators);
    copyArray_fmi3Float64_to_jdouble(env, nativeEventIndicators, eventIndicators, nEventIndicators);

    free(nativeEventIndicators);
    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetContinuousStates
 * Signature: (J[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetContinuousStates
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jdoubleArray continuousStates,
         jint nContinuousStates) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    DECLARE_ARRAY(fmi3Float64, nativeContinuousStates, nContinuousStates);
    fmi3Status status = fmuPtr_->fmi3GetContinuousStates(instancePtr_, nativeContinuousStates, nContinuousStates);
    copyArray_fmi3Float64_to_jdouble(env, nativeContinuousStates, continuousStates, nContinuousStates);

    free(nativeContinuousStates);
    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetNominalsOfContinuousStates
 * Signature: (J[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetNominalsOfContinuousStates
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jdoubleArray nominals, jint nContinuousStates) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    DECLARE_ARRAY(fmi3Float64, nativeNominals, nContinuousStates);
    fmi3Status status = fmuPtr_->fmi3GetNominalsOfContinuousStates(instancePtr_, nativeNominals, nContinuousStates);
    copyArray_fmi3Float64_to_jdouble(env, nativeNominals, nominals, nContinuousStates);

    free(nativeNominals);
    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetNumberOfEventIndicators
 * Signature: (J[I)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetNumberOfEventIndicators
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlongArray nEventIndicatorsArrPtr) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    size_t nEventIndicators;

    fmi3Status status = fmuPtr_->fmi3GetNumberOfEventIndicators(instancePtr_, &nEventIndicators);

    jlong *nEventIndicatorsElem = env->GetLongArrayElements(nEventIndicatorsArrPtr, JNI_FALSE);
    nEventIndicatorsElem[0] = (jlong) nEventIndicators;
    env->ReleaseLongArrayElements(nEventIndicatorsArrPtr, nEventIndicatorsElem, 0);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetNumberOfContinuousStates
 * Signature: (J[I)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetNumberOfContinuousStates
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlongArray nContinuousStatesArrPtr) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    size_t nContinuousStates;

    fmi3Status status = fmuPtr_->fmi3GetNumberOfContinuousStates(instancePtr_, &nContinuousStates);

    jlong *nContinuousStatesElem = env->GetLongArrayElements(nContinuousStatesArrPtr, JNI_FALSE);
    nContinuousStatesElem[0] = (jlong) nContinuousStates;
    env->ReleaseLongArrayElements(nContinuousStatesArrPtr, nContinuousStatesElem, 0);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nEnterStepMode
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nEnterStepMode
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3Status status = fmuPtr_->fmi3EnterStepMode(instancePtr_);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetOutputDerivatives
 * Signature: (J[JI[I[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetOutputDerivatives
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlongArray valueReferences, jint nValueReferences,
         jintArray orders, jdoubleArray values, jint nValues) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3ValueReference *value_refs = createArray_uint_from_jlong(env, valueReferences, nValueReferences);

    jsize ordersLen = env->GetArrayLength(orders);

    DECLARE_ARRAY(fmi3Int32, nativeOrders, ordersLen);
    copyArray_jint_to_fmi3Int32(env, orders, nativeOrders, ordersLen);
    DECLARE_ARRAY(fmi3Float64, nativeValues, nValues);

    fmi3Status status = fmuPtr_->fmi3GetOutputDerivatives(instancePtr_, value_refs, nValueReferences, nativeOrders,
                                                          nativeValues, nValues);
    copyArray_fmi3Float64_to_jdouble(env, nativeValues, values, nValues);

    free(value_refs);
    free(nativeOrders);
    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nDoStep
 * Signature: (JDDZ[Z[Z[Z[Z[D)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nDoStep
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jdouble currentCommunicationPoint,
         jdouble communicationStepSize, jboolean noSetFMUStatePriorToCurrentPoint, jbooleanArray eventEncounteredArrPtr,
         jbooleanArray terminateSimulationArrPtr, jbooleanArray earlyReturnArrPtr,
         jdoubleArray lastSuccessfulTimeArrPtr) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3Boolean eventEncountered, terminateSimulation, earlyReturn, nativeNoSetFMUStatePriorToCurrentPoint = (fmi3Boolean) (
            noSetFMUStatePriorToCurrentPoint != JNI_FALSE);

    fmi3Float64 lastSuccessfulTime;

    fmi3Status status = fmuPtr_->fmi3DoStep(instancePtr_, currentCommunicationPoint, communicationStepSize,
                                            nativeNoSetFMUStatePriorToCurrentPoint, &eventEncountered,
                                            &terminateSimulation, &earlyReturn, &lastSuccessfulTime);

    jboolean *eventEncounteredElem = env->GetBooleanArrayElements(eventEncounteredArrPtr, JNI_FALSE);
    eventEncounteredElem[0] = (jboolean) eventEncountered;
    env->ReleaseBooleanArrayElements(eventEncounteredArrPtr, eventEncounteredElem, 0);

    jboolean *terminateSimulationArrElem = env->GetBooleanArrayElements(terminateSimulationArrPtr, JNI_FALSE);
    terminateSimulationArrElem[0] = (jboolean) terminateSimulation;
    env->ReleaseBooleanArrayElements(terminateSimulationArrPtr, terminateSimulationArrElem, 0);

    jboolean *earlyReturnArrElem = env->GetBooleanArrayElements(
            earlyReturnArrPtr, JNI_FALSE);
    earlyReturnArrElem[0] = (jboolean) earlyReturn;
    env->ReleaseBooleanArrayElements(earlyReturnArrPtr, earlyReturnArrElem,
                                     0);

    jdouble *lastSuccessfulTimeArrElem = env->GetDoubleArrayElements(
            lastSuccessfulTimeArrPtr, JNI_FALSE);
    lastSuccessfulTimeArrElem[0] = (jdouble) lastSuccessfulTime;
    env->ReleaseDoubleArrayElements(lastSuccessfulTimeArrPtr, lastSuccessfulTimeArrElem, 0);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nActivateModelPartition
 * Signature: (JJID)Lorg/intocps/fmi3/Fmi3Status;
 */
JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nActivateModelPartition
        (JNIEnv *env, jobject obj, jlong fmuPtr, jlong instancePtr, jlong clockReference, jint clockElementIndex,
         jdouble activationTime) {

    fmi3Instance instancePtr_ = getInstancePtr(instancePtr);
    FMU3 *fmuPtr_ = getFmuPtr(fmuPtr);

    fmi3Status status = fmuPtr_->fmi3ActivateModelPartition(instancePtr_, clockReference, clockElementIndex,
                                                            activationTime);

    return convertStatus(env, status);
}