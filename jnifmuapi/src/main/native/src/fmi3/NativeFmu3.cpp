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

    if (!loadDll3(path, &node->fmu)) {
        throwException(env, "Load failed!!!");
        return 0;
    }

    env->ReleaseStringUTFChars(libraryPath, path);
    return (jlong) node;
}

__attribute__((unused)) JNIEXPORT void JNICALL Java_org_intocps_fmi_jnifmuapi_fmi3_NativeFmu3_nUnLoad(
        __attribute__((unused)) JNIEnv *env, __attribute__((unused)) jobject obj, jlong fmuptr) {
    Fmi3Node *ptr = getFmuNodePtr(fmuptr);
#ifdef _WIN32
    FreeLibrary(ptr->dllHandle);
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
    dlclose(ptr->dllHandle);
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

    auto node = (Fmi3InstanceNode *) instanceEnvironment;
    auto cb = node->callback_intermediateUpdate;

    JNIEnv *env = cb.env;
    JavaVM *vm = cb.g_vm;

    if (cb.logMessage != nullptr && checkJenvCallback(vm, env)) {

        auto jearlyReturnRequested = env->NewBooleanArray(1);
        auto jearlyReturnTime = env->NewDoubleArray(1);

        // call callback
        env->CallVoidMethod(cb.callbackObj,
                            cb.callbackMethod, instanceEnvironment,
                            intermediateUpdateTime,
                            intermediateVariableSetRequested,
                            intermediateVariableGetAllowed,
                            intermediateStepFinished,
                            canReturnEarly, jearlyReturnRequested, jearlyReturnTime);

        auto jearlyReturnRequestedBody = env->GetBooleanArrayElements(jearlyReturnRequested, JNI_FALSE);
        *earlyReturnRequested = jearlyReturnRequestedBody[0];
        env->ReleaseBooleanArrayElements(jearlyReturnRequested, jearlyReturnRequestedBody, 0);

        auto jjearlyReturnTimeBody = env->GetDoubleArrayElements(jearlyReturnTime, JNI_FALSE);
        *earlyReturnTime = jjearlyReturnTimeBody[0];
        env->ReleaseDoubleArrayElements(jearlyReturnTime, jjearlyReturnTimeBody, 0);

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
        throwException3(env, "Failed to find class");
    }
    cb.callbackMethod = env->GetMethodID(g_clazz, methodName,
                                         methodId);  //"(I)V");

    if (cb.callbackMethod == nullptr) {
        throwException3(env, "Unable to get method ref");
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
        (JNIEnv *env, __attribute__((unused)) jobject obj,jlong fmuPtr, jstring name, jstring instantiationToken, jstring resourceLocation, jboolean visible, jboolean loggingOn,
         jobject logMessage) {
//long fmuPtr, String instanceName, String instantiationToken, String resourceLocation,
//            boolean visible, boolean loggingOn, long instanceEnvironment, ILogMessageCallback logMessage
    auto fmuNode = getFmuNodePtr(fmuPtr);
    const char *instanceName = GetString(env, name);
    const char *fmuResourceLocation = nullptr;
    if (resourceLocation != nullptr)
        fmuResourceLocation = GetString(env, resourceLocation);
    const char *guid = GetString(env, instantiationToken);
    auto node = new Fmi3InstanceNode();
    node->type = CoSimulation;
    node->name = string(instanceName);

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

    auto instance = fmuNode->fmu.fmi3InstantiateModelExchange(instanceName, guid, fmuResourceLocation, visible,
                                                         loggingOn, node,
                                                         node->callback_logMessage.logMessage);
    if (instance != nullptr) {
        Fmi3Manager::getInstance()->store(instance, node);
    } else {
        delete node;
    }

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        throwException3(env, "exception - 2\n");
    }
    env->ReleaseStringUTFChars(name, instanceName);
    if (fmuResourceLocation != nullptr) {
        env->ReleaseStringUTFChars(resourceLocation, fmuResourceLocation);
    }
    if (guid != nullptr) {
        env->ReleaseStringUTFChars(instantiationToken, guid);
    }

    return (jlong) instance;
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
    const char *instanceName = GetString(env, name);
    const char *fmuResourceLocation = nullptr;
    if (resourceLocation != nullptr)
        fmuResourceLocation = GetString(env, resourceLocation);
    const char *guid = GetString(env, instantiationToken);
    auto node = new Fmi3InstanceNode();
    node->type = CoSimulation;
    node->name = string(instanceName);
    node->owner=fmuNode;


    //callback: jobject logMessage
    if (logMessage != nullptr) {
        node->callback_logMessage = createBasicCallback(env, logMessage, "logMessage",
                                                        "(Ljava/lang/String;Lorg/intocps/fmi/jnifmuapi/fmi3/Fmi3Status;Ljava/lang/String;Ljava/lang/String;)V");
        node->callback_logMessage.logMessage = jniFmi3CallbackLogMessage;

    }
    //callback: jobject intermediateUpdate
    if (intermediateUpdate != nullptr) {

        node->callback_intermediateUpdate = createBasicCallback(env, intermediateUpdate, "intermediateUpdate",
                                                                "(JFZZZZZZ[Z[)V");

        //JFZZZZZZ[Z[
        //long instanceEnvironment, double intermediateUpdateTime, boolean clocksTicked,
        //                boolean intermediateVariableSetRequested, boolean intermediateVariableGetAllowed, boolean intermediateStepFinished,
        //                boolean canReturnEarly, boolean[] earlyReturnRequested, double[] earlyReturnTime

        node->callback_intermediateUpdate.intermediateUpdate = jniFmi3CallbackIntermediateUpdate;
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
    auto instance = fmuNode->fmu.fmi3InstantiateCoSimulation(instanceName, guid, fmuResourceLocation, visible,
                                                         loggingOn, eventModeUsed, earlyReturnAllowed,
                                                         requiredIntermediateVariablesArr,
                                                         nRequiredIntermediateVariables, node,
                                                         node->callback_logMessage.logMessage,
                                                         node->callback_intermediateUpdate.intermediateUpdate);
    node->instance=instance;
    delete requiredIntermediateVariablesArr;
    if (instance != nullptr) {
        Fmi3Manager::getInstance()->store(instance, node);
    } else {
        delete node;
    }

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        throwException3(env, "exception - 2\n");
    }
    env->ReleaseStringUTFChars(name, instanceName);
    if (fmuResourceLocation != nullptr) {
        env->ReleaseStringUTFChars(resourceLocation, fmuResourceLocation);
    }
    if (guid != nullptr) {
        env->ReleaseStringUTFChars(instantiationToken, guid);
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

struct PreeStruct {
    fmi3LockPreemptionCallback lock;
    fmi3UnlockPreemptionCallback unlock;
    Fmi3InstanceNode *handler;
};

PreeStruct preemptionHandlers[10];

#define PREEEMPTLOCKING_X(id)                                           \
void cbPreemptLock##id()                                                \
{                                                                       \
    if(preemptionHandlers[ id ].handler!= nullptr)                      \
    {                                                                   \
        jniPreemptLockCallback(preemptionHandlers[ id ].handler,true);  \
    }                                                                   \
}                                                                       \
void cbPreemptUnLock##id()                                              \
{                                                                       \
    if(preemptionHandlers[ id ].handler!= nullptr)                      \
    {                                                                   \
        jniPreemptLockCallback(preemptionHandlers[ id ].handler,false); \
    }                                                                   \
}

#define PREEEMPTLOCKING_INIT_X(id)                                                                          \
if(preemptionHandlers[ id ].lock==nullptr)                                                                  \
{                                                                                                           \
    preemptionHandlers[ id ]={.lock=&cbPreemptLock##id, .unlock=&cbPreemptUnLock##id,.handler = nullptr};   \
}

PREEEMPTLOCKING_X(0)

PREEEMPTLOCKING_X(1)

PREEEMPTLOCKING_X(2)

PREEEMPTLOCKING_X(3)

PREEEMPTLOCKING_X(4)

PREEEMPTLOCKING_X(5)

PREEEMPTLOCKING_X(6)

PREEEMPTLOCKING_X(7)

PREEEMPTLOCKING_X(8)

PREEEMPTLOCKING_X(9)

void initialize_preempt_locking() {
    PREEEMPTLOCKING_INIT_X(0)
    PREEEMPTLOCKING_INIT_X(1)
    PREEEMPTLOCKING_INIT_X(2)
    PREEEMPTLOCKING_INIT_X(3)
    PREEEMPTLOCKING_INIT_X(4)
    PREEEMPTLOCKING_INIT_X(5)
    PREEEMPTLOCKING_INIT_X(6)
    PREEEMPTLOCKING_INIT_X(7)
    PREEEMPTLOCKING_INIT_X(8)
    PREEEMPTLOCKING_INIT_X(9)
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

void jnifmi3ClockUpdateCallback(fmi3InstanceEnvironment  instanceEnvironment)
{

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
    const char *instanceName = GetString(env, name);
    const char *fmuResourceLocation = nullptr;
    if (resourceLocation != nullptr)
        fmuResourceLocation = GetString(env, resourceLocation);
    const char *token = GetString(env, instantiationToken);
    auto node = new Fmi3InstanceNode();
    node->type = ScheduledExecution;
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

    }
    //callback: jobject intermediateUpdate
    if (clockUpdate != nullptr) {

        node->callback_intermediateUpdate = createBasicCallback(env, clockUpdate, "updated",
                                                                "()V");

        //JFZZZZZZ[Z[
        //long instanceEnvironment, double intermediateUpdateTime, boolean clocksTicked,
        //                boolean intermediateVariableSetRequested, boolean intermediateVariableGetAllowed, boolean intermediateStepFinished,
        //                boolean canReturnEarly, boolean[] earlyReturnRequested, double[] earlyReturnTime

        node->callback_intermediateUpdate.intermediateUpdate = jniFmi3CallbackIntermediateUpdate;
    }

    if (lockPreemption != nullptr) {

        node->callback_lockPreemption = createBasicCallback(env, lockPreemption, "locked",
                                                            "()V");

        node->callback_lockPreemption.lockPreemption = preemptHandler->lock;
    }

    if (unlockPreemption != nullptr) {

        node->callback_unlockPreemption = createBasicCallback(env, unlockPreemption, "unlocked",
                                                              "()V");

        node->callback_unlockPreemption.unlockPreemption = preemptHandler->unlock;
    }

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
    if (instance != nullptr) {
        Fmi3Manager::getInstance()->store(instance, node);
    } else {
        delete node;
    }

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        throwException3(env, "exception - 2\n");
    }
    env->ReleaseStringUTFChars(name, instanceName);
    if (fmuResourceLocation != nullptr) {
        env->ReleaseStringUTFChars(resourceLocation, fmuResourceLocation);
    }
    if (token != nullptr) {
        env->ReleaseStringUTFChars(instantiationToken, token);
    }

    return (jlong) instance;
}






