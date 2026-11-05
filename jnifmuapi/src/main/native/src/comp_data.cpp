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

#include "comp_data.h"

#include <map>
#include <mutex>

using namespace std;

static map<void *, HashmapNode *> g_callbackMap;
static map<fmi2Component, CompHashmapNode *> g_compMap;
static map<FMU *, size_t> g_fmus;
static mutex g_mutex;

extern "C" void storeCallbackHandler(HashmapNode *callback) {
  lock_guard<mutex> lock(g_mutex);
  g_callbackMap[callback] = callback;
}

extern "C" void storeComponent(CompHashmapNode *comp) {
  if (comp->comp == NULL) return;
  lock_guard<mutex> lock(g_mutex);
  g_compMap[comp->comp] = comp;
}

extern "C" void freeCallbackHandler(JNIEnv *env, HashmapNode *callback) {
  if (callback == NULL) return;
  lock_guard<mutex> lock(g_mutex);
  g_callbackMap.erase(callback);
  if (callback->callbackObj != NULL) {
    env->DeleteGlobalRef(callback->callbackObj);
  }
  free(callback->name);
  free(callback);
}

extern "C" void *getCallbackEnvironment(FMU *fmu, const char *instanceName) {
  if (instanceName == NULL) return NULL;
  lock_guard<mutex> lock(g_mutex);
  for (const auto &entry : g_callbackMap) {
    if (entry.second->fmu == fmu && strcmp(entry.second->name, instanceName) == 0) {
      return entry.first;
    }
  }
  return NULL;
}

extern "C" JavaVM *getCallbackVm(void *componentEnvironment) {
  lock_guard<mutex> lock(g_mutex);
  auto it = g_callbackMap.find(componentEnvironment);
  return it == g_callbackMap.end() ? NULL : it->second->g_vm;
}

extern "C" jobject getCallbackReference(JNIEnv *env, void *componentEnvironment,
                                        jmethodID *method) {
  lock_guard<mutex> lock(g_mutex);
  auto it = g_callbackMap.find(componentEnvironment);
  if (it == g_callbackMap.end()) return NULL;
  *method = it->second->callbackMethod;
  // Keep the receiver alive even if another thread frees the component.
  return env->NewLocalRef(it->second->callbackObj);
}

extern "C" void freeComponentNode(JNIEnv *env, CompHashmapNode *comp) {
  if (comp == NULL) return;
  freeCallbackHandler(env, comp->callback);
  free(comp->callbackFunctions);
  free(comp);
}

extern "C" void freeComp(JNIEnv *env, fmi2Component comp) {
  CompHashmapNode *node;
  {
    lock_guard<mutex> lock(g_mutex);
    auto it = g_compMap.find(comp);
    if (it == g_compMap.end()) return;
    node = it->second;
    g_compMap.erase(it);
  }
  freeComponentNode(env, node);
}

extern "C" int isComponentValid(FMU *fmu, fmi2Component comp) {
  lock_guard<mutex> lock(g_mutex);
  auto it = g_compMap.find(comp);
  return g_fmus.count(fmu) != 0 && it != g_compMap.end() &&
         it->second->fmu == fmu && !it->second->freeing;
}

extern "C" CompHashmapNode *beginFreeComponent(FMU *fmu, fmi2Component comp) {
  lock_guard<mutex> lock(g_mutex);
  auto it = g_compMap.find(comp);
  if (g_fmus.count(fmu) == 0 || it == g_compMap.end() ||
      it->second->fmu != fmu || it->second->freeing) {
    return NULL;
  }
  CompHashmapNode *node = it->second;
  node->freeing = 1;
  return node;
}

extern "C" void storeFmu(FMU *fmu) {
  lock_guard<mutex> lock(g_mutex);
  g_fmus[fmu] = 0;
}

extern "C" int isFmuLoaded(FMU *fmu) {
  lock_guard<mutex> lock(g_mutex);
  return g_fmus.count(fmu) != 0;
}

extern "C" int hasFmuComponents(FMU *fmu) {
  lock_guard<mutex> lock(g_mutex);
  auto it = g_fmus.find(fmu);
  if (it != g_fmus.end() && it->second != 0) return 1;
  for (const auto &entry : g_compMap) {
    if (entry.second->fmu == fmu) return 1;
  }
  return 0;
}

extern "C" int beginFmuInstantiation(FMU *fmu) {
  lock_guard<mutex> lock(g_mutex);
  auto it = g_fmus.find(fmu);
  if (it == g_fmus.end()) return 0;
  ++it->second;
  return 1;
}

extern "C" void endFmuInstantiation(FMU *fmu) {
  lock_guard<mutex> lock(g_mutex);
  --g_fmus.at(fmu);
}

extern "C" void removeFmu(FMU *fmu) {
  lock_guard<mutex> lock(g_mutex);
  g_fmus.erase(fmu);
}
