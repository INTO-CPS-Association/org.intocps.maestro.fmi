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
import platform

parser = argparse.ArgumentParser()
parser.add_argument('--build-description', dest='build', type=str, required=False)
parser.add_argument('--model-description', dest='model', type=str, required=False)
parser.add_argument('--template-dir', dest="template_dir", type=str, required=True)
parser.add_argument('--output-dir', dest='output', type=str, required=True)
parser.add_argument('--fmi-headers', dest='fmi', type=str, required=True)
parser.add_argument('--platform', dest='platform', type=str,
                    default=str(platform.machine()) + '-' + str(platform.system()).lower())

args = parser.parse_args()


def make_output_relative(input_path_name):
    p = Path(input_path_name)
    return str(Path(args.build).parent / p)


if args.build:
    root = ET.parse(args.build).getroot()

    for model_identifier in [c.attrib['modelIdentifier'] for c in root.findall('.//BuildConfiguration')]:
        build_configs = root.findall("./BuildConfiguration[@modelIdentifier='" + model_identifier + "']")

        build_base = [c for c in build_configs if 'platform' not in c.attrib]
        build_dedicated = [c for c in build_configs if 'platform' in c.attrib and args.platform in c.attrib['platform']]

        if len(build_dedicated) == 0:
            build_conf = build_base[0]
        else:
            build_conf = build_dedicated[0]

        source_file_sets: [ET.ElementTree] = [e for e in build_conf.findall('./SourceFileSet')]

        # build targets
        cmkae_lines = ""
        for idx, source_file_set in enumerate(source_file_sets):
            target_name = model_identifier + '_' + str(idx)

            includes = [make_output_relative(e.attrib['name']) for e in
                        source_file_set.findall("./IncludeDirectory")]
            includes.append(args.fmi)

            sources = [make_output_relative(e.attrib['name']) for e in source_file_set.findall("./SourceFile")]

            language = source_file_set.attrib['language'] if 'language' in source_file_set.attrib else None

            defines = [(e.attrib['name'], e.attrib['value'] if 'value' in e.attrib else None) for e in
                       source_file_set.findall("./PreprocessorDefinition")]
            defines.append(('DISABLE_PREFIX', None))

            define_string = " ".join([str(d[0]) + ("=" + str(d[1]) if d[1] else "") for d in defines])

            cmkae_lines += 'add_library("%s" OBJECT %s)\n' % (
                target_name, " ".join(sources))

            cmkae_lines += 'set_target_properties("%s" PROPERTIES POSITION_INDEPENDENT_CODE ON, PREFIX "")\n' % target_name
            if len(includes) > 0:
                cmkae_lines += 'target_include_directories("%s" PUBLIC %s)\n' % (target_name, " ".join(includes))

            if language and 'C++' in language:
                cmkae_lines += 'set_property(TARGET %s PROPERTY CXX_STANDARD %s)\n' % (target_name, language[3:])
            elif language and 'C' in language:
                cmkae_lines += 'set_property(TARGET %s PROPERTY C_STANDARD %s)\n' % (target_name, language[1:])

            if len(defines) > 0:
                cmkae_lines += 'target_compile_definitions("%s" PUBLIC %s)\n' % (target_name, define_string)

        cmkae_lines += 'file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/s.c "")\n'
        cmkae_lines += 'add_library("%s" SHARED s.c)\n' % model_identifier
        cmkae_lines += 'set_target_properties("%s" PROPERTIES POSITION_INDEPENDENT_CODE ON, PREFIX "")\n' % model_identifier
        cmkae_lines += 'target_link_libraries("%s" PUBLIC %s)\n' % (
            model_identifier, " ".join([model_identifier + "_" + str(idx) for idx, n in enumerate(source_file_sets)]))
        cmkae_lines += 'SET(output_folder "%s")\n' % args.platform

        # dump ##GENERATED##
        with open(Path(args.template_dir) / 'CMakeLists_template3.txt', 'r') as file:
            data = file.read().replace('FMU_NAME', model_identifier).replace('##GENERATED##', cmkae_lines)
            Path(args.output).mkdir(parents=True, exist_ok=True)
            out_path = Path(args.output) / 'CMakeLists.txt'
            with open(out_path, 'w') as out:
                print("Writing file: %s" % str(out_path))
                out.write(data)

    print("Done")
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

    with open(Path(args.template_dir) / 'CMakeLists_template.txt', 'r') as file:
        data = file.read().replace('FMU_NAME', name).replace('##DEFINITIONS##', 'add_definitions(' + " ".join(
            ["-D" + str(d[0]) + ("=" + str(d[1]) if d[1] else "") for d in defines]) + ')').replace('##SOURCES##',
                                                                                                    'set(SOURCES ' + " ".join(
                                                                                                        [
                                                                                                            make_output_relative(
                                                                                                                p) for p
                                                                                                            in
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

else:
    raise "Either build or model description must be provided"
    exit(1)

if args.model:
    shutil.copyfile(Path(args.model), Path(args.output) / 'modelDescription.xml')
