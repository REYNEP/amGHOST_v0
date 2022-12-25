# ***** BEGIN GPL LICENSE BLOCK *****
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# The Original Code is Copyright (C) 2021, REYNEP
#
# ***** END GPL LICENSE BLOCK *****
#

cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(amGHOST_V0.1beta)

  if (NOT (${CMAKE_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR}))
    message(NOTICE "\nadd_subdirectory: amGHOST")
  endif()

  set(CMAKE_CXX_STANDARD 17)                            # static inline
  set(CMAKE_CXX_STANDARD_REQUIRED ON)

  # ON UNIX, Terminal Output with Colors....
  if     (CMAKE_CXX_COMPILER_ID   STREQUAL   "GNU")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
  elseif (CMAKE_CXX_COMPILER_ID   STREQUAL   "Clang")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
  endif()


#[[=======================================================================================================
      ╺┳┓┏━┓┏━╸╻ ╻┏┳┓┏━╸┏┓╻╺┳╸┏━┓╺┳╸╻┏━┓┏┓╻
       ┃┃┃ ┃┃  ┃ ┃┃┃┃┣╸ ┃┗┫ ┃ ┣━┫ ┃ ┃┃ ┃┃┗┫
      ╺┻┛┗━┛┗━╸┗━┛╹ ╹┗━╸╹ ╹ ╹ ╹ ╹ ╹ ╹┗━┛╹ ╹

#     if U Want to do add_subdirectory(amGHOST)
  ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
  - See build_files/amGHOST_Options.cmake       [Also DOCS there]
  - See build_files/amGHOST_Variables.cmake     [Also DOCS there]
  - See extern/CMakeLists.txt
  - DO  list(APPEND CMAKE_MODULE_PATH ...)      [if you wanna make your own amGHOST_Variables/Options.cmake]
 
#    The Flow of this File
  ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
  - project & cmake_minimum on top, maybe some Compile related settings
  - Documentation BLOCK
  - Options DECLARATIONS - are UNSET at EOF [end-of-file]
  - include all the modules, amGHOST_<Module>.cmake
  - Options will be overwritten by the values in amGHOST_Options.cmake

  - VAR_<whatever> will be loaded from amGHOST_Variables.cmake
  - Check if all the include(amGHOST_<Module>) worked ok.... and report

  - add_subdirectory() on GLEW    [IF OpenGL enabled]

  - Finally the actual COOL Stuffs.... iam pretty sure you will understand the rest
#=======================================================================================================]]




#================================================================
# Options DECLARATIONS
# NOTE: Check build_files/amGHOST_Options.cmake
# REMEMBER TO MAKE THE CHANGES THERE.... not only here....
option(amGHOST_BUILD_STATIC_EXTERN    "[TBA...WIP] Statically link Extern libraries, such as VULKAN, GLEW, etc" ON)
option(amGHOST_BUILD_WIN32            "[I don't think we need to do this] For Windows" ON)
option(amGHOST_BUILD_X11              "X11 [UNIX] " OFF)
option(amGHOST_BUILD_XCB              "XCB [UNIX]   libxcb or libx11 (contains libxcb as dep)" OFF)

option(amGHOST_BUILD_CPP_ONLY         "Currently only affects     amGHOST_EKProcPtr_Capi   in amGHOST_Types" ON)
option(amGHOST_BUILD_TSuccess         "some Func returns amGHOST_TSuccess instead of BOOL.... Something that GHOST had" ON)

#One of these Should be ON, or we cause Error   [You can have Both ON TOO]
option(amGHOST_VULKAN_WindowSurface   "Window Surface is Platform Specific.... and amVK has no Control over that" ON)
option(amGHOST_BUILD_OPENGL           "OpenGL Support [amGHSOT_ContextWGL/GLX/EGL.cpp will be Compiled and Linked] glew.h will be included" ON)

# For now this doesn't work... we gotta remove this along with improving logger....
option(amGHOST_DEV                    "Enables some logging stuffs, only DEFINES amVK_LOG_DEV macro.... thats all." OFF)

option(amGHOST_V                      "add def amGHOST_V_DISABLED if off" ON)





