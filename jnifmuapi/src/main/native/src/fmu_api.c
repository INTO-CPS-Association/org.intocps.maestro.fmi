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
#include "fmu_api.h"

#include "org_intocps_fmi_jnifmuapi_NativeFmu.h"
#include "config.h"

JNIEXPORT jstring JNICALL Java_org_intocps_fmi_jnifmuapi_NativeFmu_nGetJniApiVersion(JNIEnv *env,
                                                          jclass cl) {
    jstring result = (*env)->NewStringUTF(env, PROJECT_VER);
    return result;
}

/********************************************************
 * Utils
 ********************************************************/

FMU *getFmuPtr(jlong fmuPtr) { return (FMU *)fmuPtr; }

JNIEXPORT jlong JNICALL Java_org_intocps_fmi_jnifmuapi_NativeFmu_nLoadLibrary(
    JNIEnv *env, jobject obj, jstring libraryPath) {
  const char *path;

  path = (*env)->GetStringUTFChars(env, libraryPath, NULL);

  // load the FMU dll

  FMU *fmu = (FMU *)malloc(sizeof(FMU));

  if (fmu == NULL) {
    throwException(env, "malloc for fmu failed");
  }

  if (!loadDll(path, fmu)) {
    throwException(env, "Load failed!!!");
    return 0;
  }

  (*env)->ReleaseStringUTFChars(env, libraryPath, path);

  return (jlong)fmu;
}

JNIEXPORT void JNICALL Java_org_intocps_fmi_jnifmuapi_NativeFmu_nUnLoad(
    JNIEnv *env, jobject obj, jlong fmuptr) {
  FMU *ptr = getFmuPtr(fmuptr);
#ifdef _WIN32
  FreeLibrary(ptr->dllHandle);
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_MAC
  // Other kinds of Mac OS
  dlclose(ptr->dllHandle);
#else
  throwException(env, "Unsupported platform");
#endif
#elif __linux
  dlclose(ptr->dllHandle);
#endif

  free(ptr);
}

/********************************************************
 * FMU
 ********************************************************/

JNIEXPORT jstring JNICALL Java_org_intocps_fmi_jnifmuapi_NativeFmu_nGetVersion(
    JNIEnv *env, jobject obj, jlong fmuPtr) {
  jstring result = (*env)->NewStringUTF(env, getFmuPtr(fmuPtr)->getVersion());
  return result;
}

JNIEXPORT jstring JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmu_nGetTypesPlatform(JNIEnv *env,
                                                           jobject obj,
                                                           jlong fmuPtr) {
  jstring result =
      (*env)->NewStringUTF(env, getFmuPtr(fmuPtr)->getTypesPlatform());
  return result;
}

HashmapNode *configureCallback(JNIEnv *env, jobject obj, jlong fmuPtr,
                               jobject callbackObj, const char *instanceName) {
  // convert local to global reference
  // (local will die after this method call)
  jobject g_obj = (*env)->NewGlobalRef(env, callbackObj);

  HashmapNode *cbn = (HashmapNode *)malloc(sizeof(HashmapNode));

  cbn->callbackObj = g_obj;
  cbn->fmu = (FMU *)fmuPtr;
  JavaVM *g_vm;
  (*env)->GetJavaVM(env, &g_vm);
  cbn->g_vm = g_vm;
  cbn->env = env;
  cbn->name = instanceName;

  // save refs for callback
  jclass g_clazz = (*env)->GetObjectClass(env, g_obj);
  if (g_clazz == NULL) {
    throwException(env, "Failed to find class");
  }

  jmethodID method = (*env)->GetMethodID(env, g_clazz, "log",
                                         FMU_CALLBACK_METHOD_ID);  //"(I)V");

  cbn->callbackMethod = method;

  if (cbn->callbackMethod == NULL) {
    throwException(env, "Unable to get method ref");
  }

  storeCallbackHandler(cbn);
  // g_callbackMap[getFreeCallbackMapIndex()] = cbn;
  return cbn;
}

