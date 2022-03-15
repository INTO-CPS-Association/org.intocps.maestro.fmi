
#include "fmi3Functions.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#define VL 4
struct Model {
    fmi3Float32 v_fmi3Float32[VL];
    fmi3Float64 v_fmi3Float64[VL];
    fmi3Int8 v_fmi3Int8[VL];
    fmi3UInt8 v_fmi3UInt8[VL];
    fmi3Int16 v_fmi3Int16[VL];
    fmi3UInt16 v_fmi3UInt16[VL];
    fmi3Int32 v_fmi3Int32[VL];
    fmi3UInt32 v_fmi3UInt32[VL];
    fmi3Int64 v_fmi3Int64[VL];
    fmi3UInt64 v_fmi3UInt64[VL];
    fmi3Boolean v_fmi3Boolean[VL];
    fmi3Char v_fmi3Char[VL];
    fmi3String v_fmi3String[VL];
    fmi3Byte v_fmi3Byte[VL];
    fmi3Binary v_fmi3Binary[VL];
    size_t v_fmiBinarySizes[VL];
    fmi3Clock v_fmi3Clock[VL];
    fmi3InstanceEnvironment instanceEnvironment;
    fmi3LogMessageCallback logMessage;
    fmi3String instanceName;
    fmi3Boolean loggingOn;
};

/***************************************************
 Common Functions
 ****************************************************/

const char *fmi3GetVersion() {
    return fmi3Version;
}

fmi3Status fmi3SetDebugLogging(fmi3Instance instance,
                               fmi3Boolean loggingOn,
                               size_t nCategories,
                               const fmi3String categories[]) {

    if (loggingOn && nCategories == 2 && categories != NULL && strcmp(categories[0], "A") == 0 &&
        strcmp(categories[1], "B") == 0) {
        return fmi3OK;
    }
    printf("LoggingOn: %d, Categories: %zu, \n", loggingOn, nCategories);
    if (categories != NULL && nCategories > 1) {
        printf("Category: '%s'\n", categories[0]);
        printf("Category: '%s'\n", categories[1]);
    }
    return fmi3Error;
}

fmi3Instance instantiateModel(fmi3String instanceName,
                              fmi3Boolean loggingOn,
                              fmi3InstanceEnvironment instanceEnvironment,
                              fmi3LogMessageCallback logMessage) {
    struct Model *model = calloc(1, sizeof(struct Model));
    if (instanceName != NULL) {
        model->instanceName = strdup((instanceName));
    }
    model->loggingOn = loggingOn;
    model->logMessage = logMessage;
    model->instanceEnvironment = instanceEnvironment;
    return model;

}

fmi3Instance fmi3InstantiateModelExchange(
        fmi3String instanceName,
        fmi3String instantiationToken,
        fmi3String resourcePath,
        fmi3Boolean visible,
        fmi3Boolean loggingOn,
        fmi3InstanceEnvironment instanceEnvironment,
        fmi3LogMessageCallback logMessage) {

    if (instanceName == NULL || instantiationToken == NULL || resourcePath == NULL) {
        printf("Getting nulls\n");
        return NULL;
    }

    if (strstr(instanceName, "fmi3functiontest") == NULL || strcmp(instantiationToken, "{FMI3_TEST_TOKEN}") != 0 ||
        strcmp(resourcePath, "/tmp") != 0 || !visible || !loggingOn) {
        printf("Name: '%s', token: '%s', resourcePath: '%s', visible: %d, loggingOn: %d\n", instanceName,
               instantiationToken, resourcePath, visible, loggingOn);
        return NULL;
    }

    struct Model *model = instantiateModel(instanceName, loggingOn, instanceEnvironment, logMessage);

    if (logMessage != NULL) {
        printf("Calling logMessage\n");
        logMessage(instanceEnvironment, fmi3OK, "som category", "some message");
    }

    return model;
}

