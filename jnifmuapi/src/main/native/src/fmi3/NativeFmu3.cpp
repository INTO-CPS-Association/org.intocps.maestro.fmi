//
// Created by Kenneth Guldbrandt Lausdahl on 17/12/2020.
//

#include "Fmi3Manager.h"
#include "config.h"
#include "org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3.h"

extern "C"
{
#include "sim_support3.h"
#include "../jni_util.h"
}

#include "Fmi3PreemptExpansion.h"
#include <cstdio>
/********************************************************
 * Utils
 ********************************************************/
/*
* Class:     org_intocps_fmi3_jnifmuapi_NativeFmu3
* Method:    getJniApiVersion
* Signature: ()J
*/
__attribute__((unused)) JNIEXPORT jstring JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3_nGetJniApiFmiVersion
        (JNIEnv *env, jclass) {
    jstring result = env->NewStringUTF(fmi3Version);
    return result;
}


__attribute__((unused)) JNIEXPORT jlong JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3_nLoadLibrary(
        JNIEnv *env, __attribute__((unused)) jobject obj, jstring libraryPath) {
    const char *path;

    path = env->GetStringUTFChars(libraryPath, NULL);

    // load the FMU dll
    auto node = new Fmi3Node();
    node->name = path;

    char **errorMessage = (char **) malloc(sizeof(char **));
    *errorMessage = nullptr;

    if (!loadDll3(path, &node->fmu, errorMessage)) {
        const char *beginMsg = "Load failed!. Missing functions: [";
        char *msg = (char *) malloc(strlen(beginMsg) + 1 + strlen(*errorMessage));
        strcat(msg, beginMsg);
        strcat(msg, *errorMessage);
        strcat(msg, "]");
        throwException(env, msg);
        free(msg);
        return 0;
    }

    env->ReleaseStringUTFChars(libraryPath, path);
    return (jlong) node;
}

__attribute__((unused)) JNIEXPORT void JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3_nUnLoad(
        __attribute__((unused)) JNIEnv *env, __attribute__((unused)) jobject obj, jlong fmuptr) {
    Fmi3Node *ptr = getFmuNodePtr(fmuptr);
#ifdef _WIN32
    FreeLibrary(ptr->fmu.dllHandle);
#elif __APPLE__
//extern "C"
//{
//#include "TargetConditionals.h"
//    }
#if TARGET_OS_MAC
    // Other kinds of Mac OS
    dlclose(ptr->fmu.dllHandle);
#else
    throwException(env, "Unsupported platform");
#endif
#elif __linux
    dlclose(ptr->fmu.dllHandle);
#endif

    delete (ptr);
}


bool checkJenvCallback(JavaVM *vm, JNIEnv *env) {

    int getEnvStat = vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        // printf("GetEnv: not attached\n");
        if (vm->AttachCurrentThread((void **) &env, nullptr) != 0) {
            printf("Failed to attach\n");
        }
    } else if (getEnvStat == JNI_OK) {
    } else if (getEnvStat == JNI_EVERSION) {
        printf("GetEnv: version not supported\n");
        return false;
    } else if (getEnvStat == JNI_ERR) {
        printf("GetEnv: error\n");
        return false;
    } else {
        printf("GetEnv: %i\n", getEnvStat);
    }

    if (env->ExceptionCheck()) {
        printf("exception\n");
        env->ExceptionDescribe();
        return false;
    }

    return true;
}


