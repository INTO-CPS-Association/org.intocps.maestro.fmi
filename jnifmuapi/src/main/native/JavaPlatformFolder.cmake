
include(TargetArch.cmake)


function(get_java_osarch_folder output_var)

    if (UNIX AND NOT APPLE)
        set(OS_NAME "Linux")
    elseif (APPLE)
        set(OS_NAME "Mac")
    elseif (WIN32)
        set(OS_NAME "Win")
    endif ()
    target_architecture(arch)
    set(${output_var} "${OS_NAME}-${arch}" PARENT_SCOPE)
endfunction()