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
    jstring result = (*env)->NewStringUTF(env, fmi2Version);
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
  storeFmu(fmu);

  return (jlong)fmu;
}

JNIEXPORT void JNICALL Java_org_intocps_fmi_jnifmuapi_NativeFmu_nUnLoad(
    JNIEnv *env, jobject obj, jlong fmuptr) {
  FMU *ptr = getFmuPtr(fmuptr);
  if (!isFmuLoaded(ptr)) {
    throwException(env, "Cannot unload an invalid or unloaded FMU");
    return;
  }
  if (hasFmuComponents(ptr)) {
    throwException(env, "Free all FMU instances before unloading the library");
    return;
  }
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

  removeFmu(ptr);
  free(ptr);
}

/********************************************************
 * FMU
 ********************************************************/

JNIEXPORT jstring JNICALL Java_org_intocps_fmi_jnifmuapi_NativeFmu_nGetVersion(
    JNIEnv *env, jobject obj, jlong fmuPtr) {
  if (!isFmuLoaded(getFmuPtr(fmuPtr))) {
    throwException(env, "Cannot query an invalid or unloaded FMU");
    return NULL;
  }
  jstring result = (*env)->NewStringUTF(env, getFmuPtr(fmuPtr)->getVersion());
  return result;
}

JNIEXPORT jstring JNICALL
Java_org_intocps_fmi_jnifmuapi_NativeFmu_nGetTypesPlatform(JNIEnv *env,
                                                           jobject obj,
                                                           jlong fmuPtr) {
  if (!isFmuLoaded(getFmuPtr(fmuPtr))) {
    throwException(env, "Cannot query an invalid or unloaded FMU");
    return NULL;
  }
  jstring result =
      (*env)->NewStringUTF(env, getFmuPtr(fmuPtr)->getTypesPlatform());
  return result;
}

static HashmapNode *configureCallback(JNIEnv *env, FMU *fmu, jobject callbackObj,
                                      const char *instanceName) {
  HashmapNode *callback = (HashmapNode *)calloc(1, sizeof(HashmapNode));
  if (callback == NULL) {
    throwException(env, "Unable to allocate FMU callback");
    return NULL;
  }
  callback->fmu = fmu;
  callback->name = strdup(instanceName);
  if (callback->name == NULL) {
    free(callback);
    throwException(env, "Unable to allocate FMU callback name");
    return NULL;
  }
  callback->callbackObj = (*env)->NewGlobalRef(env, callbackObj);
  if (callback->callbackObj == NULL) {
    freeCallbackHandler(env, callback);
    return NULL;
  }
  jclass clazz = (*env)->GetObjectClass(env, callbackObj);
  if (clazz != NULL) {
    callback->callbackMethod = (*env)->GetMethodID(env, clazz, "log", FMU_CALLBACK_METHOD_ID);
    (*env)->DeleteLocalRef(env, clazz);
  }
  if (callback->callbackMethod == NULL) {
    freeCallbackHandler(env, callback);
    return NULL;
  }
  if ((*env)->GetJavaVM(env, &callback->g_vm) != JNI_OK) {
    freeCallbackHandler(env, callback);
    throwException(env, "Unable to obtain Java VM for FMU callback");
    return NULL;
  }
  storeCallbackHandler(callback);
  return callback;
}