void jniFmi3CallbackLogMessage(fmi3InstanceEnvironment instanceEnvironment,
                               fmi3Status status,
                               fmi3String category,
                               fmi3String message) {

    auto node = (Fmi3InstanceNode *) instanceEnvironment;
    auto cb = node->callback_logMessage;

    JNIEnv *env = cb.env;
    JavaVM *vm = cb.g_vm;

    if (cb.logMessage != nullptr && checkJenvCallback(vm, env)) {
        jstring jinstanceName = env->NewStringUTF(node->name.c_str());

        if (env->ExceptionCheck()) {
            printf("exception\n");
            env->ExceptionDescribe();
        }

        jstring jcategory = env->NewStringUTF(category);
        jstring jmessage = env->NewStringUTF(message);
        auto jstatus = convertStatus(env, status);
        // call callback

        env->CallVoidMethod(cb.callbackObj,
                            cb.callbackMethod, jinstanceName, jstatus,
                            jcategory, jmessage);
        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
        }

        env->DeleteLocalRef(jinstanceName);
        env->DeleteLocalRef(jcategory);
        env->DeleteLocalRef(jmessage);
        env->DeleteLocalRef(jstatus);

        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
        }

        vm->DetachCurrentThread();
    } else {
        // no receiver found
        printf("Unable to find receiver for %s reporting: %s\n", node->name.c_str(), message);
    }
}


void jniFmi3CallbackIntermediateUpdate(
        fmi3InstanceEnvironment instanceEnvironment,
        fmi3Float64 intermediateUpdateTime,
        fmi3Boolean intermediateVariableSetRequested,
        fmi3Boolean intermediateVariableGetAllowed,
        fmi3Boolean intermediateStepFinished,
        fmi3Boolean canReturnEarly,
        fmi3Boolean *earlyReturnRequested,
        fmi3Float64 *earlyReturnTime) {

    printf("API jniFmi3CallbackIntermediateUpdate\n");
    auto node = (Fmi3InstanceNode *) instanceEnvironment;
    auto cb = node->callback_intermediateUpdate;

    JNIEnv *env = cb.env;
    JavaVM *vm = cb.g_vm;

    if (cb.intermediateUpdate != nullptr && checkJenvCallback(vm, env)) {

        auto jearlyReturnRequested = env->NewBooleanArray(1);
        auto jearlyReturnTime = env->NewDoubleArray(1);

        // call callback
        jobject retObj = env->CallObjectMethod(cb.callbackObj,
                                               cb.callbackMethod, instanceEnvironment,
                                               intermediateUpdateTime,
                                               intermediateVariableSetRequested,
                                               intermediateVariableGetAllowed,
                                               intermediateStepFinished,
                                               canReturnEarly);

        //, jearlyReturnRequested, jearlyReturnTime
        if (retObj == nullptr) {
            *earlyReturnRequested = false;
            *earlyReturnTime = 0;
        } else {

            *earlyReturnRequested = env->CallBooleanMethod(retObj,
                                                           cb.intermediateUpdateUtilMethods.earlyReturnRequestedId);
            *earlyReturnTime = env->CallDoubleMethod(retObj, cb.intermediateUpdateUtilMethods.earlyReturnTimeMethodId);
        }
//        auto jearlyReturnRequestedBody = env->GetBooleanArrayElements(jearlyReturnRequested, JNI_FALSE);
//        *earlyReturnRequested = jearlyReturnRequestedBody[0];
//        env->ReleaseBooleanArrayElements(jearlyReturnRequested, jearlyReturnRequestedBody, 0);
//
//        auto jjearlyReturnTimeBody = env->GetDoubleArrayElements(jearlyReturnTime, JNI_FALSE);
//        *earlyReturnTime = jjearlyReturnTimeBody[0];
//        env->ReleaseDoubleArrayElements(jearlyReturnTime, jjearlyReturnTimeBody, 0);

        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
        }

        vm->DetachCurrentThread();
    } else {
        // no receiver found
        printf("Unable to find receiver for jniFmi3CallbackIntermediateUpdate\n");
    }

}


CallbackJniInfo createBasicCallback(JNIEnv *env, jobject obj, const char *methodName, const char *methodId) {
    CallbackJniInfo cb{};
    cb.callbackObj = env->NewGlobalRef(obj);
    auto g_clazz = env->GetObjectClass(cb.callbackObj);
    if (g_clazz == nullptr) {
        throwException3(env, "Failed to find class for callback object");
    }
    cb.callbackMethod = env->GetMethodID(g_clazz, methodName,
                                         methodId);  //"(I)V");

    if (cb.callbackMethod == nullptr) {
        string msg("Unable to get method ref ");
        msg.append(methodName);
        msg.append(" ");
        msg.append(methodId);
        throwException3(env, msg.c_str());
    }

    cb.env = env;
    env->GetJavaVM(&cb.g_vm);

    return cb;
}
/********************************************************
 * FMU
 ********************************************************/

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmu3
 * Method:    nGetVersion
 * Signature: ()Ljava/lang/String;
 */
