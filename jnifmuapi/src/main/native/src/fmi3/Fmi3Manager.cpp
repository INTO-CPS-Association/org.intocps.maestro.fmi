//
// Created by Kenneth Guldbrandt Lausdahl on 14/12/2020.
//

#include "Fmi3Manager.h"

Fmi3Manager* Fmi3Manager::instance;

Fmi3Manager::Fmi3Manager(){}

void Fmi3Manager::store(fmi3Instance instance, Fmi3InstanceNode* node) {
    this->instanceToInstanceNode[instance] = node;
}

void Fmi3Manager::freeInstance(fmi3Instance instance){
    auto instanceNode = this->getInstanceNode(instance);
    delete(instanceNode);
    this->instanceToInstanceNode.erase(instance);

}

Fmi3InstanceNode* Fmi3Manager::Fmi3Manager::getInstanceNode(fmi3Instance instance) {
    return this->instanceToInstanceNode[instance];
}


void createStringToconstcharArray(JNIEnv *env, jobjectArray source,
                                  fmi2String *target, jsize len) {
    int i = 0;
    for (i = 0; i < len; i++) {
        auto s = static_cast<jstring>(env->GetObjectArrayElement(source, i));
        jboolean blnIsCopy;
        const char *myarray = env->GetStringUTFChars( s, &blnIsCopy);

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

        env->ReleaseStringUTFChars( s, myarray);
        env->DeleteLocalRef( s);
    }
}


jobject convertStatus(JNIEnv *env, fmi3Status status) {
    jclass cls;
    jmethodID mid;
    jstring name = NULL;
    jobject eval = NULL;

    cls = env->FindClass("org/intocps/fmi3/Fmi3Status");
    if (cls == NULL)
        goto done;

    mid = env->GetStaticMethodID(cls, "valueOf", "(Ljava/lang/String;)Lorg/intocps/fmi3/Fmi3Status;");
    if (mid == NULL)
        goto done;


    switch (status) {
        case fmi3OK:
            name = env->NewStringUTF("fmi3OK");
            break;
        case fmi3Warning:
            name = env->NewStringUTF("fmi3Warning");
            break;
        case fmi3Discard:
            name = env->NewStringUTF("fmi3Discard");
            break;
        case fmi3Error:
            name = env->NewStringUTF("fmi3Error");
            break;
        case fmi3Fatal:
            name = env->NewStringUTF("fmi3Fatal");
            break;
    }


    if (name == NULL)
        goto done;

    eval = env->CallStaticObjectMethod(cls, mid, name);

    done:
    env->ExceptionClear();
    if (name != NULL)
        env->DeleteLocalRef(name);
    if (cls != NULL)
        env->DeleteLocalRef(cls);

    return eval;
}

FMU3 *getFmuPtr(jlong fmuPtr) { return (FMU3 *) fmuPtr; }

fmi3Instance getInstancePtr(jlong compPtr){return (fmi3Instance) compPtr;}

