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
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fmi2FunctionTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

jint throwException(JNIEnv *env,const char *message);
jint throwException3(JNIEnv *env,const char *message);

const char *GetString(JNIEnv *env, jstring libraryPath);

// Native to JNI Array

void copyNativeArrayToJniDouble(JNIEnv *env, fmi2Real *source,
                                jdoubleArray target, jsize len);

void copyNativeArrayToJniInteger(JNIEnv *env, fmi2Integer *source,
                                 jintArray target, jsize len);

void copyNativeArrayToJniBoolean(JNIEnv *env, fmi2Boolean *source,
                                 jbooleanArray target, jsize len);

void copyNativeArrayToJniString(JNIEnv *env, fmi2String *source,
                                jobjectArray target, jsize len);

// JNI Array to Native
void copyJniArrayToNativeDouble(JNIEnv *env, jdoubleArray source,
                                fmi2Real *target, jsize len);

void copyJniArrayToNativeInteger(JNIEnv *env, jintArray source,
                                 fmi2Integer *target, jsize len);

void copyJniArrayToNativeBoolean(JNIEnv *env, jbooleanArray source,
                                 fmi2Boolean *target, jsize len);

void copyJniArrayToNativeString(JNIEnv *env, jobjectArray source,
                                fmi2String *target, jsize len);

#ifdef __cplusplus
}
#endif