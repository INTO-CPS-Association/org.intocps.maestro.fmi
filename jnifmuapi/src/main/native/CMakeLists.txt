cmake_minimum_required(VERSION 3.1.3)
project(fmuapi
        VERSION 3.0.0
        LANGUAGES C CXX
        )

configure_file(src/config.h.in config.h)

include(JavaJni.cmake)
add_java_jni()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

include(clang-format.cmake)

include(CheckCXXCompilerFlag)

set(CMAKE_MACOSX_RPATH 1)

set(CMAKE_VERBOSE_MAKEFILE on)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if (COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif (COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else ()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif ()


include_directories($ENV{JAVA_HOME}/include)

if (WIN32)
    #windows
    #set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--add-stdcall-alias")
    include_directories($ENV{JAVA_HOME}/include/win32)
endif ()

if (UNIX)

    if (APPLE)
        include_directories($ENV{JAVA_HOME}/include/darwin)
    else ()
        include_directories($ENV{JAVA_HOME}/include/linux)
    endif ()
endif ()

#include_directories(${libshmfmi_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src/fmi3 ${CMAKE_CURRENT_SOURCE_DIR}/src/external/shared ${CMAKE_CURRENT_SOURCE_DIR}/src/external/shared/fmi2/headers ${CMAKE_CURRENT_SOURCE_DIR}/src/external/shared/fmi3/headers)

#add_library(${PROJECT_NAME} SHARED src/main/native/sharedmemory.cpp ${PROTO_SRCS} ${PROTO_HDRS})

file(GLOB C_FILES src/*.c) # ../includes/templates/*.c)
file(GLOB CPP_FILES src/*.cpp)

file(GLOB C_FILES_3 src/fmi3/*.c) # ../includes/templates/*.c)
file(GLOB CPP_FILES_3 src/fmi3/*.cpp)

add_library(${PROJECT_NAME} SHARED ${C_FILES} ${CPP_FILES} ${C_FILES_3} ${CPP_FILES_3})

add_definitions(-DSTANDALONE_XML_PARSER -DLIBXML_STATIC -DFMI_COSIMULATION)

#target_link_libraries(${PROJECT_NAME} libshmfmi)
#target_link_libraries(${PROJECT_NAME} ${libshmfmi_LIBS})


foreach (LETTER ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES})

    if (EXISTS "${LETTER}/libstdc++.a")
        #   message (STATUS "${LETTER}")
        set(STDCPP_LIBRARY "${LETTER}/libstdc++.a")
    else ()
        #  message (STATUS "${LETTER} ---- not these")
    endif ()
endforeach ()

message(STATUS "Static linking with libstdc++.a in ${STDCPP_LIBRARY}")
target_link_libraries(${PROJECT_NAME} ${STDCPP_LIBRARY})

#find_library(PTHREAD_LIBRARY NAMES libpthread.a)
#target_link_libraries(${PROJECT_NAME} ${PTHREAD_LIBRARY})


if (WIN32)
    #windows
    
		
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libstdc++ -static-libgcc -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic")

    SET_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
endif ()

include(JavaPlatformFolder.cmake)
get_java_osarch_folder(output_folders)

foreach(output_folder ${output_folders})
message("${output_folder}")


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/../../../target/classes/lib/${output_folder}/"
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> "${CMAKE_CURRENT_SOURCE_DIR}/../../../target/classes/lib/${output_folder}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        DEPENDS ${PROJECT_NAME}
        )
endforeach()