JNIEXPORT jlong JNICALL Java_org_intocps_fmi_jnifmuapi_NativeFmu_ninstantiate(
    JNIEnv *env, jobject obj, jlong fmuPtr, jstring name, jstring jguid,
    jstring resourceLocation, jboolean visible, jboolean loggingOn,
    jobject callbackObj) {
  FMU *fmu = getFmuPtr(fmuPtr);
  if (!isFmuLoaded(fmu)) {
    throwException(env, "Cannot instantiate an invalid or unloaded FMU");
    return 0;
  }
  if (name == NULL || jguid == NULL || resourceLocation == NULL) {
    throwException(env, "Instance name, GUID and resource location must not be null");
    return 0;
  }
  if (!beginFmuInstantiation(fmu)) {
    throwException(env, "Cannot instantiate an unloaded FMU");
    return 0;
  }
  const char *instanceName = NULL;
  const char *fmuResourceLocation = NULL;
  const char *guid = NULL;
  fmi2Component component = NULL;
  CompHashmapNode *node = (CompHashmapNode *)calloc(1, sizeof(CompHashmapNode));
  if (node == NULL) {
    throwException(env, "Unable to allocate FMU instance");
    endFmuInstantiation(fmu);
    return 0;
  }
  node->fmu = fmu;
  instanceName = GetString(env, name);
  if (instanceName == NULL) goto cleanup;
  guid = GetString(env, jguid);
  if (guid == NULL) goto cleanup;
  fmuResourceLocation = GetString(env, resourceLocation);
  if (fmuResourceLocation == NULL) goto cleanup;
  if (instanceName[0] == '\0' || guid[0] == '\0') {
    throwException(env, "Instance name and GUID must not be empty");
    goto cleanup;
  }
  if (callbackObj != NULL) {
    node->callback = configureCallback(env, fmu, callbackObj, instanceName);
    if (node->callback == NULL) goto cleanup;
  }
  node->callbackFunctions = (fmi2CallbackFunctions *)calloc(1, sizeof(fmi2CallbackFunctions));
  if (node->callbackFunctions == NULL) {
    throwException(env, "Unable to allocate FMU callback functions");
    goto cleanup;
  }
  node->callbackFunctions->logger = callbackObj != NULL ? fmuLoggerCache : fmuLogger;
  node->callbackFunctions->allocateMemory = calloc;
  node->callbackFunctions->freeMemory = free;
  node->callbackFunctions->componentEnvironment = node->callback;
  component = fmu->instantiate(instanceName, fmi2CoSimulation, guid,
                               fmuResourceLocation, node->callbackFunctions, visible, loggingOn);
  if ((*env)->ExceptionCheck(env)) {
    if (component != NULL) fmu->freeInstance(component);
    component = NULL;
  }
  if (component != NULL) {
    node->comp = component;
    storeComponent(node);
    node = NULL;
  }

cleanup:
  freeComponentNode(env, node);
  endFmuInstantiation(fmu);
  if (instanceName != NULL) (*env)->ReleaseStringUTFChars(env, name, instanceName);
  if (fmuResourceLocation != NULL) (*env)->ReleaseStringUTFChars(env, resourceLocation, fmuResourceLocation);
  if (guid != NULL) (*env)->ReleaseStringUTFChars(env, jguid, guid);
  return (jlong)component;
}

void fmuLoggerCache(void *componentEnvironment, fmi2String instanceName,
                    fmi2Status status, fmi2String category, fmi2String message,
                    ...) {
  char msg[MAX_MSG_SIZE];
  va_list argp;

  if (message == NULL) {
    message = "NULL";
  }
  // replace C format strings
  va_start(argp, message);
  vsnprintf(msg, MAX_MSG_SIZE, message, argp);
  va_end(argp);

  JavaVM *vm = getCallbackVm(componentEnvironment);
  if (vm == NULL) {
    fprintf(stderr, "No FMU callback receiver for %s: %s\n", instanceName ? instanceName : "?", msg);
    return;
  }
  JNIEnv *env = NULL;
  int state = (*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_6);
  int attached = state == JNI_EDETACHED;
  if (attached) state = (*vm)->AttachCurrentThread(vm, (void **)&env, NULL);
  if (state != JNI_OK || env == NULL) {
    fprintf(stderr, "Unable to attach FMU callback thread to Java VM: %d\n", state);
    return;
  }
  if (!(*env)->ExceptionCheck(env) && (*env)->PushLocalFrame(env, 4) == JNI_OK) {
    jmethodID method = NULL;
    jobject receiver = getCallbackReference(env, componentEnvironment, &method);
    if (receiver != NULL) {
      jstring jname = (*env)->NewStringUTF(env, instanceName ? instanceName : "");
      jstring jcategory = NULL;
      jstring jmessage = NULL;
      if (jname != NULL) jcategory = (*env)->NewStringUTF(env, category ? category : "");
      if (jcategory != NULL) jmessage = (*env)->NewStringUTF(env, msg);
      if (jmessage != NULL) {
        (*env)->CallVoidMethod(env, receiver, method, jname, (jbyte)status, jcategory, jmessage);
      }
    } else if (!(*env)->ExceptionCheck(env)) {
      fprintf(stderr, "FMU callback receiver was released: %s\n", msg);
    }
    (*env)->PopLocalFrame(env, NULL);
  }
  if (attached) {
    if ((*env)->ExceptionCheck(env)) (*env)->ExceptionDescribe(env);
    (*vm)->DetachCurrentThread(vm);
  }
}
