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
#include "comp_api.h"

fmi2Component getCompPtr(jlong componentPtr) {
  return (fmi2Component)componentPtr;
}

fmi2ValueReference *createValueRefArray(JNIEnv *env, jlongArray vr, jsize len) {
  int i;
  jlong *body = (*env)->GetLongArrayElements(env, vr, 0);

  fmi2ValueReference *vr_arr =
      (fmi2ValueReference *)malloc(sizeof(fmi2ValueReference) * len);

  if (vr_arr == NULL) {
    throwException(env, "malloc vr_arr failed");
  }

  for (i = 0; i < len; i++) {
    vr_arr[i] = body[i];
  }

  (*env)->ReleaseLongArrayElements(env, vr, body, 0);

  return vr_arr;
}

#define DECLARE_ARRAY(type, name, length)             \
  type *name = (type *)malloc(sizeof(type) * length); \
  if (name == NULL) {                                 \
    throwException(env, "malloc r failed");           \
  }

/********************************************************
 * FMU Component
 ********************************************************/

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nSetDebugLogging(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong compPtr, jboolean logginOn,
    jobjectArray categories) {
  size_t size = (*env)->GetArrayLength(env, categories);

  DECLARE_ARRAY(fmi2String, strings, size);

  copyJniArrayToNativeString(env, categories, strings, size);

  fmi2Status status = getFmuPtr(fmuPtr)->setDebugLogging(
      getCompPtr(compPtr), logginOn, size, strings);

  int i;

  for (i = 0; i < size; i++) {
    free((char *)strings[i]);
  }
  free(strings);
  return status;
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nTerminate(
    JNIEnv *env, jobject obj, jlong fmuptr, jlong componentPtr) {
  fmi2Component comp = getCompPtr(componentPtr);
  return getFmuPtr(fmuptr)->terminate(comp);
}

JNIEXPORT void JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nFreeInstance(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr) {
  fmi2Component comp = getCompPtr(componentPtr);

  getFmuPtr(fmuPtr)->freeInstance(comp);

  freeComp(env, comp);
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nSetupExperiment(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr,
    jboolean toleranceDefined, jdouble tolerance, jdouble tStart, jboolean x,
    jdouble tEnd) {
  fmi2Component comp = getCompPtr(componentPtr);
  return getFmuPtr(fmuPtr)
      ->setupExperiment(comp, toleranceDefined, tolerance, tStart, x, tEnd);
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nEnterInitializationMode(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr) {
  fmi2Component comp = (fmi2Component)componentPtr;
  return getFmuPtr(fmuPtr)->enterInitializationMode(comp);
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nExitInitializationMode(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr) {
  fmi2Component comp = getCompPtr(componentPtr);
  return getFmuPtr(fmuPtr)->exitInitializationMode(comp);
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nReset(JNIEnv *env,
                                                         jobject obj,
                                                         jlong fmuPtr,
                                                         jlong componentPtr) {
  fmi2Component comp = getCompPtr(componentPtr);
  return getFmuPtr(fmuPtr)->reset(comp);
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nGetReal(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr, jlongArray vr,
    jlong size, jdoubleArray values) {
  fmi2Component c = getCompPtr(componentPtr);

  jsize len = (*env)->GetArrayLength(env, vr);

  fmi2ValueReference *vr_arr = createValueRefArray(env, vr, len);

  DECLARE_ARRAY(fmi2Real, r, len);

  fmi2Status status = getFmuPtr(fmuPtr)->getReal(c, vr_arr, len, r);

  if (status == fmi2OK || status == fmi2Warning) {
    copyNativeArrayToJniDouble(env, r, values, len);
  }

  free(r);
  free(vr_arr);

  return status;
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nGetInteger(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr, jlongArray vr,
    jlong size, jintArray values) {
  fmi2Component c = getCompPtr(componentPtr);

  jsize len = (*env)->GetArrayLength(env, vr);

  fmi2ValueReference *vr_arr = createValueRefArray(env, vr, len);

  DECLARE_ARRAY(fmi2Integer, r, len);

  fmi2Status status = getFmuPtr(fmuPtr)->getInteger(c, vr_arr, len, r);

  if (status == fmi2OK || status == fmi2Warning) {
    copyNativeArrayToJniInteger(env, r, values, len);
  }

  free(r);
  free(vr_arr);

  return status;
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nGetBoolean(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr, jlongArray vr,
    jlong size, jbooleanArray values) {
  fmi2Component c = getCompPtr(componentPtr);

  jsize len = (*env)->GetArrayLength(env, vr);

  fmi2ValueReference *vr_arr = createValueRefArray(env, vr, len);

  DECLARE_ARRAY(fmi2Boolean, b, len);

  fmi2Status status = getFmuPtr(fmuPtr)->getBoolean(c, vr_arr, len, b);

  if (status == fmi2OK || status == fmi2Warning) {
    copyNativeArrayToJniBoolean(env, b, values, len);
  }

  free(b);
  free(vr_arr);

  return status;
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nGetString(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr, jlongArray vr,
    jlong size, jobjectArray values) {
  fmi2Component c = getCompPtr(componentPtr);
  jsize len = (*env)->GetArrayLength(env, vr);

  fmi2ValueReference *vr_arr = createValueRefArray(env, vr, len);

  DECLARE_ARRAY(fmi2String, returnStrings, len);

  fmi2Status status =
      getFmuPtr(fmuPtr)->getString(c, vr_arr, len, returnStrings);

  if (status == fmi2OK || status == fmi2Warning) {
    copyNativeArrayToJniString(env, returnStrings, values, len);
  }

  free(returnStrings);
  free(vr_arr);

  return status;
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nSetBoolean(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr, jlongArray vr,
    jlong size, jbooleanArray values) {
  fmi2Component c = getCompPtr(componentPtr);

  jsize len = (*env)->GetArrayLength(env, vr);

  fmi2ValueReference *vr_arr = createValueRefArray(env, vr, len);

  DECLARE_ARRAY(fmi2Boolean, b, len);

  copyJniArrayToNativeBoolean(env, values, b, len);

  fmi2Status status = getFmuPtr(fmuPtr)->setBoolean(c, vr_arr, len, b);

  free(b);
  free(vr_arr);

  return status;
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nSetInteger(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr, jlongArray vr,
    jlong size, jintArray values) {
  fmi2Component c = getCompPtr(componentPtr);

  jsize len = (*env)->GetArrayLength(env, vr);

  fmi2ValueReference *vr_arr = createValueRefArray(env, vr, len);

  DECLARE_ARRAY(fmi2Integer, b, len);

  copyJniArrayToNativeInteger(env, values, b, len);

  fmi2Status status = getFmuPtr(fmuPtr)->setInteger(c, vr_arr, len, b);

  free(b);
  free(vr_arr);

  return status;
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nSetReal(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr, jlongArray vr,
    jlong size, jdoubleArray values) {
  fmi2Component c = getCompPtr(componentPtr);

  jsize len = (*env)->GetArrayLength(env, vr);

  fmi2ValueReference *vr_arr = createValueRefArray(env, vr, len);

  DECLARE_ARRAY(fmi2Real, r, len);

  copyJniArrayToNativeDouble(env, values, r, len);

  fmi2Status status = getFmuPtr(fmuPtr)->setReal(c, vr_arr, len, r);

  free(r);
  free(vr_arr);

  return status;
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nSetString(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr, jlongArray vr,
    jlong size, jobjectArray values) {
  fmi2Component c = getCompPtr(componentPtr);
  int i;
  jsize len = (*env)->GetArrayLength(env, vr);

  fmi2ValueReference *vr_arr = createValueRefArray(env, vr, len);

  DECLARE_ARRAY(fmi2String, r, len);

  copyJniArrayToNativeString(env, values, r, len);

  fmi2Status status = getFmuPtr(fmuPtr)->setString(c, vr_arr, len, r);

  for (i = 0; i < len; i++) {
    free((char *)r[i]);
  }

  free(r);
  free(vr_arr);

  return status;
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nDoStep(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr,
    jdouble currentCommunicationPoint, jdouble communicationStepSize,
    jboolean noSetFMUStatePriorToCurrentPoint) {
  fmi2Component comp = getCompPtr(componentPtr);
  return getFmuPtr(fmuPtr)->doStep(comp, currentCommunicationPoint,
                                   communicationStepSize,
                                   noSetFMUStatePriorToCurrentPoint);
}

/*JNIEXPORT jbyte JNICALL
 Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nCancelStep(JNIEnv *env,
 jobject obj,
 jlong fmuPtr, jlong componentPtr) {
 fmi2Component comp = (fmi2Component) componentPtr;
 return ((FMU*) fmuPtr)->cancelStep(comp);
 }*/

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nGetStatus(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr, jbyte kind,
    jbyteArray value) {
  fmi2Component comp = getCompPtr(componentPtr);
  fmi2Status status;
  fmi2Status res =
      getFmuPtr(fmuPtr)->getStatus(comp, (fmi2StatusKind)kind, &status);

  jbyte *vbody = (*env)->GetByteArrayElements(env, value, 0);
  vbody[0] = status;
  (*env)->ReleaseByteArrayElements(env, value, vbody, 0);

  return res;
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nGetRealStatus(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr, jbyte kind,
    jdoubleArray value) {
  fmi2Component comp = getCompPtr(componentPtr);
  fmi2Real status;
  fmi2Status res =
      getFmuPtr(fmuPtr)->getRealStatus(comp, (fmi2StatusKind)kind, &status);

  copyNativeArrayToJniDouble(env, &status, value, 1);

  return res;
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nGetIntegerStatus(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr, jbyte kind,
    jintArray value) {
  fmi2Component comp = getCompPtr(componentPtr);
  fmi2Integer status;
  fmi2Status res =
      getFmuPtr(fmuPtr)->getIntegerStatus(comp, (fmi2StatusKind)kind, &status);

  copyNativeArrayToJniInteger(env, &status, value, 1);

  return res;
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nGetBooleanStatus(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr, jbyte kind,
    jbooleanArray value) {
  fmi2Component comp = getCompPtr(componentPtr);
  fmi2Boolean status;
  fmi2Status res =
      getFmuPtr(fmuPtr)->getBooleanStatus(comp, (fmi2StatusKind)kind, &status);

  copyNativeArrayToJniBoolean(env, &status, value, 1);

  return res;
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nGetStringStatus(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr, jbyte kind,
    jobjectArray value) {
  fmi2Component comp = getCompPtr(componentPtr);
  fmi2String status;
  fmi2Status res =
      getFmuPtr(fmuPtr)->getStringStatus(comp, (fmi2StatusKind)kind, &status);

  (*env)
      ->SetObjectArrayElement(env, value, 0, (*env)->NewStringUTF(env, status));

  return res;
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nSetRealInputDerivatives(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr, jlongArray vr,
    jlong size, jintArray order, jdoubleArray values) {
  fmi2Component c = getCompPtr(componentPtr);

  jsize len = (*env)->GetArrayLength(env, vr);

  fmi2ValueReference *vr_arr = createValueRefArray(env, vr, len);

  DECLARE_ARRAY(fmi2Real, r, len);
  copyJniArrayToNativeDouble(env, values, r, len);

  jsize lenOrder = (*env)->GetArrayLength(env, order);
  DECLARE_ARRAY(fmi2Integer, orderArr, lenOrder);
  copyJniArrayToNativeInteger(env, order, orderArr, lenOrder);

  fmi2Status status =
      getFmuPtr(fmuPtr)->setRealInputDerivatives(c, vr_arr, len, orderArr, r);

  free(r);
  free(orderArr);
  free(vr_arr);

  return status;
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nGetRealOutputDerivatives(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr, jlongArray vr,
    jlong size, jintArray order, jdoubleArray values) {
  fmi2Component c = getCompPtr(componentPtr);

  jsize len = (*env)->GetArrayLength(env, vr);

  fmi2ValueReference *vr_arr = createValueRefArray(env, vr, len);

  DECLARE_ARRAY(fmi2Real, r, len);

  jsize lenOrder = (*env)->GetArrayLength(env, order);
  DECLARE_ARRAY(fmi2Integer, orderArr, lenOrder);
  copyJniArrayToNativeInteger(env, order, orderArr, lenOrder);

  fmi2Status status =
      getFmuPtr(fmuPtr)->getRealOutputDerivatives(c, vr_arr, len, orderArr, r);

  copyNativeArrayToJniDouble(env, r, values, len);

  free(r);
  free(vr_arr);

  return status;
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nGetDirectionalDerivative(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr,
    jlongArray vUnknownRef, jlong nUnknownSize, jlongArray vKnownRef,
    jlong nKnownSize, jdoubleArray dvKnown, jdoubleArray dvUnknown) {
  fmi2Component c = getCompPtr(componentPtr);

  fmi2ValueReference *vUnknown_ref =
      createValueRefArray(env, vUnknownRef, nUnknownSize);

  fmi2ValueReference *vKnown_ref =
      createValueRefArray(env, vKnownRef, nKnownSize);

  jsize len = (*env)->GetArrayLength(env, dvKnown);
  DECLARE_ARRAY(fmi2Real, dvKnownRef, len);

  copyJniArrayToNativeDouble(env, dvKnown, dvKnownRef, len);

  // dvUnknown has size nKnownSize
  DECLARE_ARRAY(fmi2Real, r, nKnownSize);

  fmi2Status status = getFmuPtr(fmuPtr)->getDirectionalDerivative(
      c, vUnknown_ref, nUnknownSize, vKnown_ref, nKnownSize, dvKnownRef, r);

  copyNativeArrayToJniDouble(env, r, dvUnknown, len);

  free(r);
  free(dvKnownRef);
  free(vUnknown_ref);
  free(vKnown_ref);

  return status;
}

JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nGetFmuState(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr,
    jlongArray statePtrArr) {
  fmi2Component c = getCompPtr(componentPtr);

  fmi2FMUstate state;
  state = NULL;
  fmi2Status res = getFmuPtr(fmuPtr)->getFMUstate(c, &state);

  jlong *vbody = (*env)->GetLongArrayElements(env, statePtrArr, 0);
  vbody[0] = (jlong)(state);

  (*env)->ReleaseLongArrayElements(env, statePtrArr, vbody, 0);

  return res;
}

/*
 * Class:     org_intocps_fmi_jnifmuapi_NativeFmuComponent
 * Method:    nSetFmuState
 * Signature: (JJJ)B
 */
JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nSetFmuState(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr,
    jlong statePtr) {
  fmi2Component c = getCompPtr(componentPtr);

  fmi2FMUstate state = ((fmi2FMUstate)statePtr);
  fmi2Status res = getFmuPtr(fmuPtr)->setFMUstate(c, state);

  return res;
}

//
///*
// * Class:     org_intocps_fmi_jnifmuapi_NativeFmuComponent
// * Method:    nFreeFmuState
// * Signature: (JJJ)B
// */
JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nFreeFmuState(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr,
    jlong statePtr) {
  fmi2Component c = getCompPtr(componentPtr);

  fmi2FMUstate state = ((fmi2FMUstate)statePtr);
  fmi2Status res = getFmuPtr(fmuPtr)->freeFMUstate(c, &state);

  return res;
}

// INTO CPS
JNIEXPORT jbyte JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmuComponent_nGetMaxStepsize(
    JNIEnv *env, jobject obj, jlong fmuPtr, jlong componentPtr,
    jdoubleArray maxsize) {
  fmi2Status res;

  if (getFmuPtr(fmuPtr)->getMaxStepsize == NULL) {
    res = fmi2Error;
  } else {
    fmi2Component c = getCompPtr(componentPtr);

    DECLARE_ARRAY(fmi2Real, r, 1);

    res = getFmuPtr(fmuPtr)->getMaxStepsize(c, r);

    copyNativeArrayToJniDouble(env, r, maxsize, 1);

    free(r);
  }

  return res;
}