fmi3Instance fmi3InstantiateCoSimulation(
        fmi3String instanceName,
        fmi3String instantiationToken,
        fmi3String resourcePath,
        fmi3Boolean visible,
        fmi3Boolean loggingOn,
        fmi3Boolean eventModeUsed,
        fmi3Boolean earlyReturnAllowed,
        const fmi3ValueReference requiredIntermediateVariables[],
        size_t nRequiredIntermediateVariables,
        fmi3InstanceEnvironment instanceEnvironment,
        fmi3LogMessageCallback logMessage,
        fmi3IntermediateUpdateCallback intermediateUpdate) {


    if (instanceName == NULL || instantiationToken == NULL || resourcePath == NULL) {
        printf("Getting nulls\n");
        return NULL;
    }

    if (strstr(instanceName, "fmi3functiontest") == NULL || strcmp(instantiationToken, "{FMI3_TEST_TOKEN}") != 0 ||
        strcmp(resourcePath, "/tmp") != 0 || !visible || !loggingOn || !eventModeUsed || !earlyReturnAllowed) {
        printf("Name: '%s', token: '%s', resourcePath: '%s', visible: %d, loggingOn: %d, eventModeUsed: %d, earlyReturnAllowed: %d\n",
               instanceName,
               instantiationToken, resourcePath, visible, loggingOn, eventModeUsed, earlyReturnAllowed);
        return NULL;
    }

    if (nRequiredIntermediateVariables != 2 || requiredIntermediateVariables == NULL ||
        requiredIntermediateVariables[0] != 1 || requiredIntermediateVariables[1] != 2) {
        printf("Wrong required intermediate variables count: %zu\n", nRequiredIntermediateVariables);
        if (requiredIntermediateVariables != NULL) {
            printf("Wrong required intermediate variables: %d, %d\n", requiredIntermediateVariables[0],
                   requiredIntermediateVariables[1]);
        }
        return NULL;
    }

    struct Model *model = instantiateModel(instanceName, loggingOn, instanceEnvironment, logMessage);


    if (intermediateUpdate != NULL) {
        printf("Calling intermediateUpdate\n");
        bool returnEarly;
        double earlyTime;
        intermediateUpdate(instanceEnvironment, 1.1, true, true, true, true, &returnEarly, &earlyTime);
        printf("Calling intermediateUpdate. Call completed checking returned variables\n");
        if (!returnEarly || earlyTime != 555.555) {
            printf("IntermediateUpdate returned: %d, %f\n", returnEarly, earlyTime);
            return NULL;
        }
    }
    if (logMessage != NULL) {
        printf("Calling logMessage\n");
        logMessage(instanceEnvironment, fmi3OK, "som category", "some message");
    }


    return model;
}

fmi3Instance fmi3InstantiateScheduledExecution(
        fmi3String instanceName,
        fmi3String instantiationToken,
        fmi3String resourcePath,
        fmi3Boolean visible,
        fmi3Boolean loggingOn,
        fmi3InstanceEnvironment instanceEnvironment,
        fmi3LogMessageCallback logMessage,
        fmi3ClockUpdateCallback clockUpdate,
        fmi3LockPreemptionCallback lockPreemption,
        fmi3UnlockPreemptionCallback unlockPreemption) {

    if (instanceName == NULL || instantiationToken == NULL || resourcePath == NULL) {
        printf("Getting nulls\n");
        return NULL;
    }

    if (strstr(instanceName, "fmi3functiontest") == NULL || strcmp(instantiationToken, "{FMI3_TEST_TOKEN}") != 0 ||
        strcmp(resourcePath, "/tmp") != 0 || !visible || !loggingOn) {
        printf("Name: '%s', token: '%s', resourcePath: '%s', visible: %d, loggingOn: %d\n", instanceName,
               instantiationToken, resourcePath, visible, loggingOn);
        return NULL;
    }

    struct Model *model = instantiateModel(instanceName, loggingOn, instanceEnvironment, logMessage);

    if (clockUpdate != NULL) {
        printf("Calling clockUpdate\n");
        clockUpdate(instanceEnvironment);
    }

    if (lockPreemption != NULL) {
        printf("Calling lockPreemption\n");
        lockPreemption();
    }

    if (unlockPreemption != NULL) {
        printf("Calling unlockPreemption\n");
        unlockPreemption();
    }
    if (logMessage != NULL) {
        printf("Calling logMessage\n");
        logMessage(instanceEnvironment, fmi3OK, "som category", "some message");
    }

    return model;
}


void fmi3FreeInstance(fmi3Instance instance) {

}

fmi3Status fmi3EnterInitializationMode(fmi3Instance instance,
                                       fmi3Boolean toleranceDefined,
                                       fmi3Float64 tolerance,
                                       fmi3Float64 startTime,
                                       fmi3Boolean stopTimeDefined,
                                       fmi3Float64 stopTime) {


    return fmi3OK;
}

fmi3Status fmi3ExitInitializationMode(fmi3Instance instance) {


    return fmi3OK;
}

fmi3Status fmi3EnterEventMode(fmi3Instance instance,
                              fmi3Boolean stepEvent,
                              fmi3Boolean stateEvent,
                              const fmi3Int32 rootsFound[],
                              size_t nEventIndicators,
                              fmi3Boolean timeEvent) {


    return fmi3OK;
}

