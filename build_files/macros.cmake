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
# The Original Code is Copyright (C) 2006, Blender Foundation
# All rights reserved.
# ***** END GPL LICENSE BLOCK *****

# From: from: [https://github.com/blender/blender/blob/594f47ecd2d5367ca936cf6fc6ec8168c2b360d0/build_files/cmake/macros.cmake]




# param 'REQUIRED': Doesn't depend on CMake 3.18....
function(amGHOST_find_library
  OutVar
)

  cmake_parse_arguments(
    PARSE_ARGV 1                        # escapes 1st argument
    PARAM                               # pREFIX
    
    "NO_DEFAULT_PATH;REQUIRED"          # Options, a.k.a BOOL
    ""
    #"ERROR_MSG;STATUS_MSG;FIXING_MSG"  # 1-Value
    "NAMES;PATHS;PATH_SUFFIXES"         # Multiple Values....
  )

  set(_NO_DEFAULT_PATH_)
  set(_REQUIRED_)

  if (${PARAM_NO_DEFAULT_PATH})
    set(_NO_DEFAULT_PATH_ "NO_DEFAULT_PATH")
  endif()

  find_library(_temp_
    NAMES
      ${PARAM_NAMES}
    PATHS
      ${PARAM_PATHS}
    PATH_SUFFIXES
      ${PARAM_PATH_SUFFIXES}
    ${_NO_DEFAULT_PATH_}
  )

  if (NOT EXISTS ${_temp_})
    message(NOTICE)
    message(STATUS "[amGHOST] Can't find '${PARAM_NAMES}' in ${PARAM_PATHS}\n")

    if (${PARAM_REQUIRED})
      unset(_temp_ CACHE)
      message(FATAL_ERROR)
    endif()
  endif()

  set(${OutVar} ${_temp_} PARENT_SCOPE)
  unset(_temp_ CACHE)

endfunction()





macro(add_c_flag
  flag)

  string(APPEND CMAKE_C_FLAGS " ${flag}")
  string(APPEND CMAKE_CXX_FLAGS " ${flag}")
endmacro()

function(list_assert_duplicates
  list_id
  )

  # message(STATUS "list data: ${list_id}")

  list(LENGTH list_id _len_before)
  list(REMOVE_DUPLICATES list_id)
  list(LENGTH list_id _len_after)
  # message(STATUS "list size ${_len_before} -> ${_len_after}")
  if(NOT _len_before EQUAL _len_after)
    message(FATAL_ERROR "duplicate found in list which should not contain duplicates: ${list_id}")
  endif()
  unset(_len_before)
  unset(_len_after)
endfunction()













# MODIFIED by REYNEP
# Nicer makefiles with -I/1/foo/ instead of -I/1/2/3/../../foo/
# use it instead of include_directories()
function(blender_include_dirs
  name
  includes
  _P
)

  set(_ALL_INCS "")
  foreach(_INC ${ARGV})
    get_filename_component(_ABS_INC ${_INC} ABSOLUTE)
    list(APPEND _ALL_INCS ${_ABS_INC})
    # for checking for invalid includes, disable for regular use
    # if(NOT EXISTS "${_ABS_INC}/")
    #   message(FATAL_ERROR "Include not found: ${_ABS_INC}/")
    # endif()
  endforeach()
  target_include_directories(${name} ${_ALL_INCS} ${_P})

endfunction()

# Using this one would not Show Warnings from files that are inside those SYSTEM_INCLUDE_DIRECTORIES
function(blender_include_dirs_sys
  name
  includes
  _P
)

  set(_ALL_INCS "")
  foreach(_INC ${ARGV})
    get_filename_component(_ABS_INC ${_INC} ABSOLUTE)
    list(APPEND _ALL_INCS ${_ABS_INC})
    # for checking for invalid includes, disable for regular use
    # if(NOT EXISTS "${_ABS_INC}/")
    #   message(FATAL_ERROR "Include not found: ${_ABS_INC}/")
    # endif()
  endforeach()
  target_include_directories(${name} SYSTEM ${_ALL_INCS} ${_P})

endfunction()















# Set include paths for header files included with "*.h" syntax.
# This enables auto-complete suggestions for user header files on Xcode.
# Build process is not affected since the include paths are the same
# as in HEADER_SEARCH_PATHS.
function(blender_user_header_search_paths
  name
  includes
  )

  if(XCODE)
    set(_ALL_INCS "")
    foreach(_INC ${includes})
      get_filename_component(_ABS_INC ${_INC} ABSOLUTE)
      # _ALL_INCS is a space-separated string of file paths in quotes.
      string(APPEND _ALL_INCS " \"${_ABS_INC}\"")
    endforeach()
    set_target_properties(${name} PROPERTIES XCODE_ATTRIBUTE_USER_HEADER_SEARCH_PATHS "${_ALL_INCS}")
  endif()
endfunction()


