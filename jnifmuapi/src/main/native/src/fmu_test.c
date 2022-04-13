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

#include "fmi2.h"
#include "fmu_api.h"
#include "jni_util.h"
#include "org_intocps_fmi_jnifmuapi_NativeFmu.h"

JNIEXPORT void JNICALL Java_org_intocps_fmi_jnifmuapi_NativeFmu_callbackTest(
    JNIEnv *env, jobject obj, jlong fmuPtr, jstring name) {
  const char *instanceName = GetString(env, name);
  fmuLoggerCache((FMU *)fmuPtr, instanceName, fmi2Warning, NULL, NULL);
  (*env)->ReleaseStringUTFChars(env, name, instanceName);
}

JNIEXPORT void JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmu_throwExceptionTest(JNIEnv *env,
                                                            jobject obj) {
  throwException(env, "this is a test exception throw");
}
