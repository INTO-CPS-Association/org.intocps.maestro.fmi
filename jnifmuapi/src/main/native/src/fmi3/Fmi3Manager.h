//
// Created by Kenneth Guldbrandt Lausdahl on 14/12/2020.
//

#ifndef FMUAPI_FMI3MANAGER_H
#define FMUAPI_FMI3MANAGER_H


#include <jni.h>
#include <string>
#include <map>

extern "C"
{
#include "fmi3.h"
#include "../jni_util.h"
}

#include <vector>


using namespace std;


class Fmi3Node {
public:
    FMU3 fmu;
    string name;

};

struct fmi3IntermediateUpdateCallbackUtilMethods {
    jmethodID earlyReturnTimeMethodId;
    jmethodID earlyReturnRequestedId;

};

struct CallbackJniInfo {
    union {
        fmi3IntermediateUpdateCallback intermediateUpdate;
        fmi3LogMessageCallback logMessage;
        fmi3LockPreemptionCallback lockPreemption;
        fmi3UnlockPreemptionCallback unlockPreemption;
        fmi3ClockUpdateCallback clockUpdated;
    };
    union {
        fmi3IntermediateUpdateCallbackUtilMethods intermediateUpdateUtilMethods;
    };
    jobject callbackObj;       // free
    jmethodID callbackMethod;  // free
    JavaVM *g_vm;
    JNIEnv *env;
};

enum Fmi2InstanceType {
    ModelExchange, CoSimulation, ScheduledExecution
};

class Fmi3InstanceNode {
public:
    Fmi3InstanceNode() {
        callback_logMessage.logMessage = nullptr;
        callback_logMessage.lockPreemption = nullptr;
        callback_logMessage.unlockPreemption = nullptr;
        callback_logMessage.intermediateUpdate = nullptr;
    }

    Fmi2InstanceType type;
    CallbackJniInfo callback_intermediateUpdate;
    CallbackJniInfo callback_logMessage;
    CallbackJniInfo callback_clockUpdate;
    CallbackJniInfo callback_lockPreemption;
    CallbackJniInfo callback_unlockPreemption;
    Fmi3Node *owner;
    std::string name;
    fmi3Instance instance;

    ~Fmi3InstanceNode() {
    }
};


class Fmi3Manager {
    static Fmi3Manager *_instance;
    map<fmi3Instance, Fmi3InstanceNode *> instanceToInstanceNode;

private:
    Fmi3Manager();

public:
    static Fmi3Manager *getInstance() {
        if (!_instance) {
            _instance = new Fmi3Manager();
        }
        return _instance;
    };

    void store(fmi3Instance instance, Fmi3InstanceNode *node);

    Fmi3InstanceNode *getInstanceNode(fmi3Instance);

    void freeInstance(fmi3Instance instance);
};

Fmi3Node *getFmuNodePtr(jlong fmuNodePtr);

Fmi3InstanceNode *getInstancePtr(jlong instanceNodePtr);

jobject convertStatus(JNIEnv *env, fmi3Status status);

void copyArray_fmi3DependencyKind_to_javaEnum(JNIEnv *env, const fmi3DependencyKind *dependencyKinds,
                                              jobjectArray jDependencyKinds, jsize len);

void copyArray_fmi3IntervalQualifiers_to_javaEnum(JNIEnv *env, const fmi3IntervalQualifier *qualifiers,
                                                  jobjectArray jQualifiers, jsize len);

typedef unsigned int uint;
#define COPY_ARRAY_TEMPLATE(fromType, apiName, toType)inline toType *createArray_##toType##_from_## fromType(JNIEnv *env, fromType##Array vr, jsize len) { \
int i;\
auto *body = env->Get## apiName ##ArrayElements( vr, 0);\
\
auto *vr_arr = new toType[ len];\
\
if (vr_arr == nullptr) {\
throwException(env, "malloc vr_arr failed");\
}\
\
for (i = 0; i < len; i++) {\
vr_arr[i] = body[i];\
}\
\
env->Release##apiName ##ArrayElements( vr, body, 0);\
\
return vr_arr;\
}


COPY_ARRAY_TEMPLATE(jlong, Long, long)

COPY_ARRAY_TEMPLATE(jlong, Long, uint)

COPY_ARRAY_TEMPLATE(jint, Int, int)

