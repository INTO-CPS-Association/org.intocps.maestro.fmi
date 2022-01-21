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
 * from the URLs: http://www.into-cps.org, and in the INTO-CPS toolchain
 * distribution. GNU version 3 is obtained from:
 * http://www.gnu.org/copyleft/gpl.html.
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

#include "jni_util.h"

jint throwException(JNIEnv *env,const char *message) {
  jclass exClass;
  char *className = "java/lang/Exception";

  exClass = (*env)->FindClass(env, className);
  if (exClass == NULL) {
    return -1;
  }

  return (*env)->ThrowNew(env, exClass, message);
}

jint throwException3(JNIEnv *env,const char *message) {
    jclass exClass;
    char *className = "org/intocps/fmi/jnifmuapi/fmi3/NativeFmu3Exception";

    exClass = (*env)->FindClass(env, className);
    if (exClass == NULL) {
        return -1;
    }

    return (*env)->ThrowNew(env, exClass, message);
}

const char *GetString(JNIEnv *env, jstring libraryPath) {
  return (*env)->GetStringUTFChars(env, libraryPath, NULL);
}

/**
 * Util method to copy data between native array and JNI array of type double
 */
void copyNativeArrayToJniDouble(JNIEnv *env, fmi2Real *source,
                                jdoubleArray target, jsize len) {
  jdouble *vbody = (*env)->GetDoubleArrayElements(env, target, 0);

  int i;
  for (i = 0; i < len; i++) {
    vbody[i] = source[i];
  }

  (*env)->ReleaseDoubleArrayElements(env, target, vbody, 0);
}

/**
 * Util method to copy data between native array and JNI array of type int
 */
void copyNativeArrayToJniInteger(JNIEnv *env, fmi2Integer *source,
                                 jintArray target, jsize len) {
  int i = 0;
  jint *vbody = (*env)->GetIntArrayElements(env, target, 0);
  for (i = 0; i < len; i++) {
    vbody[i] = source[i];
  }

  (*env)->ReleaseIntArrayElements(env, target, vbody, 0);
}

/**
 * Util method to copy data between native array and JNI array of type bool
 */
void copyNativeArrayToJniBoolean(JNIEnv *env, fmi2Boolean *source,
                                 jbooleanArray target, jsize len) {
  int i = 0;
  jboolean *vbody = (*env)->GetBooleanArrayElements(env, target, 0);
  for (i = 0; i < len; i++) {
    vbody[i] = source[i];
  }

  (*env)->ReleaseBooleanArrayElements(env, target, vbody, 0);
}

/**
 * Util method to copy data between native array and JNI array of type string
 */
void copyNativeArrayToJniString(JNIEnv *env, fmi2String *source,
                                jobjectArray target, jsize len) {
  int i = 0;
  for (i = 0; i < len; i++) {
    fmi2String sVal = source[i];
    jstring jsVal = (*env)->NewStringUTF(env, sVal);
    (*env)->SetObjectArrayElement(env, target, i, jsVal);
  }
}

//############## JNI to Native ############

void copyJniArrayToNativeDouble(JNIEnv *env, jdoubleArray source,
                                fmi2Real *target, jsize len) {
  int i = 0;

  jdouble *vbody = (*env)->GetDoubleArrayElements(env, source, 0);
  for (i = 0; i < len; i++) {
    target[i] = vbody[i];
  }
  (*env)->ReleaseDoubleArrayElements(env, source, vbody, 0);
}

void copyJniArrayToNativeInteger(JNIEnv *env, jintArray source,
                                 fmi2Integer *target, jsize len) {
  int i = 0;
  jint *vbody = (*env)->GetIntArrayElements(env, source, 0);
  for (i = 0; i < len; i++) {
    target[i] = vbody[i];
  }
  (*env)->ReleaseIntArrayElements(env, source, vbody, 0);
}

void copyJniArrayToNativeBoolean(JNIEnv *env, jbooleanArray source,
                                 fmi2Boolean *target, jsize len) {
  int i = 0;
  jboolean *vbody = (*env)->GetBooleanArrayElements(env, source, 0);
  for (i = 0; i < len; i++) {
    target[i] = vbody[i];
  }
  (*env)->ReleaseBooleanArrayElements(env, source, vbody, 0);
}

void copyJniArrayToNativeString(JNIEnv *env, jobjectArray source,
                                fmi2String *target, jsize len) {
  int i = 0;
  for (i = 0; i < len; i++) {
    jstring string = (*env)->GetObjectArrayElement(env, source, i);
    jboolean blnIsCopy;
    const char *myarray = (*env)->GetStringUTFChars(env, string, &blnIsCopy);

    if (myarray == NULL) {
      throwException(env, "JNI SetString failed to get string");
      return;
    }

    int slen = strlen(myarray) + 1;  // Add NULL termination

    char *tmp = (char *)malloc(sizeof(char) * slen);

    if (tmp == NULL) {
      throwException(env, "calloc tmp of char* failed");
    }

    strcpy(tmp, myarray);
    target[i] = tmp;

    (*env)->ReleaseStringUTFChars(env, string, myarray);
    (*env)->DeleteLocalRef(env, string);
  }
}