fmi3Status fmi3Terminate(fmi3Instance instance) {


    return fmi3OK;
}

fmi3Status fmi3Reset(fmi3Instance instance) {


    return fmi3OK;
}

#define MAX_MSG_SIZE 1000

void print_log(fmi3Instance instance,
               fmi3Status status,
               fmi3String category,
               fmi3String message, ...) {

    struct Model *model = ((struct Model *) instance);

    if (model->loggingOn) {

        char msg[MAX_MSG_SIZE];
        va_list argp;

        // replace C format strings
        va_start(argp, message);
        vsprintf(msg, message, argp);
        va_end(argp);
        model->logMessage(model->instanceEnvironment, status, category, msg);
    }
}

#define GET(type_name) \
fmi3Status  fmi3Get##type_name(  fmi3Instance instance,\
                            const fmi3ValueReference valueReferences[],\
                            size_t nValueReferences,\
                            fmi3##type_name values[],\
                            size_t nValues) {\
                       printf("In get\n");\
    for(int i=0;i<nValueReferences;i++){          \
       const fmi3ValueReference vref = valueReferences[i];             \
       if (vref<0 || vref>VL) {                        \
            print_log(instance,fmi3Error,"fmi3Error","VR %d at index %d is our of range max is %d",vref,i,VL);\
            return fmi3Error;                               \
       }\
                       \
        printf("In geting %d\n",vref);fmi3##type_name val = ((struct Model*)instance)->v_fmi3##type_name[vref];\
        print_log(instance,fmi3OK,"fmi3Ok","Get##type_name VR %d at index %d",vref,i);\
        values[i]=val;\
    }                   \
    return fmi3OK;\
}

GET(Float32)

GET(Float64)

GET(Int8)

GET(UInt8)

GET(Int16)

GET(UInt16)

GET(Int32)

GET(UInt32)

GET(Int64)

GET(UInt64)

GET(Boolean)

GET(Char)

//GET(String)

GET(Byte)

//GET(Binary)
//GET(Clock)
fmi3Status fmi3GetString(fmi3Instance instance, const fmi3ValueReference valueReferences[], size_t nValueReferences,
                         fmi3String values[], size_t nValues) {
    for (int i = 0; i < nValueReferences; i++) {
        const fmi3ValueReference vref = valueReferences[i];
        if (vref < 0 || vref > 4) {
            print_log(instance, fmi3Error, "fmi3Error", "VR %d at index %d is our of range max is %d", vref, i, 4);
            return fmi3Error;
        }
        fmi3String val = ((struct Model *) instance)->v_fmi3String[vref];
        print_log(instance, fmi3OK, "fmi3Ok", "Get##type_name VR %d at index %d", vref, i);
        printf("string is '%s'\n", val);
        if (val == 0) {
            values[i] = "";
        } else {
            values[i] = val;
        }
    }
    return fmi3OK;
}


fmi3Status fmi3GetBinary(fmi3Instance instance, const fmi3ValueReference valueReferences[], size_t nValueReferences,
                         size_t valueSizes[],
                         fmi3Binary values[], size_t nValues) {
    for (int i = 0; i < nValueReferences; i++) {
        const fmi3ValueReference vref = valueReferences[i];
        if (vref < 0 || vref > 4) {
            print_log(instance, fmi3Error, "fmi3Error", "VR %d at index %d is our of range max is %d", vref, i, 4);
            return fmi3Error;
        }

        struct Model *model = instance;
        fmi3Binary val = model->v_fmi3Binary[vref];
        // print_log(instance, fmi3OK, "fmi3Ok", "Get##type_name VR %d at index %d", vref, i);

        print_log(instance, fmi3OK, "fmi3Ok",
                  "VR %d at index %d   Recorded size: %d", vref, i, model->v_fmiBinarySizes[vref]);

        size_t binSize = model->v_fmiBinarySizes[vref];
//        values[i]= malloc(binSize);
//        memcpy((void *) values[i], val, binSize);
        values[i] = val;
        valueSizes[i] = binSize;

    }
    return fmi3OK;
}

fmi3Status fmi3GetClock(fmi3Instance instance, const fmi3ValueReference valueReferences[], size_t nValueReferences,
                        fmi3Clock values[]) {
    for (int i = 0; i < nValueReferences; i++) {
        const fmi3ValueReference vref = valueReferences[i];
        if (vref < 0 || vref > 4) {
            print_log(instance, fmi3Error, "fmi3Error", "VR %d at index %d is our of range max is %d", vref, i, 4);
            return fmi3Error;
        }
        fmi3Clock val = ((struct Model *) instance)->v_fmi3Clock[vref];
        print_log(instance, fmi3OK, "fmi3Ok", "Get##type_name VR %d at index %d", vref, i);
        values[i] = val;
    }
    return fmi3OK;
}


