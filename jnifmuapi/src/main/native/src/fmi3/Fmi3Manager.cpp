//
// Created by Kenneth Guldbrandt Lausdahl on 14/12/2020.
//

#include "Fmi3Manager.h"

Fmi3Manager* Fmi3Manager::_instance;

Fmi3Manager::Fmi3Manager()= default;

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
                                  fmi3String *target, jsize len) {
    int i;
    for (i = 0; i < len; i++) {
        auto s = static_cast<jstring>(env->GetObjectArrayElement(source, i));
        jboolean blnIsCopy;
        const char *myarray = env->GetStringUTFChars( s, &blnIsCopy);

        if (myarray == nullptr) {
            throwException(env, "JNI SetString failed to get string");
            return;
        }

        auto slen = strlen(myarray) + 1;  // Add NULL termination

        char *tmp = (char *)malloc(sizeof(char) * slen);

        if (tmp == nullptr) {
            throwException(env, "calloc tmp of char* failed");
        }

        strcpy(tmp, myarray);
        target[i] = tmp;

        env->ReleaseStringUTFChars( s, myarray);
        env->DeleteLocalRef( s);
    }
}

void copyArray_fmi3DependencyKind_to_javaEnum(JNIEnv *env, const fmi3DependencyKind *dependencyKinds, jobjectArray jDependencyKinds, jsize len) {
    if(len < 1){
        return;
    }

    jclass cls;
    jfieldID enumField = nullptr;
    const char* signature = "Lorg/intocps/fmi3/Fmi3DependencyKind;";
    
    cls = env->FindClass("org/intocps/fmi3/Fmi3DependencyKind");
    if (cls == nullptr) {
        return;
    }

    for(int i = 0; i < len; i++) {
        switch (dependencyKinds[i]) {
            // Not needed but reserved for future use?
            case fmi3Independent:
                enumField = env->GetStaticFieldID(cls , "fmi3Independent", signature);
                break;
            case fmi3Constant:
                enumField = env->GetStaticFieldID(cls , "fmi3Constant", signature);
                break;
            case fmi3Fixed:
                enumField = env->GetStaticFieldID(cls , "fmi3Fixed", signature);
                break;
            case fmi3Tunable:
                enumField = env->GetStaticFieldID(cls , "fmi3Tunable", signature);
                break;
            case fmi3Discrete:
                enumField = env->GetStaticFieldID(cls , "fmi3Discrete", signature);
                break;
            case fmi3Dependent:
                enumField = env->GetStaticFieldID(cls , "fmi3Dependent", signature);
                break;
        }
        if(enumField != nullptr){
            jobject enumVal = env->GetStaticObjectField(cls, enumField);
            if(enumVal != nullptr){
                env->SetObjectArrayElement(jDependencyKinds, i, enumVal);
                env->DeleteLocalRef(enumVal);
            }
        }
    }

    env->ExceptionClear();
    env->DeleteLocalRef(cls);
}

void copyArray_fmi3IntervalQualifiers_to_javaEnum(JNIEnv *env, const fmi3IntervalQualifier *qualifiers, jobjectArray jQualifiers, jsize len) {
    if(len < 1){
        return;
    }

    jclass cls;
    jfieldID enumField = nullptr;
    const char* signature = "Lorg/intocps/fmi3/Fmi3IntervalQualifier;";

    cls = env->FindClass("org/intocps/fmi3/Fmi3IntervalQualifier");
    if (cls == nullptr) {
        return;
    }

    for(int i = 0; i < len; i++) {
        switch (qualifiers[i]) {
            case fmi3IntervalNotYetKnown:
                enumField = env->GetStaticFieldID(cls , "fmi3IntervalNotYetKnown", signature);
                break;
            case fmi3IntervalUnchanged:
                enumField = env->GetStaticFieldID(cls , "fmi3IntervalUnchanged", signature);
                break;
            case fmi3IntervalChanged:
                enumField = env->GetStaticFieldID(cls , "fmi3IntervalChanged", signature);
                break;
        }
        if(enumField != nullptr){
            jobject enumVal = env->GetStaticObjectField(cls, enumField);
            if(enumVal != nullptr){
                env->SetObjectArrayElement(jQualifiers, i, enumVal);
                env->DeleteLocalRef(enumVal);
            }
        }
    }

    env->ExceptionClear();
    env->DeleteLocalRef(cls);
}


jobject convertStatus(JNIEnv *env, fmi3Status status) {
    jclass cls;
    jmethodID mid;
    jstring name = nullptr;
    jobject eval = nullptr;

    cls = env->FindClass("org/intocps/fmi/jnifmuapi/fmi3/Fmi3Status");
    if (cls == nullptr) {
        printf("Failed to find class: org/intocps/fmi/jnifmuapi/fmi3/Fmi3Status");
        goto done;
    }

    mid = env->GetStaticMethodID(cls, "valueOf", "(Ljava/lang/String;)Lorg/intocps/fmi/jnifmuapi/fmi3/Fmi3Status;");
    if (mid == nullptr) {
        printf("Failed to find class: org/intocps/fmi/jnifmuapi/fmi3/Fmi3Status");
        goto done;
    }


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


    if (name == nullptr) {
        printf("Failed to create name for status: %d" , status);
        goto done;
    }

    eval = env->CallStaticObjectMethod(cls, mid, name);

    done:
    env->ExceptionClear();
    if (name != nullptr)
        env->DeleteLocalRef(name);
    if (cls != nullptr)
        env->DeleteLocalRef(cls);

    return eval;
}

FMU3 *getFmuPtr(jlong fmuPtr) { return (FMU3 *) fmuPtr; }

fmi3Instance getInstancePtr(jlong compPtr){return (fmi3Instance) compPtr;}




