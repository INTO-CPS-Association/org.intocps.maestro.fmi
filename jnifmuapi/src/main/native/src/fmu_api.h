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

#ifndef SRC_MAIN_NATIVE_SRC_FMUAPI_H_
#define SRC_MAIN_NATIVE_SRC_FMUAPI_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "comp_data.h"
#include "fmi2.h"
#include "jni_util.h"
#include "sim_support.h"

#define MAX_MSG_SIZE 2000

#define FMU_CALLBACK_METHOD_ID \
  "(Ljava/lang/String;BLjava/lang/String;Ljava/lang/String;)V"

void fmuLoggerCache(void *componentEnvironment, fmi2String instanceName,
                    fmi2Status status, fmi2String category, fmi2String message,
                    ...);

//jint throwException(JNIEnv *env, char *message);

FMU *getFmuPtr(jlong fmuPtr);

#endif /* SRC_MAIN_NATIVE_SRC_FMUAPI_H_ */