//fmi3Status fmi3GetFloat32(fmi3Instance instance,
//                          const fmi3ValueReference valueReferences[],
//                          size_t nValueReferences,
//                          fmi3Float32 values[],
//                          size_t nValues) {
//
//    return fmi3OK;
//}

//fmi3Status fmi3GetFloat64(fmi3Instance instance,
//                          const fmi3ValueReference valueReferences[],
//                          size_t nValueReferences,
//                          fmi3Float64 values[],
//                          size_t nValues) {
//
//
//    return fmi3OK;
//}
//
//fmi3Status fmi3GetInt8(fmi3Instance instance,
//                       const fmi3ValueReference valueReferences[],
//                       size_t nValueReferences,
//                       fmi3Int8 values[],
//                       size_t nValues) {
//
//    return fmi3OK;
//}
//
//fmi3Status fmi3GetUInt8(fmi3Instance instance,
//                        const fmi3ValueReference valueReferences[],
//                        size_t nValueReferences,
//                        fmi3UInt8 values[],
//                        size_t nValues) {
//
//    return fmi3OK;
//}
//
//fmi3Status fmi3GetInt16(fmi3Instance instance,
//                        const fmi3ValueReference valueReferences[],
//                        size_t nValueReferences,
//                        fmi3Int16 values[],
//                        size_t nValues) {
//
//    return fmi3OK;
//}
//
//fmi3Status fmi3GetUInt16(fmi3Instance instance,
//                         const fmi3ValueReference valueReferences[],
//                         size_t nValueReferences,
//                         fmi3UInt16 values[],
//                         size_t nValues) {
//
//    return fmi3OK;
//}
//
//fmi3Status fmi3GetInt32(fmi3Instance instance,
//                        const fmi3ValueReference valueReferences[],
//                        size_t nValueReferences,
//                        fmi3Int32 values[],
//                        size_t nValues) {
//    return fmi3OK;
//}
//
//fmi3Status fmi3GetUInt32(fmi3Instance instance,
//                         const fmi3ValueReference valueReferences[],
//                         size_t nValueReferences,
//                         fmi3UInt32 values[],
//                         size_t nValues) {
//
//    return fmi3OK;
//}
//
//fmi3Status fmi3GetInt64(fmi3Instance instance,
//                        const fmi3ValueReference valueReferences[],
//                        size_t nValueReferences,
//                        fmi3Int64 values[],
//                        size_t nValues) {
//
//    return fmi3OK;
//}
//
//fmi3Status fmi3GetUInt64(fmi3Instance instance,
//                         const fmi3ValueReference valueReferences[],
//                         size_t nValueReferences,
//                         fmi3UInt64 values[],
//                         size_t nValues) {
//    return fmi3OK;
//}
//
//fmi3Status fmi3GetBoolean(fmi3Instance instance,
//                          const fmi3ValueReference valueReferences[],
//                          size_t nValueReferences,
//                          fmi3Boolean values[],
//                          size_t nValues) {
//    return fmi3OK;
//}
//
//fmi3Status fmi3GetString(fmi3Instance instance,
//                         const fmi3ValueReference valueReferences[],
//                         size_t nValueReferences,
//                         fmi3String values[],
//                         size_t nValues) {
//    return fmi3OK;
//}
//
//fmi3Status fmi3GetBinary(fmi3Instance instance,
//                         const fmi3ValueReference valueReferences[],
//                         size_t nValueReferences,
//                         size_t valueSizes[],
//                         fmi3Binary values[],
//                         size_t nValues) {
//
//    return fmi3OK;
//}
//
//fmi3Status fmi3GetClock(fmi3Instance instance,
//                        const fmi3ValueReference valueReferences[],
//                        size_t nValueReferences,
//                        fmi3Clock values[]) {
//
//    return fmi3OK;
//}


#define SET(type_name) \
fmi3Status  fmi3Set##type_name(  fmi3Instance instance,\
                            const fmi3ValueReference valueReferences[],\
                            size_t nValueReferences,\
                           const fmi3##type_name values[],\
                            size_t nValues) {\
                       \
    for(int i=0;i<nValueReferences;i++){          \
       const fmi3ValueReference vref = valueReferences[i];             \
       if (vref<0 || vref>VL) {                        \
            print_log(instance,fmi3Error,"fmi3Error","VR %d at index %d is our of range max is %d",vref,i,VL);\
            return fmi3Error;                               \
       }\
       \
        fmi3##type_name val = values[i];\
        print_log(instance,fmi3OK,"fmi3Ok","Set##type_name VR %d at index %d",vref,i);\
        ((struct Model*)instance)->v_fmi3##type_name[vref]=val;\
    }                   \
    return fmi3OK;\
}

