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
* from the URLs: http://www.into-cps.org, and in the INTO-CPS toolchain distribution.
* GNU version 3 is obtained from: http://www.gnu.org/copyleft/gpl.html.
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
#include <string>

using namespace std;

static map<string, HashmapNode *> g_callbackMap;

static map<fmi2Component, CompHashmapNode *> g_compMap;

extern "C" void storeCallbackHandler(HashmapNode *callback) {
  g_callbackMap[string(callback->name)] = callback;
}

extern "C" void storeComponent(CompHashmapNode *comp) {
  if (comp->comp == NULL) return;

  g_compMap[comp->comp] = comp;
}

extern "C" void freeComp(JNIEnv *env, fmi2Component comp) {
  auto itr = g_compMap.find(comp);

  CompHashmapNode *cn = NULL;
  if (itr != g_compMap.end()) {
    cn = itr->second;
  }

  if (cn == NULL) {
    return;
  }

  if (cn->callback != NULL) {
    // first remove callback from the list which is searched by the cache
    // callback

    for (auto it = g_callbackMap.begin(); it != g_callbackMap.end(); ++it) {
      if (it->second == cn->callback) {
        string key = it->first;
        g_callbackMap.erase(it);
        break;
      }
    }

    // This is concurrency issue, technically it is possible to delete
    // the object here at the same time as fmu_loggerCache tries to use it
    if (cn->callback->callbackObj != NULL) {
      env->DeleteGlobalRef(cn->callback->callbackObj);
      cn->callback->callbackObj = NULL;
    }

    free(cn->callback);
    cn->callback = NULL;
  }
  if (cn->callbackFunctions != NULL) {
    free(cn->callbackFunctions);
    cn->callbackFunctions = NULL;
  }
  g_compMap.erase(itr);
  free(cn);
}

extern "C" HashmapNode *getCallbackHandler(fmi2String instanceName) {
  auto itr = g_callbackMap.find(string(instanceName));

  if (itr == g_callbackMap.end()) {
    return NULL;
  } else {
    return itr->second;
  }
}
