//
// Created by Kenneth Guldbrandt Lausdahl on 17/12/2020.
//

#include "Fmi3Manager.h"
#include "config.h"
#include "org_intocps_fmi3_jnifmuapi_NativeFmu3.h"
extern "C"
{
#include "sim_support3.h"
#include "../jni_util.h"
}
#include <stdio.h>
/********************************************************
 * Utils
 ********************************************************/
/*
* Class:     org_intocps_fmi3_jnifmuapi_NativeFmu3
* Method:    getJniApiVersion
* Signature: ()J
*/
JNIEXPORT jstring JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmu3_getJniApiVersion
        (JNIEnv *env, jclass) {
    jstring result = env->NewStringUTF(PROJECT_VER);
    return result;
}

JNIEXPORT jlong JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmu3_nLoadLibrary(
        JNIEnv *env, jobject obj, jstring libraryPath) {
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

JNIEXPORT void JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmu3_nUnLoad(
        JNIEnv *env, jobject obj, jlong fmuptr) {
    FMU3* ptr = getFmuPtr(fmuptr);
#ifdef _WIN32
    FreeLibrary(ptr->dllHandle);
#elif __APPLE__
//extern "C"
//{
//#include "TargetConditionals.h"
//    }
#if TARGET_OS_MAC
    // Other kinds of Mac OS
    dlclose(ptr->dllHandle);
#else
    throwException(env, "Unsupported platform");
#endif
#elif __linux
    dlclose(ptr->dllHandle);
#endif

    delete(ptr);
}




void jniFmi3CallbackLogMessage(fmi3InstanceEnvironment instanceEnvironment,
                               fmi3String instanceName,
                               fmi3Status status,
                               fmi3String category,
                               fmi3String message) {

    auto node = (Fmi3InstanceNode *) instanceEnvironment;
    bool reported = 0;

    auto cb = node->callback_logMessage;

    if (cb.logMessage != nullptr) {
        reported = 1;
        JNIEnv *env = cb.env;
        JavaVM *vm = cb.g_vm;

        JNIEnv *g_env;
        // double check it's all ok

        int getEnvStat = vm->GetEnv((void **) &g_env, JNI_VERSION_1_6);
        if (getEnvStat == JNI_EDETACHED) {
            // printf("GetEnv: not attached\n");
            if (vm->AttachCurrentThread((void **) &g_env, NULL) != 0) {
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

        if (env->ExceptionCheck()) {
            printf("exception\n");
            env->ExceptionDescribe();
        }
        jstring jinstanceName = env->NewStringUTF(instanceName);

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
    }

    if (reported == 0) {
        // no receiver found
        printf("Unable to find receiver for %s reporting: %s", instanceName, message);
    }
}


void jniFmi3CallbackIntermediateUpdate(
        fmi3InstanceEnvironment instanceEnvironment,
        fmi3Float64 intermediateUpdateTime,
        fmi3Boolean clocksTicked,
        fmi3Boolean intermediateVariableSetRequested,
        fmi3Boolean intermediateVariableGetAllowed,
        fmi3Boolean intermediateStepFinished,
        fmi3Boolean canReturnEarly,
        fmi3Boolean *earlyReturnRequested,
        fmi3Float64 *earlyReturnTime) { printf("jniFmi3CallbackIntermediateUpdate - NOT IMPLEMENTED\n"); }


CallbackJniInfo createBasicCallback(JNIEnv *env, jobject obj, const char *methodName, const char *methodId) {
    CallbackJniInfo cb;
    cb.callbackObj = env->NewGlobalRef(obj);
    auto g_clazz = env->GetObjectClass(cb.callbackObj);
    if (g_clazz == NULL) {
        throwException(env, "Failed to find class");
    }
    cb.callbackMethod = env->GetMethodID(g_clazz, methodName,
                                         methodId);  //"(I)V");

    if (cb.callbackMethod == NULL) {
        throwException(env, "Unable to get method ref");
    }

    cb.env = env;
    env->GetJavaVM(&cb.g_vm);
}
/********************************************************
 * FMU
 ********************************************************/

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmu3
 * Method:    nGetVersion
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmu3_nGetVersion
        (JNIEnv *env, jobject, jlong fmuPtr) {

    jstring result = env->NewStringUTF(getFmuPtr(fmuPtr)->fmi3GetVersion());
    return result;

}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmu3
 * Method:    nInstantiateModelExchange
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ZZJLorg/intocps/fmi3/jnifmuapi/NativeFmu3/ICallbackLogMessage;)J
 */
JNIEXPORT jlong JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmu3_nInstantiateModelExchange
        (JNIEnv *, jobject, jlong fmuPtr, jstring, jstring, jstring, jboolean, jboolean, jlong, jobject) {

    return (jlong) nullptr;

}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmu3
 * Method:    nInstantiateCoSimulation
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ZZZ[JJJLorg/intocps/fmi3/jnifmuapi/NativeFmu3/ICallbackLogMessage;Lorg/intocps/fmi3/jnifmuapi/NativeFmu3/ICallbackIntermediateUpdate;)J
 */
JNIEXPORT jlong JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmu3_nInstantiateCoSimulation
        (JNIEnv *env, jobject obj, jlong fmuPtr, jstring name, jstring instantiationToken, jstring resourceLocation,
         jboolean visible, jboolean loggingOn, jboolean eventModeUsed, jlongArray requiredIntermediateVariables,
         jlong nRequiredIntermediateVariables, jlong instanceEnvironment, jobject logMessage,
         jobject intermediateUpdate) {

    auto fmuNode = getFmuPtr(fmuPtr);
    const char *instanceName = GetString(env, name);
    const char *fmuResourceLocation = GetString(env, resourceLocation);
    const char *guid = GetString(env, instantiationToken);
    auto node = new Fmi3InstanceNode();
    node->type = CoSimulation;

    //callback: jobject logMessage
    if (logMessage != NULL) {

        node->callback_logMessage = createBasicCallback(env, obj, "logMessage",
                                                        "(JLjava/lang/String;Lorg/intocps/fmi3/Fmi3Status;Ljava/lang/String;Ljava/lang/String;)V");
        node->callback_logMessage.logMessage = jniFmi3CallbackLogMessage;

    }
    //callback: jobject intermediateUpdate
    if (intermediateUpdate != NULL) {

        node->callback_intermediateUpdate = createBasicCallback(env, obj, "intermediateUpdate",
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

    fmi2ValueReference *requiredIntermediateVariablesArr = createArray_uint_from_jlong(env, requiredIntermediateVariables,
                                                                                  nRequiredIntermediateVariables);

    auto instance = fmuNode->fmi3InstantiateCoSimulation(instanceName, guid, fmuResourceLocation, visible,
                                                             loggingOn, eventModeUsed, requiredIntermediateVariablesArr,
                                                             nRequiredIntermediateVariables, &node,
                                                             node->callback_logMessage.logMessage,
                                                             node->callback_intermediateUpdate.intermediateUpdate);
    delete requiredIntermediateVariablesArr;

    if (instance != NULL) {
        Fmi3Manager::getInstance()->store(instance, node);
    } else {
        delete node;
    }

    if (env->ExceptionCheck()) {
        throwException(env, "exception - 2\n");
        env->ExceptionDescribe();
    }

    env->ReleaseStringUTFChars(name, instanceName);
    env->ReleaseStringUTFChars(resourceLocation, fmuResourceLocation);
    env->ReleaseStringUTFChars(instantiationToken, guid);

    return (jlong) instance;
}

/*
 * Class:     org_intocps_fmi3_jnifmuapi_NativeFmu3
 * Method:    n3InstantiateScheduledExecution
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ZZ[JJJLorg/intocps/fmi3/jnifmuapi/NativeFmu3/ICallbackLogMessage;Lorg/intocps/fmi3/jnifmuapi/NativeFmu3/ICallbackIntermediateUpdate;Lorg/intocps/fmi3/jnifmuapi/NativeFmu3/ICallbackLockPreemption;Lorg/intocps/fmi3/jnifmuapi/NativeFmu3/ICallbackUnlockPreemption;)J
 */
JNIEXPORT jlong JNICALL Java_org_intocps_fmi3_jnifmuapi_NativeFmu3_n3InstantiateScheduledExecution
        (JNIEnv *, jobject, jlong fmuPtr, jstring, jstring, jstring, jboolean, jboolean, jlongArray, jlong, jlong,
         jobject, jobject, jobject, jobject) {
    return (jlong) nullptr;
}