SET(Float32)

SET(Float64)

SET(Int8)

SET(UInt8)

SET(Int16)

SET(UInt16)

SET(Int32)

SET(UInt32)

SET(Int64)

SET(UInt64)

SET(Boolean)

SET(Char)

//SET(String)

SET(Byte)
//GET(Binary)
//GET(Clock)

fmi3Status fmi3SetString(fmi3Instance instance, const fmi3ValueReference valueReferences[], size_t nValueReferences,
                         const fmi3String values[], size_t nValues) {
    printf("fmi3 api test fmi3SetString\n");

    if (nValueReferences != nValues) {
        print_log(instance, fmi3Error, "fmi3Error", "Vrefs %d  and value count not matching %d ", nValueReferences,
                  nValues);
        return fmi3Error;
    }

    for (int i = 0; i < nValueReferences; i++) {
        const fmi3ValueReference vref = valueReferences[i];
        if (vref < 0 || vref > 4) {
            print_log(instance, fmi3Error, "fmi3Error", "VR %d at index %d is our of range max is %d", vref, i, 4);
            return fmi3Error;
        }
        fmi3String val = values[i];
        print_log(instance, fmi3OK, "fmi3Ok", "Set##type_name VR %d at index %d", vref, i);
        // print_log(instance, fmi3OK, "fmi3Ok", "Set##type_name VR %d at index %d, value %s", vref, i,val);
        ((struct Model *) instance)->v_fmi3String[vref] = strdup(val);
    }
    return fmi3OK;
}


fmi3Status fmi3SetBinary(fmi3Instance instance, const fmi3ValueReference valueReferences[], size_t nValueReferences,
                         const size_t valueSizes[],
                         const fmi3Binary values[], size_t nValues) {
    for (int i = 0; i < nValueReferences; i++) {
        const fmi3ValueReference vref = valueReferences[i];
        if (vref < 0 || vref > 4) {
            print_log(instance, fmi3Error, "fmi3Error", "VR %d at index %d is our of range max is %d", vref, i, 4);
            return fmi3Error;
        }
        size_t binSize = valueSizes[i];
        fmi3Binary val = values[i];
        print_log(instance, fmi3OK, "fmi3Ok", "Set##type_name VR %d at index %d. Size %d", vref, i, binSize);

        struct Model *model = instance;

        if (model->v_fmi3Binary[vref] != 0) {
            //clean up last space used
            free((void *) model->v_fmi3Binary[vref]);
        }

        //allocate new space
        model->v_fmi3Binary[vref] = malloc(binSize);

        //store bytes
        memcpy((void *) model->v_fmi3Binary[vref], val, binSize);

        //record size for next fetch
        model->v_fmiBinarySizes[vref] = binSize;

        print_log(instance, fmi3OK, "fmi3Ok", "Set##type_name VR %d at index %d. Size %d. Recorded size: %d", vref, i,
                  binSize, ((struct Model *) instance)->v_fmiBinarySizes[vref]);
    }
    return fmi3OK;
}

fmi3Status fmi3SetClock(fmi3Instance instance, const fmi3ValueReference valueReferences[], size_t nValueReferences,
                        const fmi3Clock values[]) {
    for (int i = 0; i < nValueReferences; i++) {
        const fmi3ValueReference vref = valueReferences[i];
        if (vref < 0 || vref > 4) {
            print_log(instance, fmi3Error, "fmi3Error", "VR %d at index %d is our of range max is %d", vref, i, 4);
            return fmi3Error;
        }
        fmi3Clock val = values[i];
        print_log(instance, fmi3OK, "fmi3Ok", "Get##type_name VR %d at index %d", vref, i);
        ((struct Model *) instance)->v_fmi3Clock[vref] = val;
    }
    return fmi3OK;
}

