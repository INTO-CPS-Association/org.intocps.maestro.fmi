# /*
#  * This file is part of the INTO-CPS toolchain.
#  *
#  * Copyright (c) 2017-CurrentYear, INTO-CPS Association,
#  * c/o Professor Peter Gorm Larsen, Department of Engineering
#  * Finlandsgade 22, 8200 Aarhus N.
#  *
#  * All rights reserved.
#  *
#  * THIS PROGRAM IS PROVIDED UNDER THE TERMS OF GPL VERSION 3 LICENSE OR
#  * THIS INTO-CPS ASSOCIATION PUBLIC LICENSE VERSION 1.0.
#  * ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS PROGRAM CONSTITUTES
#  * RECIPIENT'S ACCEPTANCE OF THE OSMC PUBLIC LICENSE OR THE GPL
#  * VERSION 3, ACCORDING TO RECIPIENTS CHOICE.
#  *
#  * The INTO-CPS toolchain  and the INTO-CPS Association Public License
#  * are obtained from the INTO-CPS Association, either from the above address,
#  * from the URLs: http://www.into-cps.org, and in the INTO-CPS toolchain distribution.
#  * GNU version 3 is obtained from: http://www.gnu.org/copyleft/gpl.html.
#  *
#  * This program is distributed WITHOUT ANY WARRANTY; without
#  * even the implied warranty of  MERCHANTABILITY or FITNESS FOR
#  * A PARTICULAR PURPOSE, EXCEPT AS EXPRESSLY SET FORTH IN THE
#  * BY RECIPIENT SELECTED SUBSIDIARY LICENSE CONDITIONS OF
#  * THE INTO-CPS ASSOCIATION.
#  *
#  * See the full INTO-CPS Association Public License conditions for more details.
#  */
#
# /*
#  * Author:
#  *		Kenneth Lausdahl
#  */


import argparse
import xml.etree.ElementTree as ET
from pathlib import Path
import shutil

parser = argparse.ArgumentParser()
parser.add_argument('--build-description', dest='build', type=str, required=False)
parser.add_argument('--model-description', dest='model', type=str, required=False)
parser.add_argument('--template-dir', dest="template_dir", type=str, required=True)
parser.add_argument('--output-dir', dest='output', type=str, required=True)
parser.add_argument('--fmi-headers', dest='fmi', type=str, required=True)

args = parser.parse_args()

if args.build:
    root = ET.parse(args.build).getroot()

    name = root.find('./BuildConfiguration').attrib['modelIdentifier']
    source_files = [e.attrib['name'] for e in root.findall("./BuildConfiguration/SourceFileSet/SourceFile")]
    defines = [(e.attrib['name'], e.attrib['value'] if 'value' in e.attrib else None) for e in
               root.findall("./BuildConfiguration/SourceFileSet/PreprocessorDefinition")]
    includes = [e.attrib['name'] for e in
                root.findall("./BuildConfiguration/SourceFileSet/IncludeDirectory")]

    includes.append(args.fmi)
    print(name)
    print(source_files)
    print(defines)
    defines.append(('DISABLE_PREFIX', None))


    def make_output_relative(input_path_name):
        p = Path(input_path_name)
        return str(Path(args.build).parent / p)


    for s in source_files:
        print(make_output_relative(s))
elif args.model:
    root = ET.parse(args.build).getroot()
    name = root.find('./fmiModelDescription/CoSimulation').attrib['modelIdentifier']
    source_files = """file(GLOB C_FILES sources/*.c sources/**/*.c)
                      file(GLOB CPP_FILES sources/*.cpp sources/**/*.cpp)
                      SET(SOURCES ${C_FILES} ${CPP_FILES})
                      """

    defines = "add_definitions(-DFMI_VERSION=2)"
    includes = "include_directories(" + args.fmi + ")"

    name = args.name
else:
    raise "Either build or model description must be provided"
    exit(1)

with open(Path(args.template_dir) / 'CMakeLists_template.txt', 'r') as file:
    data = file.read().replace('FMU_NAME', name).replace('##DEFINITIONS##', 'add_definitions(' + " ".join(
        ["-D" + str(d[0]) + ("=" + str(d[1]) if d[1] else "") for d in defines]) + ')').replace('##SOURCES##',
                                                                                                'set(SOURCES ' + " ".join(
                                                                                                    [
                                                                                                        make_output_relative(
                                                                                                            p) for p in
                                                                                                        source_files]) + ')').replace(
        '##INCLUDES##',
        'include_directories( ' + " ".join(
            [make_output_relative(p) for p in
             includes]) + ')')

    Path(args.output).mkdir(parents=True, exist_ok=True)
    with open(Path(args.output) / 'CMakeLists.txt', 'w') as out:
        out.write(data)

for name in ['TargetArch.cmake', 'FmiBinaryFolder.cmake']:
    shutil.copyfile(Path(args.template_dir) / name, Path(args.output) / name)

if args.model:
    shutil.copyfile(Path(args.model), Path(args.output) / 'modelDescription.xml')
