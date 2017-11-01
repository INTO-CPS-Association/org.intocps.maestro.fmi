cmake_minimum_required (VERSION 2.8.5)
project (fmuapi C CXX)

include(clang-format.cmake)

include(CheckCXXCompilerFlag)

set(CMAKE_VERBOSE_MAKEFILE on)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


include_directories($ENV{JAVA_HOME}/include)

if (WIN32)
	#windows	
	#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
 	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--add-stdcall-alias")
	include_directories($ENV{JAVA_HOME}/include/win32)
endif() 

if (UNIX)

	if(APPLE)
		include_directories($ENV{JAVA_HOME}/include/darwin)
	else()
		include_directories($ENV{JAVA_HOME}/include/linux)
	endif()
endif()

#include_directories(${libshmfmi_SOURCE_DIR})
include_directories(include src/external/shared src/external/shared/fmi/include )

#add_library(${PROJECT_NAME} SHARED src/main/native/sharedmemory.cpp ${PROTO_SRCS} ${PROTO_HDRS})

file(GLOB C_FILES src/*.c) # ../includes/templates/*.c)
file(GLOB CPP_FILES src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${C_FILES} ${CPP_FILES})

add_definitions(-DSTANDALONE_XML_PARSER -DLIBXML_STATIC -DFMI_COSIMULATION)

#target_link_libraries(${PROJECT_NAME} libshmfmi)
#target_link_libraries(${PROJECT_NAME} ${libshmfmi_LIBS})


foreach(LETTER ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES})
	
if(EXISTS "${LETTER}/libstdc++.a")
#   message (STATUS "${LETTER}")
	set(STDCPP_LIBRARY "${LETTER}/libstdc++.a")
else()
#  message (STATUS "${LETTER} ---- not these")
endif()
endforeach()

message(STATUS "Static linking with libstdc++.a in ${STDCPP_LIBRARY}")
target_link_libraries(${PROJECT_NAME} ${STDCPP_LIBRARY})

#find_library(PTHREAD_LIBRARY NAMES libpthread.a)
#target_link_libraries(${PROJECT_NAME} ${PTHREAD_LIBRARY})


if (WIN32)
	#windows	
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libstdc++ -static-libgcc")

	SET_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
endif() 