//fmi3Status fmi3SetFloat32(fmi3Instance instance,
//                          const fmi3ValueReference valueReferences[],
//                          size_t nValueReferences,
//                          const fmi3Float32 values[],
//                          size_t nValues) {
//
//    return fmi3OK;
//}
//
//fmi3Status fmi3SetFloat64(fmi3Instance instance,
//                          const fmi3ValueReference valueReferences[],
//                          size_t nValueReferences,
//                          const fmi3Float64 values[],
//                          size_t nValues) {
//
//    return fmi3OK;
//}
//
//fmi3Status fmi3SetInt8(fmi3Instance instance,
//                       const fmi3ValueReference valueReferences[],
//                       size_t nValueReferences,
//                       const fmi3Int8 values[],
//                       size_t nValues) {
//
//    return fmi3OK;
//}
//
//fmi3Status fmi3SetUInt8(fmi3Instance instance,
//                        const fmi3ValueReference valueReferences[],
//                        size_t nValueReferences,
//                        const fmi3UInt8 values[],
//                        size_t nValues) {
//
//    return fmi3OK;
//}
//
//fmi3Status fmi3SetInt16(fmi3Instance instance,
//                        const fmi3ValueReference valueReferences[],
//                        size_t nValueReferences,
//                        const fmi3Int16 values[],
//                        size_t nValues) {
//
//    return fmi3OK;
//}
//
//fmi3Status fmi3SetUInt16(fmi3Instance instance,
//                         const fmi3ValueReference valueReferences[],
//                         size_t nValueReferences,
//                         const fmi3UInt16 values[],
//                         size_t nValues) {
//    return fmi3OK;
//}
//
//fmi3Status fmi3SetInt32(fmi3Instance instance,
//                        const fmi3ValueReference valueReferences[],
//                        size_t nValueReferences,
//                        const fmi3Int32 values[],
//                        size_t nValues) {
//    return fmi3OK;
//}
//
//fmi3Status fmi3SetUInt32(fmi3Instance instance,
//                         const fmi3ValueReference valueReferences[],
//                         size_t nValueReferences,
//                         const fmi3UInt32 values[],
//                         size_t nValues) {
//
//    return fmi3OK;
//}
//
//fmi3Status fmi3SetInt64(fmi3Instance instance,
//                        const fmi3ValueReference valueReferences[],
//                        size_t nValueReferences,
//                        const fmi3Int64 values[],
//                        size_t nValues) {
//
//    return fmi3OK;
//}
//
//fmi3Status fmi3SetUInt64(fmi3Instance instance,
//                         const fmi3ValueReference valueReferences[],
//                         size_t nValueReferences,
//                         const fmi3UInt64 values[],
//                         size_t nValues) {
//    return fmi3OK;
//}
//
//fmi3Status fmi3SetBoolean(fmi3Instance instance,
//                          const fmi3ValueReference valueReferences[],
//                          size_t nValueReferences,
//                          const fmi3Boolean values[],
//                          size_t nValues) {
//    return fmi3OK;
//}
//
//fmi3Status fmi3SetString(fmi3Instance instance,
//                         const fmi3ValueReference valueReferences[],
//                         size_t nValueReferences,
//                         const fmi3String values[],
//                         size_t nValues) {
//    return fmi3OK;
//}
//
//fmi3Status fmi3SetBinary(fmi3Instance instance,
//                         const fmi3ValueReference valueReferences[],
//                         size_t nValueReferences,
//                         const size_t valueSizes[],
//                         const fmi3Binary values[],
//                         size_t nValues) {
//
//    return fmi3OK;
//}
//
//fmi3Status fmi3SetClock(fmi3Instance instance,
//                        const fmi3ValueReference valueReferences[],
//                        size_t nValueReferences,
//                        const fmi3Clock values[]) {
//
//    return fmi3OK;
//}

fmi3Status fmi3GetNumberOfVariableDependencies(fmi3Instance instance,
                                               fmi3ValueReference valueReference,
                                               size_t *nDependencies) {
    *nDependencies = 5;
    return fmi3OK;
}

fmi3Status fmi3GetVariableDependencies(fmi3Instance instance,
                                       fmi3ValueReference dependent,
                                       size_t elementIndicesOfDependent[],
                                       fmi3ValueReference independents[],
                                       size_t elementIndicesOfIndependents[],
                                       fmi3DependencyKind dependencyKinds[],
                                       size_t nDependencies) {

    if (dependent == 9)
        return fmi3Error;


    for (int i = 0; i < nDependencies; i++) {
        dependencyKinds[i] = i;
        elementIndicesOfDependent[i] = i;
        independents[i] = i;
        elementIndicesOfIndependents[i] = i;
    }

    return fmi3OK;
}

fmi3Status fmi3GetFMUState(fmi3Instance instance, fmi3FMUState *FMUState) {

    memcpy(FMUState, instance, sizeof(struct Model));
    return fmi3OK;
}