#================================================================
# loading amGHOST_<Module>.cmake

  # It will work as a fallback, like you might not have copied files from amGHOST/build_files.... tho for duplicates, this will be searched last [THEORATICALLY]
    list(APPEND   CMAKE_MODULE_PATH   ${PROJECT_SOURCE_DIR}/build_files)
    message(NOTICE "         include: amGHOST_Options & amGHOST_Variables")

  include(${PROJECT_SOURCE_DIR}/build_files/macros.cmake    OPTIONAL RESULT_VARIABLE BLENDER_MACROS_M)            # Macros From BLENDER
  include(${PROJECT_SOURCE_DIR}/build_files/wrappers.cmake  OPTIONAL RESULT_VARIABLE amGHOST_WRAPPERS_M)          # WRAPPER_*** & ASSERT_listAppend
  include(amGHOST_Options                                   OPTIONAL RESULT_VARIABLE amGHOST_OPTIONS_M)           # same as options under documentation section
  #-----------------------------------------------------------------------------
  # INCLUDE VARIABLES [& LIB VARS Partially] - all VAR_<whatever> is here
  include(amGHOST_Variables                                 OPTIONAL RESULT_VARIABLE amGHOST_VARIABLES_M)         # Depends on OPTIONS Above

  #-----------------------------------------------------------------------------
  # Check if all the Modules required were Found!
  set(MODULES
    "${BLENDER_MACROS_M}"
    "${amGHOST_WRAPPERS_M}"
    "${amGHOST_OPTIONS_M}"
    "${amGHOST_VARIABLES_M}"
  )
  set(MODULE_NAMES
    "macros.cmake"
    "wrappers.cmake"
    "amGHOST_Options.cmake"
    "amGHOST_Variables.cmake"
  )
  set(_all_modules_found TRUE)
  message(NOTICE "") #Just a Newline

  message(NOTICE "amGHOST CMake modules:- ")
  list(APPEND CMAKE_MESSAGE_INDENT "-- ")
  foreach (_M ${MODULES})
    list(POP_FRONT MODULE_NAMES _M_NAME)
    if (_M)
      message(NOTICE "Using: " ${_M})
    else()
      set(_all_modules_found FALSE)
      message(NOTICE "[NOTFOUND]: " ${_M_NAME})
    endif()
  endforeach()
  set(CMAKE_MESSAGE_INDENT "")
  message(NOTICE "")

  unset(MODULE_NAMES)
  if (NOT _all_modules_found)
    message(STATUS "Did you move any files from 'build_files'?   [Listed above]")
    message(STATUS "If you did, remember to call: ")
    message(STATUS "   `list(APPEND   CMAKE_MODULE_PATH   path/to/build_files)`")
    message(NOTICE "")
    message(FATAL_ERROR "amGHOST Can't be built....")
  endif()

# [END-BLOCK] loading amGHOST_<Module>.cmake
#==================================================================
















































#-----------------------------------------------------------------------------
# EXTERNAL STUFF....
if(amGHOST_BUILD_OPENGL)
  add_subdirectory(${VAR_GLEW_DIR}   ${CMAKE_CURRENT_BINARY_DIR}/glew)
endif()
# this glew has to have a `CMakeLists.txt` Too!






#----------------------------------
# Variables used for amGHOST Target
#----------------------------------
set(SRC
  intern/amGHOST_System.cpp
  intern/amGHOST_V.cpp

  # 😄
  include/amGHOST_Decode.hh
)

set(INC
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/intern/
  ${PROJECT_SOURCE_DIR}/include/
  ${PROJECT_SOURCE_DIR}/extern/wuffs/
)

set(INC_SYS
) #This will ttrigger    SYSTEM option of target_include_directories()    Which turns off WARNINGs from Headers files of this Directory

set(LIB
)

# Prefix 'amGHOST_BUILD' will be added to all these in the end.... (it doesn't even matter - ches)
set(DEF
)




if(EXISTS ${VAR_amVK_LOGGER_N_UTIL}/amVK_Logger.hh)
  ASSERT_listAppend(   INC  VAR_amVK_LOGGER_N_UTIL   )
  if (${VAR_amVK_LOGGER_N_UTIL_IMPL})    #amVK_Variables.cmake
    list(APPEND DEF amVK_LOGGER_N_UTIL_IMPL)
    if (amGHOST_BUILD_WIN32)
      list(APPEND LIB dbghelp shlwapi kernel32 Version)
    endif()
  endif()
else()
  message(FATAL_ERROR "amVK_Logger.hh wasn;t found....   do  `git submodule update`")
endif()
 



#-----------------------------------------------------------------------------
# OS SPECIFIC CONFIG   [WIN32 for now]
if(amGHOST_BUILD_WIN32)
  list(APPEND   DEF   amGHOST_BUILD_WIN32)
  list(APPEND   SRC
    intern/amGHOST_WindowWIN32.cpp
    intern/amGHOST_SystemWIN32.cpp
  )