JNIEXPORT jlong JNICALL Java_org_intocps_fmi_jnifmuapi_NativeFmu_ninstantiate(
    JNIEnv *env, jobject obj, jlong fmuPtr, jstring name, jstring jguid,
    jstring resourceLocation, jboolean visible, jboolean loggingOn,
    jobject callbackObj) {
  const char *instanceName = GetString(env, name);
  const char *fmuResourceLocation = GetString(env, resourceLocation);
  const char *guid = GetString(env, jguid);

  CompHashmapNode *compNode =
      (CompHashmapNode *)malloc(sizeof(CompHashmapNode));

  if (compNode == NULL) {
    throwException(env, "malloc of compNode failed");
  }

  if (callbackObj != NULL) {
    int size = strlen(instanceName);

    const char *cname = strcpy(malloc(sizeof(char) * (size + 1)), instanceName);

    if (cname == NULL) {
      throwException(env, "malloc of cname failed");
    }

    compNode->callback =
        configureCallback(env, obj, fmuPtr, callbackObj, cname);
  }
  fmi2CallbackFunctions *callbacks =
      (fmi2CallbackFunctions *)malloc(sizeof(fmi2CallbackFunctions));

  if (callbacks == NULL) {
    throwException(env, "malloc of callbacks failed");
  }

  fmi2CallbackFunctions cb = {
      callbackObj != NULL ? fmuLoggerCache : fmuLogger, calloc, free, NULL,
      (FMU *)fmuPtr};  // called by the model during simulation

  memcpy(callbacks, &cb, sizeof(fmi2CallbackFunctions));
  compNode->callbackFunctions = callbacks;

  if ((*env)->ExceptionCheck(env)) {
    printf("exception\n");
    (*env)->ExceptionDescribe(env);
  }

  fmi2Component c =
      ((FMU *)fmuPtr)
          ->instantiate(instanceName, fmi2CoSimulation, guid,
                        fmuResourceLocation, callbacks, visible, loggingOn);

  if (c != NULL) {
    compNode->comp = c;
    // store for later free
    storeComponent(compNode);
  } else {
    free((void*)compNode->callback->name);
    free(compNode->callbackFunctions);
    compNode->callbackFunctions = NULL;
    free(compNode);
    compNode = NULL;
  }

  if ((*env)->ExceptionCheck(env)) {
    throwException(env, "exception - 2\n");
    (*env)->ExceptionDescribe(env);
  }

  (*env)->ReleaseStringUTFChars(env, name, instanceName);
  (*env)->ReleaseStringUTFChars(env, resourceLocation, fmuResourceLocation);
  (*env)->ReleaseStringUTFChars(env, jguid, guid);

  return (jlong)c;
}

void fmuLoggerCache(void *componentEnvironment, fmi2String instanceName,
                    fmi2Status status, fmi2String category, fmi2String message,
                    ...) {
  size_t n = MAX_MSG_SIZE;

  char msg[MAX_MSG_SIZE];
  char *copy;
  va_list argp;

  if (message == NULL) {
    message = "NULL";
  }
  // replace C format strings
  va_start(argp, message);
  int truncation = vsnprintf(msg, MAX_MSG_SIZE, message, argp);
  va_end(argp);

  int i = 0;

  int reported = 0;

  HashmapNode *callBackNode = getCallbackHandler(instanceName);

  if (callBackNode != NULL) {
    reported = 1;
    JNIEnv *env = callBackNode->env;
    JavaVM *vm = callBackNode->g_vm;

    JNIEnv *g_env;
    // double check it's all ok

    int getEnvStat = (*vm)->GetEnv(vm, (void **)&g_env, JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
      // printf("GetEnv: not attached\n");
      if ((*vm)->AttachCurrentThread(vm, (void **)&g_env, NULL) != 0) {
        printf("Failed to attach\n");
      }
    } else if (getEnvStat == JNI_OK) {
    } else if (getEnvStat == JNI_EVERSION) {
      printf("GetEnv: version not supported\n");
    } else if (getEnvStat == JNI_ERR) {
      printf("GetEnv: error\n");
    } else {
      printf("GetEnv: %i\n", getEnvStat);
    }

    env = g_env;

    if ((*env)->ExceptionCheck(env)) {
      printf("exception\n");
      (*env)->ExceptionDescribe(env);
    }
    jstring jinstanceName = (*env)->NewStringUTF(env, instanceName);

    if ((*env)->ExceptionCheck(env)) {
      printf("exception\n");
      (*env)->ExceptionDescribe(env);
    }

    jstring jcategory = (*env)->NewStringUTF(env, category);
    jstring jmessage = (*env)->NewStringUTF(env, msg);

    // call callback
    (*env)->CallVoidMethod(env, callBackNode->callbackObj,
                           callBackNode->callbackMethod, jinstanceName, status,
                           jcategory, jmessage);
    if ((*env)->ExceptionCheck(env)) {
      (*env)->ExceptionDescribe(env);
    }

    (*env)->DeleteLocalRef(env, jinstanceName);
    (*env)->DeleteLocalRef(env, jcategory);
    (*env)->DeleteLocalRef(env, jmessage);

    if ((*env)->ExceptionCheck(env)) {
      (*env)->ExceptionDescribe(env);
    }

    (*vm)->DetachCurrentThread(vm);
  }

  if (reported == 0) {
    // no receiver found
    printf("Unable to find receiver for %s reporting: %s", instanceName, msg);
  }
}