fmi3Status fmi3SetFMUState(fmi3Instance instance, fmi3FMUState FMUState) {

    memcpy(instance, FMUState, sizeof(struct Model));
    return fmi3OK;
}

fmi3Status fmi3FreeFMUState(fmi3Instance instance, fmi3FMUState *FMUState) {

    free(FMUState);
    return fmi3OK;
}

fmi3Status fmi3SerializedFMUStateSize(fmi3Instance instance,
                                      fmi3FMUState FMUState,
                                      size_t *size) {


    return fmi3Discard;
}

fmi3Status fmi3SerializeFMUState(fmi3Instance instance,
                                 fmi3FMUState FMUState,
                                 fmi3Byte serializedState[],
                                 size_t size) {


    return fmi3Discard;
}

fmi3Status fmi3DeSerializeFMUState(fmi3Instance instance,
                                   const fmi3Byte serializedState[],
                                   size_t size,
                                   fmi3FMUState *FMUState) {


    return fmi3Discard;
}

fmi3Status fmi3GetDirectionalDerivative(fmi3Instance instance,
                                        const fmi3ValueReference unknowns[],
                                        size_t nUnknowns,
                                        const fmi3ValueReference knowns[],
                                        size_t nKnowns,
                                        const fmi3Float64 seed[],
                                        size_t nSeed,
                                        fmi3Float64 sensitivity[],
                                        size_t nSensitivity) {


    return fmi3OK;
}

fmi3Status fmi3GetAdjointDerivative(fmi3Instance instance,
                                    const fmi3ValueReference unknowns[],
                                    size_t nUnknowns,
                                    const fmi3ValueReference knowns[],
                                    size_t nKnowns,
                                    const fmi3Float64 seed[],
                                    size_t nSeed,
                                    fmi3Float64 sensitivity[],
                                    size_t nSensitivity) {


    return fmi3OK;
}

fmi3Status fmi3EnterConfigurationMode(fmi3Instance instance) {


    return fmi3OK;
}

fmi3Status fmi3ExitConfigurationMode(fmi3Instance instance) {


    return fmi3OK;
}

fmi3Status fmi3GetIntervalDecimal(fmi3Instance instance,
                                  const fmi3ValueReference valueReferences[],
                                  size_t nValueReferences,
                                  fmi3Float64 intervals[],
                                  fmi3IntervalQualifier qualifiers[]) {

    if (valueReferences[0] == 9)
        return fmi3Error;


    for (int i = 0; i < nValueReferences; ++i) {
        intervals[i] = i * 0.1;
        qualifiers[i] = i;
    }

    return fmi3OK;
}

fmi3Status fmi3GetIntervalFraction(fmi3Instance instance,
                                   const fmi3ValueReference valueReferences[],
                                   size_t nValueReferences,
                                   fmi3UInt64 intervalCounters[],
                                   fmi3UInt64 resolutions[],
                                   fmi3IntervalQualifier qualifiers[]) {

    if (valueReferences[0] == 9)
        return fmi3Error;


    for (int i = 0; i < nValueReferences; ++i) {
        intervalCounters[i] = i;
        resolutions[i] = i;
        qualifiers[i] = i;
    }


    return fmi3OK;
}

fmi3Status fmi3GetShiftDecimal(fmi3Instance instance,
                               const fmi3ValueReference valueReferences[],
                               size_t nValueReferences,
                               fmi3Float64 shifts[]) {


    if (valueReferences[0] == 9)
        return fmi3Error;


    for (int i = 0; i < nValueReferences; ++i) {
        shifts[i] = i * 0.1;
    }


    return fmi3OK;
}

fmi3Status fmi3GetShiftFraction(fmi3Instance instance,
                                const fmi3ValueReference valueReferences[],
                                size_t nValueReferences,
                                fmi3UInt64 shiftCounters[],
                                fmi3UInt64 resolutions[]) {


    if (valueReferences[0] == 9)
        return fmi3Error;


    for (int i = 0; i < nValueReferences; ++i) {
        shiftCounters[i] = i;
        resolutions[i] = i;
    }

    return fmi3OK;
}

fmi3Status fmi3SetIntervalDecimal(fmi3Instance instance,
                                  const fmi3ValueReference valueReferences[],
                                  size_t nValueReferences,
                                  const fmi3Float64 intervals[]) {

    if (intervals[0] == 1.1 && intervals[1] == 2.2)
        return fmi3OK;
    return fmi3Error;
}

