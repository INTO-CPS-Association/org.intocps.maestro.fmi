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
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetDebugLogging
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jboolean loggingOn,
         jobjectArray categories) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    auto size = env->GetArrayLength(categories);

    DECLARE_ARRAY(fmi3String, strings, size)
    copyJniArrayToNativeString(env, categories, strings, size);

    fmi3Status status = instanceNode->owner->fmu.fmi3SetDebugLogging(
            instanceNode->instance, loggingOn, size, strings);


    for (int i = 0; i < size; i++) {
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
__attribute__((unused)) JNIEXPORT void JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nFreeInstance
        (JNIEnv *, jobject, jlong instancePtr) {
    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;
    instanceNode->owner->fmu.fmi3FreeInstance(instanceNode->instance);
    Fmi3Manager::getInstance()->freeInstance(instanceNode->instance);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nEnterInitializationMode
 * Signature: (JZDDZD)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nEnterInitializationMode
        (JNIEnv *env, jobject, jlong instancePtr,
         jboolean toleranceDefined, jdouble tolerance, jdouble startTime,
         jboolean stopTimeDefined, jdouble stopTime) {
    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;
    return convertStatus(env,
                         instanceNode->owner->fmu.fmi3EnterInitializationMode(instanceNode->instance, toleranceDefined,
                                                                              tolerance,
                                                                              startTime, stopTimeDefined,
                                                                              stopTime));
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nExitInitializationMode
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nExitInitializationMode
        (JNIEnv *env, jobject, jlong instancePtr) {
    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;
    return convertStatus(env, instanceNode->owner->fmu.fmi3ExitInitializationMode(instanceNode->instance));
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
__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nEnterEventMode
        (JNIEnv *env, jobject, jlong instancePtr, jint stepEvent, jint stateEvent,
         jintArray rootsFound,
         jint nEventIndicators, jint timeEvent) {
    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;
    fmi3Int32 rootsFound_[nEventIndicators];
    fmi3Status status = instanceNode->owner->fmu.fmi3EnterEventMode(instanceNode->instance,(fmi3EventQualifier) stepEvent, (fmi3EventQualifier)stateEvent,
                                                                    rootsFound_,
                                                                    nEventIndicators,
                                                                    (fmi3EventQualifier) timeEvent);
    if (status == fmi3OK || status == fmi3Warning)
        copyArray_fmi3Int32_to_jint(env, rootsFound_, rootsFound, nEventIndicators);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nTerminate
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nTerminate
        (JNIEnv *env, jobject, jlong instancePtr) {
    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;
    return convertStatus(env, instanceNode->owner->fmu.fmi3Terminate(instanceNode->instance));
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nReset
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nReset
        (JNIEnv *env, jobject, jlong instancePtr) {
    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;
    return convertStatus(env, instanceNode->owner->fmu.fmi3Reset(instanceNode->instance));
}


/**************************************************************
 *
 *          GET
 *
 **************************************************************/

#define GET_TEMPLATE(fmiType, api, jniType) __attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGet##fmiType\
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlongArray valueReferences, jint nValueReferences,\
         jniType##Array values, jint nValues) {\
    Fmi3InstanceNode *instance = getInstancePtr( instancePtr);\
    jsize len = env->GetArrayLength(valueReferences);\
    fmi3ValueReference *vr_arr = createArray_uint_from_jlong(env, valueReferences, len);\
\
    fmi3 ## fmiType outputValues[len];\
    auto status = instance->owner->fmu.fmi3Get##fmiType(instance->instance, vr_arr, len, outputValues,len);\
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

GET_TEMPLATE(Int8, Byte, jbyte)

GET_TEMPLATE(UInt8, Int, jbyte)

GET_TEMPLATE(Int16, Short, jshort)

GET_TEMPLATE(UInt16, Short, jshort)

GET_TEMPLATE(Int32, Int, jint)

GET_TEMPLATE(UInt32, Int, jint)

GET_TEMPLATE(Int64, Long, jlong)

GET_TEMPLATE(UInt64, Long, jlong)

GET_TEMPLATE(Boolean, Boolean, jboolean)

// The functions below are all implemented via the templated version above.

//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetFloat32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetFloat64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);

//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetInt8
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetUInt8
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetInt16
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetUInt16
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetInt32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetUInt32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetInt64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetUInt64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jlongArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetBoolean
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jbooleanArray, jint);
//
__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetBinary
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instanceNodePtr, jlongArray valueReferences,
         jint nValueReferences,
         jobjectArray values, jint nValues) {

    auto *instanceNode = (Fmi3InstanceNode *) instanceNodePtr;

    DECLARE_ARRAY(fmi3Binary, nativeByteValues, nValues)
    fmi3ValueReference *nativeValueReferences = createArray_uint_from_jlong(env, valueReferences, nValueReferences);
    DECLARE_ARRAY(size_t, nativeValueSizes, nValues)


    fmi3Status status = instanceNode->owner->fmu.fmi3GetBinary(instanceNode->instance, nativeValueReferences,
                                                               nValueReferences,
                                                               nativeValueSizes, nativeByteValues, nValues);

    if (status == fmi3Status::fmi3OK) {
        for (int i = 0; i < nValues; i++) {
            int byteValueLength = (int) nativeValueSizes[i];
            jbyteArray vRefByteArray = env->NewByteArray(byteValueLength);

            env->SetByteArrayRegion(vRefByteArray, 0, byteValueLength,
                                    reinterpret_cast<const jbyte *>(nativeByteValues[i]));

            env->SetObjectArrayElement(values, i, vRefByteArray);
            env->DeleteLocalRef(vRefByteArray);
        }

    }

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
__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetString
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instanceNodePtr, jlongArray valueReferences,
         jint nValueReferences,
         jobjectArray values, jint nValues) {

    auto *instanceNode = (Fmi3InstanceNode *) instanceNodePtr;

    DECLARE_ARRAY(fmi3String, nativeStrings, nValues)
    fmi3ValueReference *nativeValueReferences = createArray_uint_from_jlong(env, valueReferences, nValueReferences);

    fmi3Status status = instanceNode->owner->fmu.fmi3GetString(instanceNode->instance, nativeValueReferences,
                                                               nValueReferences,
                                                               nativeStrings, nValues);

    if (status == fmi3Status::fmi3OK) {
        //only assume valid string data if the fmi function returned ok, and only convert the number of values that could have been written
        for (int i = 0; i < nValues && i < nValueReferences; i++) {
            auto val = nativeStrings[i];
            if (val != nullptr) {
                jstring jstrBuf = env->NewStringUTF(val);
                env->SetObjectArrayElement(values, i, jstrBuf);
                env->DeleteLocalRef(jstrBuf);
            }
        }
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

//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetFloat32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetFloat64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jdoubleArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetInt8
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetUInt8
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetInt16
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetUInt16
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetInt32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetUInt32
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetInt64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jintArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetUInt64
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jlongArray, jint);
//
//__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetBoolean
//        (JNIEnv *, jobject, jlong, jlongArray, jint, jbooleanArray, jint);

#define SET_TEMPLATE(fmiType, api, jniType) __attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSet##fmiType\
(JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlongArray valueReferences, jint nValueReferences,\
         jniType##Array values, jint nValues)\
{\
     auto instance = getInstancePtr( instancePtr);\
\
    jsize len = env->GetArrayLength( valueReferences);\
\
    fmi3ValueReference *vr_arr = createArray_uint_from_jlong(env, valueReferences, len);\
\
    fmi3##fmiType inputs[len]; \
    copyArray_##jniType##_to_##fmi3##fmiType(env, values,inputs, len);\
\
    auto status = instance->owner->fmu.fmi3Set##fmiType(instance->instance, vr_arr, len, inputs,len);\
\
    delete vr_arr;\
\
    return convertStatus(env, status);\
}

SET_TEMPLATE(Float32, Float, jfloat)

SET_TEMPLATE(Float64, Double, jdouble)

SET_TEMPLATE(Int8, Byte, jbyte)

SET_TEMPLATE(UInt8, Byte, jbyte)

SET_TEMPLATE(Int16, Short, jshort)

SET_TEMPLATE(UInt16, Short, jshort)

SET_TEMPLATE(Int32, Int, jint)

SET_TEMPLATE(UInt32, Int, jint)

SET_TEMPLATE(Int64, Long, jlong)

SET_TEMPLATE(UInt64, Long, jlong)

SET_TEMPLATE(Boolean, Boolean, jboolean)

__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetBinary
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instanceNodePtr, jlongArray valueReferences,
         jint nValueReferences,
         jlongArray valueSizes, jobjectArray values, jint nValues) {

    auto instanceNode = getInstancePtr(instanceNodePtr);

    fmi3Binary nativeByteValues[nValues];
    std::map<jbyteArray, jbyte *> reservedByteValues;
    jbyteArray reservedArrays[nValues];
    fmi3ValueReference *nativeValueReferences = createArray_uint_from_jlong(env, valueReferences, nValueReferences);

    DECLARE_ARRAY(size_t, nativeValueSizes, nValues)

    auto elems = env->GetLongArrayElements(valueSizes, JNI_FALSE);
    for (int i = 0; i < nValues; i++) {
        nativeValueSizes[i] = elems[i];
    }
    env->ReleaseLongArrayElements(valueSizes, elems, 0);

    for (int i = 0; i < nValues; i++) {
        auto vRefValueArray = (jbyteArray) env->GetObjectArrayElement(values, i);
        if (vRefValueArray != nullptr && nativeValueSizes[i] > 0) {
            jbyte *bytesPtr = env->GetByteArrayElements(vRefValueArray, JNI_FALSE);
            reservedByteValues.insert(pair<jbyteArray, jbyte *>(vRefValueArray, bytesPtr));
            nativeByteValues[i] = (fmi3Binary) bytesPtr;
        } else {
            nativeByteValues[i] = nullptr;
        }
        reservedArrays[i] = vRefValueArray;
        // env->DeleteLocalRef(vRefValueArray);
    }

    fmi3Status status = instanceNode->owner->fmu.fmi3SetBinary(instanceNode->instance, nativeValueReferences,
                                                               nValueReferences, nativeValueSizes, nativeByteValues,
                                                               nValues);

    for (auto const &entry: reservedByteValues) {
        env->ReleaseByteArrayElements(entry.first, entry.second, 0);
    }

    for (const auto &item: reservedArrays) {
        env->DeleteLocalRef(item);
    }

    free(nativeValueReferences);
    free(nativeValueSizes);
    return convertStatus(env, status);
}


__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetString
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instanceNodePtr, jlongArray valueReferences,
         jint nValueReferences,
         jobjectArray values, jint nValues) {

    auto instanceNode = getInstancePtr(instanceNodePtr);

    fmi3String nativeStrings[nValues];
    jstring reservedStrings[nValues];
    fmi3ValueReference *nativeValueReferences = createArray_uint_from_jlong(env, valueReferences, nValueReferences);

    for (int i = 0; i < nValues; i++) {
        auto value = (jstring) (env->GetObjectArrayElement(values, i));
        reservedStrings[i] = value;
        if (value != nullptr) { nativeStrings[i] = env->GetStringUTFChars(value, JNI_FALSE); }
        else {
            // fmi does not support 3 value strings
            nativeStrings[i] = "\0";
        }
        env->DeleteLocalRef(value);
    }

    fmi3Status status = instanceNode->owner->fmu.fmi3SetString(instanceNode->instance, nativeValueReferences,
                                                               nValueReferences,
                                                               nativeStrings, nValues);

    for (int i = 0; i < nValues; i++) {
        if (reservedStrings[i] != nullptr) {
            env->ReleaseStringUTFChars(reservedStrings[i], nativeStrings[i]);
        }
    }

    free(nativeValueReferences);
    return convertStatus(env, status);
}


/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetNumberOfVariableDependencies
 * Signature: (JJ[I)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetNumberOfVariableDependencies
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlong valueReference,
         jlongArray nDependenciesArrPtr) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    auto nativeValueReference = (fmi3ValueReference) valueReference;
    size_t nDependencies;
    fmi3Status status = instanceNode->owner->fmu.fmi3GetNumberOfVariableDependencies(instanceNode->instance,
                                                                                     nativeValueReference,
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
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetVariableDependencies
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlong dependent,
         jlongArray elementIndicesOfDependent, jlongArray independents, jlongArray elementIndicesOfIndependents,
         jobjectArray dependencyKinds, jlong nDependencies) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    auto nativeDependent = (fmi3ValueReference) dependent;
    DECLARE_ARRAY(fmi3ValueReference, nativeIndependents, nDependencies)
    DECLARE_ARRAY(size_t, nativeElementIndicesOfDependent, nDependencies)
    DECLARE_ARRAY(size_t, nativeElementIndicesOfIndependents, nDependencies)
    DECLARE_ARRAY(fmi3DependencyKind, nativeDependencyKinds, nDependencies)

    fmi3Status status = instanceNode->owner->fmu.fmi3GetVariableDependencies(instanceNode->instance, nativeDependent,
                                                                             nativeElementIndicesOfDependent,
                                                                             nativeIndependents,
                                                                             nativeElementIndicesOfIndependents,
                                                                             nativeDependencyKinds,
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
__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetFMUState
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlongArray statePtrArr) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3FMUState state = nullptr;
    fmi3Status status = instanceNode->owner->fmu.fmi3GetFMUState(instanceNode->instance, &state);

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
__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetFMUState
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlong statePtr) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3FMUState state = ((fmi3FMUState) statePtr);
    fmi3Status status = instanceNode->owner->fmu.fmi3SetFMUState(instanceNode->instance, state);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nFreeFMUState
 * Signature: (JJ)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nFreeFMUState
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlong statePtr) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3FMUState state = ((fmi3FMUState) statePtr);
    fmi3Status status = instanceNode->owner->fmu.fmi3FreeFMUState(instanceNode->instance, &state);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSerializedFMUStateSize
 * Signature: (JJ[I)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSerializedFMUStateSize
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlong statePtr,
         jlongArray sizeArrPtr) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3FMUState state = ((fmi3FMUState) statePtr);

    size_t size;
    fmi3Status status = instanceNode->owner->fmu.fmi3SerializedFMUStateSize(instanceNode->instance, state, &size);

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
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetDirectionalDerivative
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlongArray unknowns,
         jint nUnknows,
         jlongArray knowns, jint nKnowns, jdoubleArray seed, jint nSeed, jdoubleArray sensitivity, jint nSensitivity) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3ValueReference *unknowns_refs = createArray_uint_from_jlong(env, unknowns, nUnknows);
    fmi3ValueReference *knowns_refs = createArray_uint_from_jlong(env, knowns, nKnowns);

    DECLARE_ARRAY(fmi3Float64, nativeSeed, nSeed)
    copyArray_jdouble_to_fmi3Float64(env, seed, nativeSeed, nSeed);

    DECLARE_ARRAY(fmi3Float64, nativeSensitivity, nSensitivity)

    fmi3Status status = instanceNode->owner->fmu.fmi3GetDirectionalDerivative(
            instanceNode->instance, unknowns_refs, nUnknows, knowns_refs, nKnowns, nativeSeed, nSeed,
            nativeSensitivity,
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
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetAdjointDerivative
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlongArray unknowns,
         jint nUnknows,
         jlongArray knowns, jint nKnowns, jdoubleArray seed, jint nSeed, jdoubleArray sensitivity, jint nSensitivity) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3ValueReference *unknowns_refs = createArray_uint_from_jlong(env, unknowns, nUnknows);
    fmi3ValueReference *knowns_refs = createArray_uint_from_jlong(env, knowns, nKnowns);

    DECLARE_ARRAY(fmi3Float64, nativeSeed, nSeed)
    copyArray_jdouble_to_fmi3Float64(env, seed, nativeSeed, nSeed);

    DECLARE_ARRAY(fmi3Float64, nativeSensitivity, nSensitivity)

    fmi3Status status = instanceNode->owner->fmu.fmi3GetAdjointDerivative(
            instanceNode->instance, unknowns_refs, nUnknows, knowns_refs, nKnowns, nativeSeed, nSeed,
            nativeSensitivity,
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
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nEnterConfigurationMode
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3Status status = instanceNode->owner->fmu.fmi3EnterConfigurationMode(instanceNode->instance);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nExitConfigurationMode
 * Signature: (J)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nExitConfigurationMode
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3Status status = instanceNode->owner->fmu.fmi3ExitConfigurationMode(instanceNode->instance);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetClock
 * Signature: (J[JI[II)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetClock
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlongArray valueReferences,
         jint nValueReferences,
         jbooleanArray values) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3ValueReference *value_refs = createArray_uint_from_jlong(env, valueReferences, nValueReferences);
    DECLARE_ARRAY(fmi3Clock, nativeClocks, nValueReferences)

    fmi3Status status = instanceNode->owner->fmu.fmi3GetClock(instanceNode->instance, value_refs, nValueReferences,
                                                              nativeClocks);

    copyArray_fmi3Clock_to_jboolean(env, nativeClocks, values, nValueReferences);

    free(value_refs);
    free(nativeClocks);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetClock
 * Signature: (J[JI[I[ZI)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetClock
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlongArray valueReferences,
         jint nValueReferences,
         jbooleanArray values) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3ValueReference *value_refs = createArray_uint_from_jlong(env, valueReferences, nValueReferences);
    DECLARE_ARRAY(fmi3Clock, nativeClocks, nValueReferences)
    copyArray_jboolean_to_fmi3Clock(env, values, nativeClocks, nValueReferences);

    fmi3Status status = instanceNode->owner->fmu.fmi3GetClock(instanceNode->instance, value_refs, nValueReferences,
                                                              nativeClocks);

    free(value_refs);
    free(nativeClocks);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetIntervalDecimal
 * Signature: (J[JI[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetIntervalDecimal
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlongArray valueReferences,
         jint nValueReferences,
         jdoubleArray intervals, jobjectArray qualifiers) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;
    fmi3ValueReference *value_refs = createArray_uint_from_jlong(env, valueReferences, nValueReferences);

    DECLARE_ARRAY(fmi3IntervalQualifier, nativeQualifiers, nValueReferences)
    DECLARE_ARRAY(fmi3Float64, nativeIntervals, nValueReferences)

    fmi3Status status = instanceNode->owner->fmu.fmi3GetIntervalDecimal(instanceNode->instance, value_refs,
                                                                        nValueReferences,
                                                                        nativeIntervals,
                                                                        nativeQualifiers);
    copyArray_fmi3IntervalQualifiers_to_javaEnum(env, nativeQualifiers, qualifiers, nValueReferences);
    copyArray_fmi3Float64_to_jdouble(env, nativeIntervals, intervals, nValueReferences);

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
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetIntervalFraction
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlongArray valueReferences,
         jint nValueReferences,
         jlongArray intervalCounters, jlongArray resolutions, jobjectArray qualifiers) {

    //intervalCounters length == resolutions length == nIntervals
    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;
    fmi3ValueReference *value_refs = createArray_uint_from_jlong(env, valueReferences, nValueReferences);

    DECLARE_ARRAY(fmi3IntervalQualifier, nativeQualifiers, nValueReferences)
    DECLARE_ARRAY(fmi3UInt64, nativeIntervalCounters, nValueReferences)
    DECLARE_ARRAY(fmi3UInt64, nativeResolutions, nValueReferences)


    fmi3Status status = instanceNode->owner->fmu.fmi3GetIntervalFraction(instanceNode->instance, value_refs,
                                                                         nValueReferences,
                                                                         nativeIntervalCounters, nativeResolutions,
                                                                         nativeQualifiers
    );
    copyArray_fmi3IntervalQualifiers_to_javaEnum(env, nativeQualifiers, qualifiers, nValueReferences);
    copyArray_fmi3UInt64_to_jlong(env, nativeIntervalCounters, intervalCounters, nValueReferences);
    copyArray_fmi3UInt64_to_jlong(env, nativeResolutions, resolutions, nValueReferences);

    free(value_refs);
    free(nativeQualifiers);
    free(nativeIntervalCounters);
    free(nativeResolutions);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance
 * Method:    nGetShiftDecimal
 * Signature: (J[JI[D)Lorg/intocps/fmi/jnifmuapi/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetShiftDecimal
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlongArray valueReferences,
         jint nValueReferences, jdoubleArray shifts) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;
    fmi3ValueReference *value_refs = createArray_uint_from_jlong(env, valueReferences, nValueReferences);

    DECLARE_ARRAY(fmi3Float64, nativeShifts, nValueReferences)

    fmi3Status status = instanceNode->owner->fmu.fmi3GetShiftDecimal(instanceNode->instance, value_refs,
                                                                     nValueReferences, nativeShifts);

    copyArray_fmi3Float64_to_jdouble(env, nativeShifts, shifts, nValueReferences);

    free(value_refs);
    free(nativeShifts);

    return convertStatus(env, status);

}

/*
 * Class:     org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance
 * Method:    nGetShiftFraction
 * Signature: (J[II[D[D)Lorg/intocps/fmi/jnifmuapi/fmi3/Fmi3Status;
 */
__attribute__((unused))  JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetShiftFraction
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlongArray valueReferences,
         jint nValueReferences, jlongArray shiftCounters, jlongArray resolutions) {
    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;
    fmi3ValueReference *value_refs = createArray_uint_from_jlong(env, valueReferences, nValueReferences);

    DECLARE_ARRAY(fmi3UInt64, nativeShiftCounters, nValueReferences)
    DECLARE_ARRAY(fmi3UInt64, nativeResolutions, nValueReferences)


    fmi3Status status = instanceNode->owner->fmu.fmi3GetShiftFraction(instanceNode->instance, value_refs,
                                                                      nValueReferences,
                                                                      nativeShiftCounters, nativeResolutions
    );
    copyArray_fmi3UInt64_to_jlong(env, nativeShiftCounters, shiftCounters, nValueReferences);
    copyArray_fmi3UInt64_to_jlong(env, nativeResolutions, resolutions, nValueReferences);

    free(value_refs);
    free(nativeShiftCounters);
    free(nativeResolutions);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetIntervalDecimal
 * Signature: (J[JI[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetIntervalDecimal
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlongArray valueReferences,
         jint nValueReferences,
         jdoubleArray intervals) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3ValueReference *value_refs = createArray_uint_from_jlong(env, valueReferences, nValueReferences);
    DECLARE_ARRAY(fmi3Float64, nativeIntervals, nValueReferences)
    copyArray_jdouble_to_fmi3Float64(env, intervals, nativeIntervals, nValueReferences);

    fmi3Status status = instanceNode->owner->fmu.fmi3SetIntervalDecimal(instanceNode->instance, value_refs,
                                                                        nValueReferences,
                                                                        nativeIntervals
    );

    free(value_refs);
    free(nativeIntervals);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetIntervalFraction
 * Signature: (J[JI[J[JI)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetIntervalFraction
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlongArray valueReferences,
         jint nValueReferences,
         jlongArray intervalCounters, jlongArray resolutions) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3ValueReference *value_refs = createArray_uint_from_jlong(env, valueReferences, nValueReferences);
    DECLARE_ARRAY(fmi3UInt64, nativeIntervalCounters, nValueReferences)
    DECLARE_ARRAY(fmi3UInt64, nativeResolutions, nValueReferences)
    copyArray_jlong_to_fmi3UInt64(env, intervalCounters, nativeIntervalCounters, nValueReferences);
    copyArray_jlong_to_fmi3UInt64(env, resolutions, nativeResolutions, nValueReferences);

    fmi3Status status = instanceNode->owner->fmu.fmi3SetIntervalFraction(instanceNode->instance, value_refs,
                                                                         nValueReferences,
                                                                         nativeIntervalCounters,
                                                                         nativeResolutions);

    free(value_refs);
    free(nativeIntervalCounters);
    free(nativeResolutions);

    return convertStatus(env, status);
}


/*
 * Class:     org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance
 * Method:    nEvaluateDiscreteStates
 * Signature: (J)Lorg/intocps/fmi/jnifmuapi/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nEvaluateDiscreteStates
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr){
    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    return convertStatus(env,instanceNode->owner->fmu.fmi3EvaluateDiscreteStates(instanceNode->instance));

}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nUpdateDiscreteStates
 * Signature: (J[Z[Z[Z[Z[Z[D)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nUpdateDiscreteStates
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr,
         jbooleanArray discreteStatesNeedUpdateArrPtr,
         jbooleanArray terminateSimulationArrPtr, jbooleanArray nominalsOfContinuousStatesChangedArrPtr,
         jbooleanArray valuesOfContinuousStatesChangedArrPtr, jbooleanArray nextEventTimeDefinedArrPtr,
         jdoubleArray nextEventTimeArrPtr) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3Boolean discreteStatesNeedUpdate, terminateSimulation, nominalsOfContinuousStatesChanged, valuesOfContinuousStatesChanged, nextEventTimeDefined;
    fmi3Float64 nextEventTime;

    fmi3Status status = instanceNode->owner->fmu.fmi3UpdateDiscreteStates(instanceNode->instance,
                                                                          &discreteStatesNeedUpdate,
                                                                          &terminateSimulation,
                                                                          &nominalsOfContinuousStatesChanged,
                                                                          &valuesOfContinuousStatesChanged,
                                                                          &nextEventTimeDefined,
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
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nEnterContinuousTimeMode
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3Status status = instanceNode->owner->fmu.fmi3EnterContinuousTimeMode(instanceNode->instance);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nCompletedIntegratorStep
 * Signature: (JZ[Z[Z)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nCompletedIntegratorStep
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr,
         jboolean noSetFMUStatePriorToCurrentPoint,
         jbooleanArray enterEventModeArrPtr, jbooleanArray terminateSimulationArrPtr) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3Boolean enterEventMode, terminateSimulation, nativeNoSetFMUStatePriorToCurrentPoint = (fmi3Boolean) (
            noSetFMUStatePriorToCurrentPoint != JNI_FALSE);

    fmi3Status status = instanceNode->owner->fmu.fmi3CompletedIntegratorStep(instanceNode->instance,
                                                                             nativeNoSetFMUStatePriorToCurrentPoint,
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
__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetTime
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jdouble time) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3Status status = instanceNode->owner->fmu.fmi3SetTime(instanceNode->instance, time);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nSetContinuousStates
 * Signature: (J[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nSetContinuousStates
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr,
         jdoubleArray continuousStates,
         jint nContinuousStates) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    DECLARE_ARRAY(fmi3Float64, nativeContinuousStates, nContinuousStates)
    copyArray_jdouble_to_fmi3Float64(env, continuousStates, nativeContinuousStates, nContinuousStates);
    fmi3Status status = instanceNode->owner->fmu.fmi3SetContinuousStates(instanceNode->instance, nativeContinuousStates,
                                                                         nContinuousStates);

    free(nativeContinuousStates);
    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetDerivatives
 * Signature: (J[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) __attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetDerivatives
        (JNIEnv *, jobject, jlong, jdoubleArray, jint) {
    //FIXME not yet implemented
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetEventIndicators
 * Signature: (J[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetEventIndicators
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr,
         jdoubleArray eventIndicators,
         jint nEventIndicators) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    DECLARE_ARRAY(fmi3Float64, nativeEventIndicators, nEventIndicators)
    fmi3Status status = instanceNode->owner->fmu.fmi3GetEventIndicators(instanceNode->instance, nativeEventIndicators,
                                                                        nEventIndicators);
    copyArray_fmi3Float64_to_jdouble(env, nativeEventIndicators, eventIndicators, nEventIndicators);

    free(nativeEventIndicators);
    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetContinuousStates
 * Signature: (J[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetContinuousStates
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr,
         jdoubleArray continuousStates,
         jint nContinuousStates) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    DECLARE_ARRAY(fmi3Float64, nativeContinuousStates, nContinuousStates)
    fmi3Status status = instanceNode->owner->fmu.fmi3GetContinuousStates(instanceNode->instance, nativeContinuousStates,
                                                                         nContinuousStates);
    copyArray_fmi3Float64_to_jdouble(env, nativeContinuousStates, continuousStates, nContinuousStates);

    free(nativeContinuousStates);
    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetNominalsOfContinuousStates
 * Signature: (J[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetNominalsOfContinuousStates
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jdoubleArray nominals,
         jint nContinuousStates) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    DECLARE_ARRAY(fmi3Float64, nativeNominals, nContinuousStates)
    fmi3Status status = instanceNode->owner->fmu.fmi3GetNominalsOfContinuousStates(instanceNode->instance,
                                                                                   nativeNominals,
                                                                                   nContinuousStates);
    copyArray_fmi3Float64_to_jdouble(env, nativeNominals, nominals, nContinuousStates);

    free(nativeNominals);
    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetNumberOfEventIndicators
 * Signature: (J[I)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetNumberOfEventIndicators
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr,
         jlongArray nEventIndicatorsArrPtr) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    size_t nEventIndicators;

    fmi3Status status = instanceNode->owner->fmu.fmi3GetNumberOfEventIndicators(instanceNode->instance,
                                                                                &nEventIndicators);

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
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetNumberOfContinuousStates
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr,
         jlongArray nContinuousStatesArrPtr) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    size_t nContinuousStates;

    fmi3Status status = instanceNode->owner->fmu.fmi3GetNumberOfContinuousStates(instanceNode->instance,
                                                                                 &nContinuousStates);

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
__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nEnterStepMode
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3Status status = instanceNode->owner->fmu.fmi3EnterStepMode(instanceNode->instance);

    return convertStatus(env, status);
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmuInstance3
 * Method:    nGetOutputDerivatives
 * Signature: (J[JI[I[DI)Lorg/intocps/fmi3/Fmi3Status;
 */
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nGetOutputDerivatives
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlongArray valueReferences,
         jint nValueReferences,
         jintArray orders, jdoubleArray values, jint nValues) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3ValueReference *value_refs = createArray_uint_from_jlong(env, valueReferences, nValueReferences);

    jsize ordersLen = env->GetArrayLength(orders);

    DECLARE_ARRAY(fmi3Int32, nativeOrders, ordersLen)
    copyArray_jint_to_fmi3Int32(env, orders, nativeOrders, ordersLen);
    DECLARE_ARRAY(fmi3Float64, nativeValues, nValues)

    fmi3Status status = instanceNode->owner->fmu.fmi3GetOutputDerivatives(instanceNode->instance, value_refs,
                                                                          nValueReferences,
                                                                          nativeOrders,
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
__attribute__((unused)) JNIEXPORT jobject JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nDoStep
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr,
         jdouble currentCommunicationPoint,
         jdouble communicationStepSize, jboolean noSetFMUStatePriorToCurrentPoint, jbooleanArray eventEncounteredArrPtr,
         jbooleanArray terminateSimulationArrPtr, jbooleanArray earlyReturnArrPtr,
         jdoubleArray lastSuccessfulTimeArrPtr) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3Boolean eventEncountered, terminateSimulation, earlyReturn, nativeNoSetFMUStatePriorToCurrentPoint = (fmi3Boolean) (
            noSetFMUStatePriorToCurrentPoint != JNI_FALSE);

    fmi3Float64 lastSuccessfulTime;

    fmi3Status status = instanceNode->owner->fmu.fmi3DoStep(instanceNode->instance, currentCommunicationPoint,
                                                            communicationStepSize,
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
    env->ReleaseBooleanArrayElements(earlyReturnArrPtr, earlyReturnArrElem, 0);

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
__attribute__((unused)) JNIEXPORT jobject JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3Instance_nActivateModelPartition
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong instancePtr, jlong clockReference,
         jdouble activationTime) {

    auto *instanceNode = (Fmi3InstanceNode *) instancePtr;

    fmi3Status status = instanceNode->owner->fmu.fmi3ActivateModelPartition(instanceNode->instance, clockReference,
                                                                            activationTime);

    return convertStatus(env, status);
}