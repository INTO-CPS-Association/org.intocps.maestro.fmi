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

#include "fmi3.h"
#define BUFSIZE 4096

void fmu3Logger(fmi3InstanceEnvironment instanceEnvironment,
                fmi3String instanceName, fmi3Status status, fmi3String category,
                fmi3String message, ...);
void fmu3CallbackIntermediateUpdate(
    fmi3InstanceEnvironment instanceEnvironment,
    fmi3Float64 intermediateUpdateTime, fmi3Boolean clocksTicked,
    fmi3Boolean intermediateVariableSetRequested,
    fmi3Boolean intermediateVariableGetAllowed,
    fmi3Boolean intermediateStepFinished, fmi3Boolean canReturnEarly,
    fmi3Boolean *earlyReturnRequested, fmi3Float64 *earlyReturnTime);

void fmu3CallbackLockPreemption();
void fmu3CallbackUnlockPreemption();

int loadDll3(const char *dllPath, FMU3 *fmu);
int error3(const char *message);