fmi3Status fmi3SetIntervalFraction(fmi3Instance instance,
                                   const fmi3ValueReference valueReferences[],
                                   size_t nValueReferences,
                                   const fmi3UInt64 intervalCounters[],
                                   const fmi3UInt64 resolutions[]) {

    if (intervalCounters[0] == 1 && intervalCounters[1] == 2 && resolutions[0] == 3 && resolutions[1] == 4)
        return fmi3OK;
    return fmi3Error;
}

fmi3Status fmi3EvaluateDiscreteStates(fmi3Instance instance) {
    return fmi3OK;
}

fmi3Status fmi3UpdateDiscreteStates(fmi3Instance instance,
                                    fmi3Boolean *discreteStatesNeedUpdate,
                                    fmi3Boolean *terminateSimulation,
                                    fmi3Boolean *nominalsOfContinuousStatesChanged,
                                    fmi3Boolean *valuesOfContinuousStatesChanged,
                                    fmi3Boolean *nextEventTimeDefined,
                                    fmi3Float64 *nextEventTime) {

    *nextEventTimeDefined = true;
    *nextEventTime = 99.99;
    return fmi3OK;

}

/***************************************************
 Functions for Model Exchange
 ****************************************************/

fmi3Status fmi3EnterContinuousTimeMode(fmi3Instance instance) {


    return fmi3OK;
}

fmi3Status fmi3CompletedIntegratorStep(fmi3Instance instance,
                                       fmi3Boolean noSetFMUStatePriorToCurrentPoint,
                                       fmi3Boolean *enterEventMode,
                                       fmi3Boolean *terminateSimulation) {


    return fmi3OK;
}

/* Providing independent variables and re-initialization of caching */
fmi3Status fmi3SetTime(fmi3Instance instance, fmi3Float64 time) {

    if (time == 99.99)
        return fmi3OK;
    return fmi3Error;
}

fmi3Status fmi3SetContinuousStates(fmi3Instance instance,
                                   const fmi3Float64 continuousStates[],
                                   size_t nContinuousStates) {


    return fmi3OK;
}

/* Evaluation of the model equations */
fmi3Status fmi3GetContinuousStateDerivatives(fmi3Instance instance,
                                             fmi3Float64 derivatives[],
                                             size_t nContinuousStates) {

    derivatives[0] = 99.99;


    return fmi3OK;
}

fmi3Status fmi3GetEventIndicators(fmi3Instance instance,
                                  fmi3Float64 eventIndicators[],
                                  size_t nEventIndicators) {
    eventIndicators[0] = 99.99;

    return fmi3OK;
}

fmi3Status fmi3GetContinuousStates(fmi3Instance instance,
                                   fmi3Float64 continuousStates[],
                                   size_t nContinuousStates) {

    continuousStates[0] = 99.99;
    return fmi3OK;
}

fmi3Status fmi3GetNominalsOfContinuousStates(fmi3Instance instance,
                                             fmi3Float64 nominals[],
                                             size_t nContinuousStates) {

    nominals[0] = 99.99;
    return fmi3OK;
}

fmi3Status fmi3GetNumberOfEventIndicators(fmi3Instance instance,
                                          size_t *nEventIndicators) {

    *nEventIndicators = 99;
    return fmi3OK;
}

fmi3Status fmi3GetNumberOfContinuousStates(fmi3Instance instance,
                                           size_t *nContinuousStates) {

    *nContinuousStates = 99;
    return fmi3OK;
}

/***************************************************
 Functions for Co-Simulation
 ****************************************************/

fmi3Status fmi3EnterStepMode(fmi3Instance instance) {


    return fmi3OK;
}

fmi3Status fmi3GetOutputDerivatives(fmi3Instance instance,
                                    const fmi3ValueReference valueReferences[],
                                    size_t nValueReferences,
                                    const fmi3Int32 orders[],
                                    fmi3Float64 values[],
                                    size_t nValues) {


    values[0] = 99.99;
    values[1] = 98.98;

    return fmi3OK;
}

fmi3Status fmi3DoStep(fmi3Instance instance,
                      fmi3Float64 currentCommunicationPoint,
                      fmi3Float64 communicationStepSize,
                      fmi3Boolean noSetFMUStatePriorToCurrentPoint,
                      fmi3Boolean *eventHandlingNeeded,
                      fmi3Boolean *terminateSimulation,
                      fmi3Boolean *earlyReturn,
                      fmi3Float64 *lastSuccessfulTime) {


    return fmi3OK;
}

fmi3Status fmi3ActivateModelPartition(fmi3Instance instance,
                                      fmi3ValueReference clockReference,
                                      fmi3Float64 activationTime) {

    return fmi3OK;
}
