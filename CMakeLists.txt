cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Paths with SPACES Aren't Supported
project(amGHOST_V0.1beta)  #Cmake Docs on this command says,  SubDirectory should have usually have its own project() too

set(CMAKE_CXX_STANDARD 17)    #amVK_IN's Member variables [intern/amVK_IN.hh]
set(CMAKE_CXX_STANDARD_REQUIRED ON)



#[[=======================================================================================================
      ╺┳┓┏━┓┏━╸╻ ╻┏┳┓┏━╸┏┓╻╺┳╸┏━┓╺┳╸╻┏━┓┏┓╻
       ┃┃┃ ┃┃  ┃ ┃┃┃┃┣╸ ┃┗┫ ┃ ┣━┫ ┃ ┃┃ ┃┃┗┫
      ╺┻┛┗━┛┗━╸┗━┛╹ ╹┗━╸╹ ╹ ╹ ╹ ╹ ╹ ╹┗━┛╹ ╹

#     if U Want to do add_subdirectory(amGHOST)
  ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
  - See build_files/amGHOST_Options.cmake
  - See build_files/amGHOST_Variables.cmake
  - See extern/CMakeLists.txt
  - DO  list(APPEND CMAKE_MODULE_PATH ...)      [if you wanna     move/merge     ./build_files with your own dir]
 
#    The Flow of this File
  ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
  - project & cmake_minimim on top, maybe some Compile related settings in future
  - Documentation BLOCK
  - Options declarations - are unset at EOF [end-of-file]
  - include all the modules, amGHOST_<Module>.cmake
  - Options will be overwritten by the values in amGHOST_Options.cmake
  - VAR_<whatever> will be loaded from amGHOST_Variables.cmake
  - Check if all the include(amGHOST_<Module>) worked ok.... and report
  - add_subdirectory() on all the extern libraries.... cz we dont have a any CMakeLists.txt inside amGHOST/extern folder
  - Finally the actual COOL Stuffs.... iam pretty sure you will understand the rest


  
#     FAQ
  ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
█ HOW DOES IT WORK in a NUTSHELL?
  ▀ see SRC, INC vars & the last section where we set target stuffs
  ▀   &  check out  blender_add_lib_nolist()     [This is LITERALLY stuff from BLENDER No Modification]
  ▀ OPTIONS can modify these  
  ▀ Extern Rel. VALUES are set in amGHOST_Variables.cmake    [we wanted to expose it.... ]

█ add_subdirectory(amGHOST)  will work Out of the BOX, that was the idea

█ You might wanna Modify amGHOST.... 
  ▀ So I tried to keep this file EASY & UNDERSTANDABLE   [even tho I kinda feel the Opposite still]

█ What if U Haven't read this or any DOCS & Didn't set the VARS?
  ▀ We got ASSERT_listAppend()

█ What if Ur VARIABLES are not VALID
  ▀ we got ASSERT_Glew, ASSERT_Vulkan


█ What if U wanna move/merge extern
  ▀ well its pretty easy.... just copy paste stuffs inside extern to wheever you like & delete the folder extern
  ▀   change path/to/extern [the variable VARIABLE_EXTERN_DIR] which is inside build_files/amGHOST_Variables.cmake

█ What if U wanna move/merge build_files
  ▀ You simply can just take away Only 1 or multiple or all .cmake files from there and paste it to wheever you wanna
  ▀   why move? cz, you may wanna move extern.... thats when you may need to modify amGHOST_Variables....
  ▀   just remember to do `list(APPEND   CMAKE_MODULE_PATH   path/to/build_files)`

█ What if you don't wanna LINK amGHOST EXTERN DEPs Locally to   amGHOST.lib output?                          
  ▀ see option amGHOST_BUILD_STATIC_EXTERN
  ▀ default ON.... bcz most prolly amGHOST is gonna be your program's EXTERN, and idont think any extern library should dependencies being SHARED Library