__attribute__((unused)) JNIEXPORT jstring JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3_nGetVersion
        (JNIEnv *env, jobject, jlong fmuPtr) {
    return env->NewStringUTF(getFmuNodePtr(fmuPtr)->fmu.fmi3GetVersion());
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmu3
 * Method:    nInstantiateModelExchange
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ZZJLorg/intocps/fmi3/jnifmuapi/NativeFmu3/ICallbackLogMessage;)J
 */
__attribute__((unused)) JNIEXPORT jlong JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3_nInstantiateModelExchange
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong fmuPtr, jstring name, jstring instantiationToken,
         jstring resourceLocation, jboolean visible, jboolean loggingOn,
         jobject logMessage) {
//long fmuPtr, String instanceName, String instantiationToken, String resourceLocation,
//            boolean visible, boolean loggingOn, long instanceEnvironment, ILogMessageCallback logMessage
    auto fmuNode = getFmuNodePtr(fmuPtr);
    const char *instanceName = name == nullptr ? "no name" : GetString(env, name);
    const char *fmuResourceLocation = nullptr;
    if (resourceLocation != nullptr)
        fmuResourceLocation = GetString(env, resourceLocation);
    const char *token = instantiationToken == nullptr ? nullptr : GetString(env, instantiationToken);
    auto node = new Fmi3InstanceNode();
    node->type = ModelExchange;
    node->name = string(instanceName);
    node->owner = fmuNode;

    //callback: jobject logMessage
    if (logMessage != nullptr) {
        node->callback_logMessage = createBasicCallback(env, logMessage, "logMessage",
                                                        "(Ljava/lang/String;Lorg/intocps/fmi/jnifmuapi/fmi3/Fmi3Status;Ljava/lang/String;Ljava/lang/String;)V");
        node->callback_logMessage.logMessage = jniFmi3CallbackLogMessage;

    }

    if (env->ExceptionCheck()) {
        printf("exception\n");
        env->ExceptionDescribe();
    }

    auto instance = fmuNode->fmu.fmi3InstantiateModelExchange(instanceName, token, fmuResourceLocation, visible,
                                                              loggingOn, node,
                                                              node->callback_logMessage.logMessage);

    node->instance = instance;
    if (instance != nullptr) {
        Fmi3Manager::getInstance()->store(instance, node);
    } else {
        delete node;
        node = nullptr;
    }

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        throwException3(env, "exception - 2\n");
    }
    if (name != nullptr) {
        env->ReleaseStringUTFChars(name, instanceName);
    }
    if (fmuResourceLocation != nullptr) {
        env->ReleaseStringUTFChars(resourceLocation, fmuResourceLocation);
    }
    if (token != nullptr) {
        env->ReleaseStringUTFChars(instantiationToken, token);
    }

    return (jlong) node;
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmu3
 * Method:    nInstantiateCoSimulation
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ZZZ[JJJLorg/intocps/fmi3/jnifmuapi/NativeFmu3/ICallbackLogMessage;Lorg/intocps/fmi3/jnifmuapi/NativeFmu3/ICallbackIntermediateUpdate;)J
 */