function(blender_source_group
  name
  sources
  )

  # if enabled, use the sources directories as filters.    P.S. what are filters?
  if(WINDOWS_USE_VISUAL_STUDIO_SOURCE_FOLDERS)
    foreach(_SRC ${sources})
      # remove ../'s
      get_filename_component(_SRC_DIR ${_SRC} REALPATH)
      get_filename_component(_SRC_DIR ${_SRC_DIR} DIRECTORY)
      string(FIND ${_SRC_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/" _pos)
      if(NOT _pos EQUAL -1)
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" GROUP_ID ${_SRC_DIR})
        string(REPLACE "/" "\\" GROUP_ID ${GROUP_ID})
        source_group("${GROUP_ID}" FILES ${_SRC})
      endif()
      unset(_pos)
    endforeach()
  else()
    # Group by location on disk
    source_group("Source Files" FILES CMakeLists.txt)
    foreach(_SRC ${sources})
      get_filename_component(_SRC_EXT ${_SRC} EXT)
      if((${_SRC_EXT} MATCHES ".h") OR
         (${_SRC_EXT} MATCHES ".hpp") OR
         (${_SRC_EXT} MATCHES ".hh"))

        set(GROUP_ID "Header Files")
      elseif(${_SRC_EXT} MATCHES ".glsl$")
        set(GROUP_ID "Shaders")
      else()
        set(GROUP_ID "Source Files")
      endif()
      source_group("${GROUP_ID}" FILES ${_SRC})
    endforeach()
  endif()

  # if enabled, set the FOLDER property for visual studio projects
  if(WINDOWS_USE_VISUAL_STUDIO_PROJECT_FOLDERS)
    get_filename_component(FolderDir ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
    string(REPLACE ${CMAKE_SOURCE_DIR} "" FolderDir ${FolderDir})
    set_target_properties(${name} PROPERTIES FOLDER ${FolderDir})
  endif()
endfunction()

# only MSVC uses SOURCE_GROUP
function(blender_add_lib__impl
  name
  sources
  includes
  includes_sys
  library_deps
  )

  # message(STATUS "Configuring library ${name}")

  # include_directories(${includes})
  # include_directories(SYSTEM ${includes_sys})
  blender_include_dirs_sys("${includes_sys}")

  add_library(${name} ${sources})

  # On Windows certain libraries have two sets of binaries: one for debug builds and one for
  # release builds. The root of this requirement goes into ABI, I believe, but that's outside
  # of a scope of this comment.
  #
  # CMake have a native way of dealing with this, which is specifying what build type the
  # libraries are provided for:
  #
  #   target_link_libraries(tagret optimized|debug|general <libraries>)
  #
  # The build type is to be provided as a separate argument to the function.
  #
  # CMake's variables for libraries will contain build type in such cases. For example:
  #
  #   set(FOO_LIBRARIES optimized libfoo.lib debug libfoo_d.lib)
  #
  # Complications starts with a single argument for library_deps: all the elements are being
  # put to a list: "${FOO_LIBRARIES}" will become "optimized;libfoo.lib;debug;libfoo_d.lib".
  # This makes it impossible to pass it as-is to target_link_libraries sine it will treat
  # this argument as a list of libraries to be linked against, causing missing libraries
  # for optimized.lib.
  #
  # What this code does it traverses library_deps and extracts information about whether
  # library is to provided as general, debug or optimized. This is a little state machine which
  # keeps track of which build type library is to provided for:
  #
  # - If "debug" or "optimized" word is found, the next element in the list is expected to be
  #   a library which will be passed to target_link_libraries() under corresponding build type.
  #
  # - If there is no "debug" or "optimized" used library is specified for all build types.
  #
  # NOTE: If separated libraries for debug and release are needed every library is the list are
  # to be prefixed explicitly.
  #
  #  Use: "optimized libfoo optimized libbar debug libfoo_d debug libbar_d"
  #  NOT: "optimized libfoo libbar debug libfoo_d libbar_d"
  #
  # NOTE-REYNEP: None of the BLENDER's intern or source Libraries/Modules uses optimized or debug   only External stuffs does.... [Do a Quick search with *.cmake]
  #
  if(NOT "${library_deps}" STREQUAL "")
    set(next_library_mode "")
    foreach(library ${library_deps})
      string(TOLOWER "${library}" library_lower)
      if(("${library_lower}" STREQUAL "optimized") OR
         ("${library_lower}" STREQUAL "debug"))
        set(next_library_mode "${library_lower}")
      else()
        if("${next_library_mode}" STREQUAL "optimized")
          target_link_libraries(${name} INTERFACE optimized ${library})
        elseif("${next_library_mode}" STREQUAL "debug")
          target_link_libraries(${name} INTERFACE debug ${library})
        else()
          target_link_libraries(${name} INTERFACE ${library})
        endif()
        set(next_library_mode "")
      endif()
    endforeach()
  endif()

  # works fine without having the includes
  # listed is helpful for IDE's (QtCreator/MSVC)            [REYNEP: Not something I can understand before I actually test MSVC IDE]
  blender_source_group("${name}" "${sources}")
  blender_user_header_search_paths("${name}" "${includes}")

  list_assert_duplicates("${sources}")
  list_assert_duplicates("${includes}")
  # Not for system includes because they can resolve to the same path
  # list_assert_duplicates("${includes_sys}")

endfunction()

















function(blender_add_lib_nolist
  name
  sources
  includes
  includes_sys
  library_deps
  )

# add_cc_flags_custom_test(${name} PARENT_SCOPE)    [REYNEP: Don't think we would need this]

  blender_add_lib__impl(${name} "${sources}" "${includes}" "${includes_sys}" "${library_deps}")
endfunction()

function(blender_add_lib
  name
  sources
  includes
  includes_sys
  library_deps
  )

# add_cc_flags_custom_test(${name} PARENT_SCOPE)    [REYNEP: Don't think we would need this]

  blender_add_lib__impl(${name} "${sources}" "${includes}" "${includes_sys}" "${library_deps}")

# set_property(GLOBAL APPEND PROPERTY BLENDER_LINK_LIBS ${name})
endfunction()