█ This File can't just CONTAIN Everything.... 
  ▀ Regular Stuffs like    macros.cmake   amGHOST_Wrapper.cmake   is in build_files directory
  ▀ You can MOVE/MERGE build_files dir \w yours          [if you got one....  just remember to  list(APPEND   CMAKE_MODULE_PATH   ... )  ]

█ What if you wanna Modify amGHOST OPTIONS based on OPTIONS from your Program/Lib/Api/App ?
  ▀ You can do so by superSeeding amGHOST_Options by your Options]
  ▀     amGHOST Options will be Unset at the End of Configuration.... 
  
#=======================================================================================================]]

option(amGHOST_BUILD_STATIC_EXTERN  "[TBA...WIP] Statically link Extern libraries, such as VULKAN, GLEW, etc" ON)
option(amGHOST_BUILD_WIN32  "[I don't think we need to do this] For Windows" ON)

option(amGHOST_BUILD_CPP_ONLY  "Currently only affects     amGHOST_EKProcPtr_Capi   in amGHOST_Types" ON)
option(amGHOST_BUILD_TSuccess  "some Func returns amGHOST_TSuccess instead of BOOL.... Something that GHOST had" ON)

#One of these Should be ON, or we cause Error   [You can have Both ON TOO]
option(amGHOST_VULKAN_WindowSurface  "Window Surface is Platform Specific.... and amVK has no Control over that" ON)
option(amGHOST_BUILD_OPENGL  "OpenGL Support [amGHSOT_ContextWGL/GLX/EGL.cpp will be Compiled and Linked] glew.h will be included" ON)

# For now this doesn't work... we gotta remove this along with improving logger....
option(amGHOST_DEV "Enables some logging stuffs" OFF)

option(amGHOST_V "add def amGHOST_V_DISABLED if off" ON)




#================================================================
# loading amGHOST_<Module>.cmake

  # It will work as a fallback, like you might not have copied all files from amGHOST/build_files.... then even if theres duplicate this will be searched last [THEORATICALLY]
    list(APPEND   CMAKE_MODULE_PATH   ${PROJECT_SOURCE_DIR}/build_files)

  include(macros            OPTIONAL RESULT_VARIABLE BLENDER_MACROS_M)            # Macros From BLENDER
  include(amGHOST_Wrappers  OPTIONAL RESULT_VARIABLE amGHOST_WRAPPERS_M)          # WRAPPER_*** & ASSERT_listAppend
  include(amGHOST_Options   OPTIONAL RESULT_VARIABLE amGHOST_OPTIONS_M)           # same as options under documentation section
  #-----------------------------------------------------------------------------
  # INCLUDE VARIABLES [& LIB VARS Partially] - all VAR_<whatever> is here
  include(amGHOST_Variables OPTIONAL RESULT_VARIABLE amGHOST_VARIABLES_M)         # Depends on OPTIONS Above

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
    "amGHOST_Wrappers.cmake"
    "amGHOST_Options.cmake"
    "amGHOST_Variables.cmake"
  )
  set(_all_modules_found TRUE)
  message(STATUS "") #Just a Newline

  message(NOTICE "amGHOST CMake modules:- ") #Just a Newline
  foreach (_M ${MODULES})
    list(POP_FRONT MODULE_NAMES _M_NAME)
    if (_M)
      message(STATUS "Using: " ${_M})
    else()
      set(_all_modules_found FALSE)
      message(STATUS "[NOTFOUND]: " ${_M_NAME})
    endif()
  endforeach()
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
# all the add_subdirectory()....
if(amGHOST_BUILD_OPENGL)
  add_subdirectory(${VAR_GLEW_DIR}   ${CMAKE_CURRENT_BINARY_DIR}/glew)
endif()
# this glew has to have a `CMakeLists.txt` Too!






#----------------------------------
# Variables used for amGHOST Target
#----------------------------------
set(SRC
  intern/amGHOST_System.cpp
)

set(INC
  ./
  intern/
)

set(INC_SYS
) #This will ttrigger    SYSTEM option of target_include_directories()    Which turns of WARNINGs from Headers files of this Directory

