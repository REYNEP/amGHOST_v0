# UNMODIFIED File
if (${CMAKE_SOURCE_DIR} EQUAL ${PROJECT_SOURCE_DIR})
  message(STATUS "Using UnModified amGHOST_Variables.cmake in [" ${PROJECT_SOURCE_DIR} "/build_files]")
endif()

#-----------------------------------------------------------------------------
# All the amGHOST's extern/library related Variables are here
# Now we made this separte, bcz didn't want everything in 'CMakeLists.txt'
#   And anyone who wants to move 'amGHOST/extern' can now just do that & make use of this file & change stuffs as they change/move/merge extern stuffs
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Modify These....
set(EXTERN            ${PROJECT_SOURCE_DIR}/extern)       #Downloaded by `make.py`
set(VAR_VULKAN_DIR        ${EXTERN}/vulkan-sdk-lunarg)    #Needed if   amGHOST_VULKAN_WindowSurface [option] is ON
set(VAR_GLEW_DIR          ${EXTERN}/glew)                 #Needed if   amGHOST_BUILD_OPENGL         [option] is ON
# later add_subdirectory(VAR_GLEW_DIR) in CMakeLists.txt.... so glew has to have a `CMakeLists.txt` Too!

if (amGHOST_BUILD_OPENGL)
  if (NOT EXISTS ${VAR_GLEW_DIR})
    message(FATAL_ERROR "amGHOST_Variables.cmake::VAR_GLEW_DIR is not set right....   but amGHOST_BUILD_OPENGL [option] is ON.... Can't Build")
  endif()

  set(VAR_amGHOST_GLEW_STATIC       GLEW_STATIC)
  set(VAR_amGHOST_GLEW_TARGET_NAME  extern_glew)
  set(VAR_GLEW_LIB                  extern_glew)    #Name that we have in our `extern/glew/CMakeLists.txt::add_library()`
  #set(VAR_GLEW_DIR)
  set(VAR_GLEW_INC           ${VAR_GLEW_DIR}/include)
endif()


#-----------------------------------------------------------------------------
# amGHOST_VULKAN   [amGHOST_Window::vulkan_create_surface()]
if(amGHOST_VULKAN_WindowSurface)
  if (NOT EXISTS ${VAR_VULKAN_DIR})
    message(FATAL_ERROR "amGHOST_Variables.cmake::VAR_VULKAN_DIR is not set right....   but amGHOST_VULKAN_WindowSurface [option] is ON.... Can't Build")
  endif()

  set(VAR_VULKAN_INC   
      # ${vkSDK}/include/vulkan  You should instead include "vulkan/vulkan.h"
      ${VAR_VULKAN_DIR}/include
  )

  find_library(vulkan_static 
        NAMES 
          vulkan
          vulkan-1    #On windows as of Vulkan 1.2
        PATHS 
          ${VAR_VULKAN_DIR}
        PATH_SUFFIXES
          lib lib64
        NO_DEFAULT_PATH
        REQUIRED
        # Will cz the Error if not FOUND
    )

    set(VAR_VULKAN_LIB            ${vulkan_static}         )
    unset(vulkan_static CACHE)
endif()