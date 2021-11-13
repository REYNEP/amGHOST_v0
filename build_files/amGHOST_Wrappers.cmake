function(WRAPPER_FindOpenGL)       #e.g. call_hint = amGHOST   [which lib is calling this]
    FIND_PACKAGE(OpenGL)
    
    if (NOT OPENGL_FOUND)
        if (NOT OPENGL_OPENGL_FOUND)
            message(FATAL_ERROR "[amGHOST_BUILD_OPENGL]    FIND_PACKAGE(OpenGL) couldn't Find any OpenGL Library in System")
        else()
            message(STATUS "[amGHOST_BUILD_OPENGL]    FIND_PACKAGE(OpenGL) couldn't find All OpenGL Components but OpenGL Library [OPENGL_OPENGL_FOUND]")
        endif()
    endif()
endfunction()


macro(ASSERT_listAppend
    list
    XD
)
    if("${${XD}}" STREQUAL "")
        message(STATUS "[amGHOST DEP]: ${XD} is not DEFINED.... ")
    else()
        list(APPEND ${list}  ${${XD}})
    endif()
endmacro()

macro(ASSERT_libAppend
    XD
)
    if (amGHOST_REQUIRE_LINKING)      # You might not Want to Link amGHOST Deps into amGHOST.lib
        ASSERT_listAppend(  LIB   ${XD}  )
    else()
        list(APPEND     LIB    ${${XD}}  )
    endif()
endmacro()


function(ASSERT_Vulkan
    _inc_
)
    find_path(VULKAN_INC_TMP
        NAMES
          vulkan/vulkan.h
        PATHS
          ${${_inc_}}
        DOC
          "We just simply Check If this will work.... I mean Why not"
        NO_DEFAULT_PATH
    )
    if ("${VULKAN_INC_TMP}" MATCHES "NOTFOUND$")
        message(STATUS "[amGHOST]: VULKAN_INC Variable is not set right.... The dir doesn't have vulkan/vulkan.h")
        message(FATAL_ERROR "[BYE BYE]")
    endif()
    unset(VULKAN_INC_TMP CACHE)
endfunction()

function(ASSERT_GLEW
    _inc_
)
    find_path(GLEW_INC_TMP
        NAMES
          GL/glew.h
        PATHS
          ${${_inc_}}
        DOC
          "We just simply Check If this will work.... I mean Why not"
        NO_DEFAULT_PATH
    )

    if ("${GLEW_INC_TMP}" MATCHES "NOTFOUND$")
        message(STATUS "[amGHOST]: GLEW Variable is not set right.... The dir doesn't have GL/glew.h")
        message(FATAL_ERROR "[BYE BYE]")
    endif()
    unset(GLEW_INC_TMP CACHE)
endfunction()