set(LIB
)

# Prefix 'amGHOST_BUILD' will be added to all these in the end.... (it doesn't even matter - ches)
set(DEF
)


if(EXISTS ${VAR_amVK_LOGGER_N_UTIL}/amVK_Logger.hh)
  ASSERT_listAppend(   INC  VAR_amVK_LOGGER_N_UTIL   )
else()
  message(FATAL_ERROR "amVK_Logger.hh wasn;t found....   do  `git submodule update`")
endif()



#-----------------------------------------------------------------------------
# COMMONN BUILD OPTIONS CONFIG
if(amGHOST_BUILD_CPP_ONLY)
  list(APPEND   DEF   CPP_ONLY)
else()
  list(APPEND   SRC   intern/amGHOST_Capi.cpp)
endif()
if(amGHOST_BUILD_TSuccess)
  list(APPEND   DEF   TSuccess)
endif()


#-----------------------------------------------------------------------------
# OS SPECIFIC CONFIG   [WIN32 for now]
if(amGHOST_BUILD_WIN32)
  list(APPEND   DEF   WIN32)
  list(APPEND   SRC
    intern/amGHOST_WindowWIN32.cpp
    intern/amGHOST_SystemWIN32.cpp
  )
endif()


#------------------
# OPENGL CONFIG
#------------------
if(amGHOST_BUILD_OPENGL)
  list(APPEND   DEF   OPENGL)

  if (amGHOST_BUILD_WIN32)
    list(APPEND SRC
      intern/amGHOST_ContextWGL.cpp
    )
  endif()


  WRAPPER_FindOpenGL()
  list(APPEND   LIB   ${OPENGL_LIBRARIES})

  #WhereEver you include glew.h you need to do this or GLEW_BUILD
  list(APPEND   DEF   ${amGHOST_GLEW_STATIC})

  ASSERT_listAppend(  INC   VAR_GLEW_INC  )
  ASSERT_GLEW      (        VAR_GLEW_INC  )
  ASSERT_libAppend (        VAR_GLEW_LIB  )
endif()


#-----------------
# VULKAN CONFIG
#-----------------
if(amGHOST_VULKAN_WindowSurface)
  list(APPEND   DEF   VULKAN)
  
  ASSERT_listAppend(   INC   VAR_VULKAN_INC  )
  ASSERT_Vulkan    (         VAR_VULKAN_INC  )
  ASSERT_libAppend (         VAR_VULKAN_LIB  )
endif()






#-----------------------------------------------------------------------------
# Finally Where we Make the amGHOST Library
#-----------------------------------------------------------------------------
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "" FORCE)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "" FORCE)

blender_add_lib_nolist(lib-amGHOST   "${SRC}"  "${INC}"  "${INC_SYS}"  "${LIB}")

set(FINAL_MACROS 
)
foreach(macro ${DEF})
  list(APPEND FINAL_MACROS    amGHOST_BUILD_${macro})
endforeach()
if(amGHOST_DEV)
  list(APPEND   FINAL_MACROS   amVK_DEBUG_LOG)
endif()
if(NOT amGHOST_V)
  list(APPEND   FINAL_MACROS   amGHOST_V_DISABLED)
endif()
target_compile_definitions(lib-amGHOST  PUBLIC  "${FINAL_MACROS}")

install(TARGETS lib-amGHOST
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})



#-----------------------------------------------------------------------------
# UNSET OPTIONS, otherwise These will stay cached.... ReConfiguration of CMAKE won't change that CACHE
#-----------------------------------------------------------------------------
unset(amGHOST_BUILD_CPP_ONLY CACHE)
unset(amGHOST_BUILD_TSuccess CACHE)
unset(amGHOST_BUILD_WIN32 CACHE)
unset(amGHOST_VULKAN_WindowSurface CACHE)
unset(amGHOST_BUILD_OPENGL CACHE)
unset(amGHOST_DEV CACHE)
unset(amGHOST_V CACHE)