elseif(amGHOST_BUILD_X11)
  list(APPEND   DEF   amGHOST_BUILD_X11)
  list(APPEND   DEF   amGHOST_LINUX_X11)
  
  list(APPEND   INC
    ${X11_INCLUDE_DIR}
  )
  list(APPEND   SRC 
    intern/amGHOST_SystemX11.cpp
  )

  list(APPEND   LIB
    X11
  )
elseif(amGHOST_BUILD_XCB)
  list(APPEND   DEF   amGHOST_BUILD_XCB)
  list(APPEND   DEF   amGHOST_LINUX_XCB)
  list(APPEND   SRC 
    intern/amGHOST_SystemXCB.cpp
  )

  # MAKE USE OF https://github.com/KDE/extra-cmake-modules/blob/master/find-modules/FindXCB.cmake
  list(APPEND   LIB   /lib/libxcb.so /lib/libxcb-errors.so /lib/libxcb-ewmh.so /lib/libxcb-icccm.so)
endif()


#------------------
# OPENGL CONFIG
#------------------
if(amGHOST_BUILD_OPENGL)
  list(APPEND   DEF   amGHOST_BUILD_OPENGL)

  if (amGHOST_BUILD_WIN32)
    list(APPEND SRC
      intern/amGHOST_ContextWGL.cpp
    )
  endif()


  WRAPPER_FindOpenGL()
  list(APPEND   LIB   ${OPENGL_LIBRARIES})

  #WhereEver you include glew.h you need to do this or GLEW_BUILD
  list(APPEND   DEF   amGHOST_BUILD_${amGHOST_GLEW_STATIC})

  ASSERT_listAppend(  INC   VAR_GLEW_INC  )
  ASSERT_GLEW      (        VAR_GLEW_INC  )
  ASSERT_libAppend (        VAR_GLEW_LIB  )
endif()


#-----------------
# VULKAN CONFIG
#-----------------
if(amGHOST_VULKAN_WindowSurface)
  list(APPEND   DEF   amGHOST_BUILD_VULKAN)
  
  ASSERT_listAppend(   INC   VAR_VULKAN_INC  )
  ASSERT_Vulkan    (         VAR_VULKAN_INC  )
  ASSERT_libAppend (         VAR_VULKAN_LIB  )
endif()





#--------------------------------------
# COMMONN BUILD OPTIONS CONFIG
#--------------------------------------
if(amGHOST_BUILD_CPP_ONLY)
  list(APPEND   DEF   amGHOST_BUILD_CPP_ONLY)
else()
  list(APPEND   SRC   intern/amGHOST_Capi.cpp)
endif()

if(amGHOST_BUILD_TSuccess)
  list(APPEND   DEF   amGHOST_BUILD_TSuccess)
endif()

if(amGHOST_DEV)
  list(APPEND   DEF   amVK_DEV_LOG)
endif()

if(NOT amGHOST_V)
  list(APPEND   DEF   amGHOST_V_DISABLED)
endif()


#-----------------------------------------------------------------------------
# Finally Where we Make the amGHOST Library
#-----------------------------------------------------------------------------
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "" FORCE)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "" FORCE)

#blender_include_dirs(lib_amGHOST "${INC}" PUBLIC)
#blender_add_lib_nolist(lib-amGHOST   "${SRC}"  "${INC}"  "${INC_SYS}"  "${LIB}")


add_library(lib-amGHOST ${SRC})
target_include_directories(lib-amGHOST  PUBLIC  ${INC})
target_include_directories(lib-amGHOST  SYSTEM  PUBLIC "${INC_SYS}")
target_compile_definitions(lib-amGHOST  PUBLIC  ${DEF})
target_link_libraries(     lib-amGHOST  PRIVATE ${LIB})

set_target_properties(     lib-amGHOST  PROPERTIES  OUTPUT_NAME "lib-amGHOST"  PREFIX "")


# install ------------
install(TARGETS lib-amGHOST
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})



#-----------------------------------------------------------------------------
# UNSET OPTIONS, otherwise These will stay cached.... ReConfiguration of CMAKE won't change that CACHE
#-----------------------------------------------------------------------------
unset(amGHOST_BUILD_STATIC_EXTERN CACHE)
unset(amGHOST_BUILD_WIN32 CACHE)
unset(amGHOST_BUILD_X11 CACHE)
unset(amGHOST_BUILD_XCB)

unset(amGHOST_BUILD_CPP_ONLY CACHE)
unset(amGHOST_BUILD_TSuccess CACHE)

unset(amGHOST_VULKAN_WindowSurface CACHE)
unset(amGHOST_BUILD_OPENGL CACHE)

unset(amGHOST_DEV CACHE)
unset(amGHOST_V CACHE)