__attribute__((unused)) JNIEXPORT jlong JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3_nInstantiateCoSimulation
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong fmuPtr, jstring name, jstring instantiationToken,
         jstring resourceLocation,
         jboolean visible, jboolean loggingOn, jboolean eventModeUsed, jboolean earlyReturnAllowed,
         jlongArray requiredIntermediateVariables,
         jlong nRequiredIntermediateVariables, jobject logMessage,
         jobject intermediateUpdate) {
    auto fmuNode = getFmuNodePtr(fmuPtr);
    const char *instanceName = name == nullptr ? "no name" : GetString(env, name);
    const char *fmuResourceLocation = nullptr;
    if (resourceLocation != nullptr)
        fmuResourceLocation = GetString(env, resourceLocation);
    const char *token = instantiationToken == nullptr ? nullptr : GetString(env, instantiationToken);
    auto node = new Fmi3InstanceNode();
    node->type = CoSimulation;
    node->name = string(instanceName);
    node->owner = fmuNode;


    //callback: jobject logMessage
    if (logMessage != nullptr) {
        node->callback_logMessage = createBasicCallback(env, logMessage, "logMessage",
                                                        "(Ljava/lang/String;Lorg/intocps/fmi/jnifmuapi/fmi3/Fmi3Status;Ljava/lang/String;Ljava/lang/String;)V");
        node->callback_logMessage.logMessage = jniFmi3CallbackLogMessage;

    } else {
        node->callback_logMessage.logMessage = nullptr;
    }
    //callback: jobject intermediateUpdate
    if (intermediateUpdate != nullptr) {

        node->callback_intermediateUpdate = createBasicCallback(env, intermediateUpdate, "intermediateUpdate",
                                                                "(JDZZZZZ)Lorg/intocps/fmi/jnifmuapi/fmi3/IIntermediateUpdateCallback$IntermediateUpdateResponse;");

        if (env->ExceptionCheck()) {
            printf("exception\n");
            env->ExceptionDescribe();
        }


        auto clazz = env->FindClass(
                "Lorg/intocps/fmi/jnifmuapi/fmi3/IIntermediateUpdateCallback$IntermediateUpdateResponse;");
        if (clazz == nullptr) {
            throwException3(env, "Failed to find class IIntermediateUpdateCallback$IntermediateUpdateResponse");
        }

        if (env->ExceptionCheck()) {
            printf("exception\n");
            env->ExceptionDescribe();
        }

        node->callback_intermediateUpdate.intermediateUpdateUtilMethods.earlyReturnTimeMethodId = env->GetMethodID(
                clazz, "getEarlyReturnTime", "()D");

        if (env->ExceptionCheck()) {
            printf("exception\n");
            env->ExceptionDescribe();
        }

        if (node->callback_intermediateUpdate.intermediateUpdateUtilMethods.earlyReturnTimeMethodId == nullptr) {
            throwException3(env, "Unable to get method ref getEarlyReturnTime");
        }

        node->callback_intermediateUpdate.intermediateUpdateUtilMethods.earlyReturnRequestedId = env->GetMethodID(clazz,
                                                                                                                  "isEarlyReturnRequested",
                                                                                                                  "()Z");

        if (node->callback_intermediateUpdate.intermediateUpdateUtilMethods.earlyReturnRequestedId == nullptr) {
            throwException3(env, "Unable to get method ref isEarlyReturnRequested");
        }

        //JFZZZZZZ[Z[
        //long instanceEnvironment, double intermediateUpdateTime, boolean clocksTicked,
        //                boolean intermediateVariableSetRequested, boolean intermediateVariableGetAllowed, boolean intermediateStepFinished,
        //                boolean canReturnEarly, boolean[] earlyReturnRequested, double[] earlyReturnTime

        node->callback_intermediateUpdate.intermediateUpdate = jniFmi3CallbackIntermediateUpdate;
    } else {
        node->callback_intermediateUpdate.intermediateUpdate = nullptr;
    }

    if (env->ExceptionCheck()) {
        printf("exception\n");
        env->ExceptionDescribe();
    }
    fmi3ValueReference *requiredIntermediateVariablesArr = nullptr;
    if (nRequiredIntermediateVariables > 0 && requiredIntermediateVariables != nullptr) {

        requiredIntermediateVariablesArr = createArray_uint_from_jlong(env, requiredIntermediateVariables,
                                                                       nRequiredIntermediateVariables);
    }
    auto instance = fmuNode->fmu.fmi3InstantiateCoSimulation(instanceName, token, fmuResourceLocation, visible,
                                                             loggingOn, eventModeUsed, earlyReturnAllowed,
                                                             requiredIntermediateVariablesArr,
                                                             nRequiredIntermediateVariables, node,
                                                             node->callback_logMessage.logMessage,
                                                             node->callback_intermediateUpdate.intermediateUpdate);
    node->instance = instance;
    delete requiredIntermediateVariablesArr;
    if (instance != nullptr) {
        Fmi3Manager::getInstance()->store(instance, node);
    } else {
        delete node;
        node = nullptr;
    }

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        throwException3(env, "exception - 2\n");
    }

    if (name != nullptr) {
        env->ReleaseStringUTFChars(name, instanceName);
    }
    if (fmuResourceLocation != nullptr) {
        env->ReleaseStringUTFChars(resourceLocation, fmuResourceLocation);
    }
    if (token != nullptr) {
        env->ReleaseStringUTFChars(instantiationToken, token);
    }
    return (jlong) node;
}

