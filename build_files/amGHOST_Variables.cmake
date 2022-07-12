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
# The Original Code is Copyright (C) 2022, REYNEP
#
# ***** END GPL LICENSE BLOCK *****
#
#    All the amGHOST's external libs related Variables are here
#    Now we made this separte, bcz didn't want everything in 'CMakeLists.txt'
#    + Anyone can modify the settings easily now... instead of having to change ENVIRONMENT VARIABLE everytime now....
#
#    
#    You can make UR own copies of amGHOST_Options.cmake & amGHOST_Variables.cmake
#
#      - do `list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/buildFiles)` 
#      - before `add_subdirectory(amGHOST)`
#
#  UNMODIFIED File
if (NOT ${CMAKE_SOURCE_DIR} EQUAL ${PROJECT_SOURCE_DIR})
  message(STATUS "using UnModified amGHOST_Variables.cmake in [" ${PROJECT_SOURCE_DIR} "/build_files]")
  message(STATUS "    CMAKE_CURRENT_LIST_DIR: " ${CMAKE_CURRENT_LIST_DIR})
  message(STATUS "    ....you should make your own copy, since its most likely you already do have external stuffs like vulkan, glew")
  message(STATUS "    ....check ReadMe::Build\n")
endif()


#-----------------------------------------------------------------------------
# Modify These....          ${PROJECT_SOURCE_DIR}/extern is downloaded by make.py
set(VAR_VULKAN_DIR          ${PROJECT_SOURCE_DIR}/extern/vulkan-sdk-lunarg)    #Needed if   amGHOST_VULKAN_WindowSurface [option] is ON
set(VAR_GLEW_DIR            ${PROJECT_SOURCE_DIR}/extern/glew)                 #Needed if   amGHOST_BUILD_OPENGL         [option] is ON   later add_subdirectory(VAR_GLEW_DIR)
set(VAR_amVK_LOGGER_N_UTIL  ${PROJECT_SOURCE_DIR}/amVK_LoggerNUtils)           #You will need to download using
set(VAR_amVK_LOGGER_N_UTIL_IMPL TRUE)                                          #COMPILE amVK_Logger IMPLEMENTATIONS


# YOU GOTTA DEFINE
#
# amGHOST_BUILD_OPENGL [W.I.P.]
#    VAR_amGHOST_GLEW_STATIC      : GLEW_STATIC/GLEW_SHARED
#    VAR_amGHOST_GLEW_TARGET_NAME : extern_glew
#    VAR_GLEW_LIB                 : extern_glew [wip \todo]
#    VAR-GLEW_INC                 : GL/glew.h       [https://github.com/nigels-com/glew]
#
# amGHOST_VULKAN_WindowSurface
#    VAR_VULKAN_INC: [has vulkan/vulkan.h]
#    VAR_VULKAN_LIB: [full path vulkan-1.lib]    [provided by vulkan-sdk-lunarg (Khronos)]
#    VAR_VULKAN_DIR: add_subdirectory
#


include("${PROJECT_SOURCE_DIR}/build_files/macros.cmake")




if (NOT EXISTS VAR_amVK_LOGGER_N_UTIL/amVK_Logger.hh)
  message(SEND_ERROR 
    "____________\n"
    "amVK_LoggerNUtils GIT SubModule is empty.... "
    "do `git submodule init` & `git submodule update`"
    "\n¯¯¯¯¯¯¯¯¯¯¯¯"
  )
endif()



#-----------------------------------------------------------------------------
# amGHOST_GLEW   [amGHOST_Window::vulkan_create_surface()]
if (amGHOST_BUILD_OPENGL)
  if (NOT EXISTS ${VAR_GLEW_DIR})
    message(FATAL_ERROR 
      "____________\n"
      "amGHOST_Variables.cmake::VAR_GLEW_DIR is not set right....   but "
      "amGHOST_BUILD_OPENGL [option] is ON.... Can't Build \n"
      "xD, try running amGHOST/make.py to download required extern stuff"
      "\n¯¯¯¯¯¯¯¯¯¯¯¯"
    )
  endif()

  set(VAR_amGHOST_GLEW_STATIC       GLEW_STATIC)
  set(VAR_amGHOST_GLEW_TARGET_NAME  extern_glew)
  set(VAR_GLEW_LIB                  extern_glew)    #Name that we have in our `extern/glew/CMakeLists.txt::add_library()`
  set(VAR_GLEW_INC           ${VAR_GLEW_DIR}/include)
endif()



#-----------------------------------------------------------------------------
# amGHOST_VULKAN   [amGHOST_Window::vulkan_create_surface()]
if(WIN32)


  if(amGHOST_VULKAN_WindowSurface)
    if (NOT EXISTS ${VAR_VULKAN_DIR})
      message(FATAL_ERROR 
        "____________\n"
        "amGHOST_Variables.cmake::VAR_VULKAN_DIR is not set right.... but "
        "amGHOST_VULKAN_WindowSurface [option] is ON.... Can't Build \n"
        "xD, try running amGHOST/make.py to download required extern stuff"
        "\n¯¯¯¯¯¯¯¯¯¯¯¯"
      )
    endif()

    set(VAR_VULKAN_INC   
      ${VAR_VULKAN_DIR}/include   # we use "vulkan/vulkan.h"
    )

    amGHOST_find_library(VAR_VULKAN_LIB 
      NAMES 
        vulkan
        vulkan-1    #On windows as of Vulkan 1.2
      PATHS 
        ${VAR_VULKAN_DIR}
      PATH_SUFFIXES
        lib lib64
      NO_DEFAULT_PATH
      REQUIRED      # Doesn't require 3.18 CMake
    )
  endif()


else()


  find_package(Vulkan REQUIRED)
  find_package(X11 REQUIRED)

  set(VAR_VULKAN_LIB Vulkan::Vulkan)
  

endif()