COPY_ARRAY_TEMPLATE(jdouble, Double, long)

COPY_ARRAY_TEMPLATE(jfloat, Float, float)

//COPY_ARRAY_TEMPLATE(jlong,Long,long)
void createStringToconstcharArray(JNIEnv *env, jobjectArray source,
                                  fmi3String *target, jsize len);


#define COPY_TO_JNI_ARRAY(fromType, apiName, toType)inline void copyArray_## fromType##_to_##toType(JNIEnv *env,const fromType *source,\
                                toType ##Array target, jsize len) {\
    auto vbody = env->Get##apiName##ArrayElements( target, 0);\
\
    int i;\
    for (i = 0; i < len; i++) {\
        vbody[i] =(toType) source[i];\
    }\
\
    env->Release##apiName##ArrayElements( target, vbody, 0);\
}

COPY_TO_JNI_ARRAY(double, Double, jdouble)

COPY_TO_JNI_ARRAY(int, Int, jint)

COPY_TO_JNI_ARRAY(bool, Boolean, jboolean)

COPY_TO_JNI_ARRAY(float, Double, jdouble)

//COPY_TO_JNI_ARRAY(fmi3Int8,Int,jint)



COPY_TO_JNI_ARRAY(fmi3Float32, Float, jfloat)

COPY_TO_JNI_ARRAY(fmi3Float64, Double, jdouble)

COPY_TO_JNI_ARRAY(fmi3Int8, Byte, jbyte)

COPY_TO_JNI_ARRAY(fmi3UInt8, Byte, jbyte)

COPY_TO_JNI_ARRAY(fmi3Int16, Short, jshort)

COPY_TO_JNI_ARRAY(fmi3UInt16, Short, jshort)

COPY_TO_JNI_ARRAY(fmi3Int32, Int, jint)

COPY_TO_JNI_ARRAY(fmi3UInt32, Int, jint)

COPY_TO_JNI_ARRAY(fmi3Int64, Long, jlong)

COPY_TO_JNI_ARRAY(fmi3UInt64, Long, jlong)

COPY_TO_JNI_ARRAY(fmi3Boolean, Boolean, jboolean)

COPY_TO_JNI_ARRAY(fmi3Clock, Boolean, jboolean)

COPY_TO_JNI_ARRAY(fmi3Char, Int, jint)

COPY_TO_JNI_ARRAY(fmi3Byte, Int, jint)

COPY_TO_JNI_ARRAY(fmi3Binary, Long, jlong)

#define COPY_FROM_JNI_ARRAY(toType, apiName, fromType) inline void copyArray_##fromType##_to_##toType(JNIEnv *env, fromType ##Array source, toType *target, jsize len) {\
    int i = 0;\
\
    auto vbody = env->Get##apiName##ArrayElements( source, 0);\
    for (i = 0; i < len; i++) {\
        target[i] = vbody[i];\
    }\
    env->Release##apiName##ArrayElements( source, vbody, 0);\
}

COPY_FROM_JNI_ARRAY(fmi3Float32, Float, jfloat)

COPY_FROM_JNI_ARRAY(fmi3Float64, Double, jdouble)

COPY_FROM_JNI_ARRAY(fmi3Int8, Byte, jbyte)

COPY_FROM_JNI_ARRAY(fmi3UInt8, Byte, jbyte)

COPY_FROM_JNI_ARRAY(fmi3Int16, Short, jshort)

COPY_FROM_JNI_ARRAY(fmi3UInt16, Short, jshort)

COPY_FROM_JNI_ARRAY(fmi3Int32, Int, jint)

COPY_FROM_JNI_ARRAY(fmi3UInt32, Int, jint)

COPY_FROM_JNI_ARRAY(fmi3Int64, Long, jlong)

COPY_FROM_JNI_ARRAY(fmi3UInt64, Long, jlong)

COPY_FROM_JNI_ARRAY(fmi3Boolean, Boolean, jboolean)

COPY_FROM_JNI_ARRAY(fmi3Clock, Boolean, jboolean)

COPY_FROM_JNI_ARRAY(fmi3Char, Int, jint)

COPY_FROM_JNI_ARRAY(fmi3Byte, Int, jint)

#endif //FMUAPI_FMI3MANAGER_H