void jniPreemptLockCallback(Fmi3InstanceNode *handler, bool lock) {

    CallbackJniInfo cb = lock ? handler->callback_lockPreemption : handler->callback_unlockPreemption;

    JNIEnv *env = cb.env;
    JavaVM *vm = cb.g_vm;

    if (checkJenvCallback(vm, env)) {

        // call callback
        env->CallVoidMethod(cb.callbackObj,
                            cb.callbackMethod);

        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
        }

        vm->DetachCurrentThread();
    } else {
        // no receiver found
        printf("Unable to find receiver for jniPreemptLockCallback %d\n", lock);
    }
}


PreeStruct preemptionHandlers[PREEEMPTLOCKING_UNFOLD_MAX];

PREEEMPTLOCKING_UNFOLD

void initialize_preempt_locking() {
    PREEEMPTLOCKING_INIT_UNFOLD
}

PreeStruct *get_preemtp_lock_handler(Fmi3InstanceNode *handler) {
    for (auto &preemptionHandler: preemptionHandlers) {
        if (preemptionHandler.handler == nullptr) {
            preemptionHandler.handler = handler;
            return &preemptionHandler;
        }
    }
    return nullptr;
}

void jnifmi3ClockUpdateCallback(fmi3InstanceEnvironment instanceEnvironment) {
    auto node = (Fmi3InstanceNode *) instanceEnvironment;
    auto cb = node->callback_clockUpdate;

    JNIEnv *env = cb.env;
    JavaVM *vm = cb.g_vm;

    if (cb.clockUpdated != nullptr && checkJenvCallback(vm, env)) {

        auto jearlyReturnRequested = env->NewBooleanArray(1);
        auto jearlyReturnTime = env->NewDoubleArray(1);

        // call callback
        env->CallVoidMethod(cb.callbackObj,
                            cb.callbackMethod);

        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
        }

        vm->DetachCurrentThread();
    } else {
        // no receiver found
        printf("Unable to find receiver for jniFmi3CallbackIntermediateUpdate\n");
    }
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmu3
 * Method:    n3InstantiateScheduledExecution
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ZZ[JJJLorg/intocps/fmi3/jnifmuapi/NativeFmu3/ILogMessageCallback;Lorg/intocps/fmi3/jnifmuapi/NativeFmu3/ICallbackIntermediateUpdate;Lorg/intocps/fmi3/jnifmuapi/NativeFmu3/ICallbackLockPreemption;Lorg/intocps/fmi3/jnifmuapi/NativeFmu3/ICallbackUnlockPreemption;)J
 */

//JNIEXPORT jlong JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3_n3InstantiateScheduledExecution
//  (JNIEnv *, jobject, jlong, jstring, jstring, jstring, jboolean, jboolean, jlong, jobject, jobject, jobject, jobject);

//  protected native long n3InstantiateScheduledExecution(long fmuPtr, String instanceName, String instantiationToken, String resourceLocation,
//            boolean visible, boolean loggingOn, long instanceEnvironment, ILogMessageCallback logMessage, IClockUpdateCallback clockUpdate,
//            ILockPreemptionCallback lockPreemption, IUnlockPreemptionCallback unlockPreemption);
__attribute__((unused)) JNIEXPORT jlong JNICALL
Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3_nInstantiateScheduledExecution
        (JNIEnv *env, __attribute__((unused)) jobject obj, jlong fmuPtr, jstring name, jstring instantiationToken,
         jstring resourceLocation,
         jboolean visible,
         jboolean loggingOn,
         jobject logMessage, jobject clockUpdate, jobject lockPreemption, jobject unlockPreemption) {
    auto fmuNode = getFmuNodePtr(fmuPtr);
    const char *instanceName = name == nullptr ? "no name" : GetString(env, name);
    const char *fmuResourceLocation = nullptr;
    if (resourceLocation != nullptr)
        fmuResourceLocation = GetString(env, resourceLocation);
    const char *token = instantiationToken == nullptr ? nullptr : GetString(env, instantiationToken);
    auto node = new Fmi3InstanceNode();
    node->type = ScheduledExecution;
    node->name = string(instanceName);
    node->owner = fmuNode;

    initialize_preempt_locking();
    auto preemptHandler = get_preemtp_lock_handler(node);

    if (preemptHandler == nullptr) {
        throwException(env, "No more preempt handlers available. Please destroy on first");
    }

    //callback: jobject logMessage
    if (logMessage != nullptr) {
        node->callback_logMessage = createBasicCallback(env, logMessage, "logMessage",
                                                        "(Ljava/lang/String;Lorg/intocps/fmi/jnifmuapi/fmi3/Fmi3Status;Ljava/lang/String;Ljava/lang/String;)V");
        node->callback_logMessage.logMessage = jniFmi3CallbackLogMessage;

    } else {
        node->callback_logMessage.logMessage = nullptr;
    }
    //callback: jobject intermediateUpdate
    if (clockUpdate != nullptr) {

        node->callback_clockUpdate = createBasicCallback(env, clockUpdate, "updated",
                                                         "()V");

        //JFZZZZZZ[Z[
        //long instanceEnvironment, double intermediateUpdateTime, boolean clocksTicked,
        //                boolean intermediateVariableSetRequested, boolean intermediateVariableGetAllowed, boolean intermediateStepFinished,
        //                boolean canReturnEarly, boolean[] earlyReturnRequested, double[] earlyReturnTime

        node->callback_clockUpdate.clockUpdated = jnifmi3ClockUpdateCallback;
    } else { node->callback_clockUpdate.clockUpdated = nullptr; }

    if (lockPreemption != nullptr) {

        node->callback_lockPreemption = createBasicCallback(env, lockPreemption, "locked",
                                                            "()V");

        node->callback_lockPreemption.lockPreemption = preemptHandler->lock;
    } else { node->callback_lockPreemption.lockPreemption = nullptr; }

    if (unlockPreemption != nullptr) {

        node->callback_unlockPreemption = createBasicCallback(env, unlockPreemption, "unlocked",
                                                              "()V");

        node->callback_unlockPreemption.unlockPreemption = preemptHandler->unlock;
    } else { node->callback_unlockPreemption.unlockPreemption = nullptr; }

    if (env->ExceptionCheck()) {
        printf("exception\n");
        env->ExceptionDescribe();
    }


    auto instance = fmuNode->fmu.fmi3InstantiateScheduledExecution(instanceName, token, fmuResourceLocation,
                                                                   visible,
                                                                   loggingOn,
                                                                   node,
                                                                   node->callback_logMessage.logMessage,
                                                                   node->callback_clockUpdate.clockUpdated,
                                                                   node->callback_lockPreemption.lockPreemption,
                                                                   node->callback_unlockPreemption.unlockPreemption);
    node->instance = instance;
    if (instance != nullptr) {
        Fmi3Manager::getInstance()->store(instance, node);
    } else {
        delete node;
        node = nullptr;
    }

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        throwException3(env, "exception - 2\n");
    }
    if (name != nullptr) {
        env->ReleaseStringUTFChars(name, instanceName);
    }
    if (fmuResourceLocation != nullptr) {
        env->ReleaseStringUTFChars(resourceLocation, fmuResourceLocation);
    }
    if (token != nullptr) {
        env->ReleaseStringUTFChars(instantiationToken, token);
    }

    return (jlong) node;
}






