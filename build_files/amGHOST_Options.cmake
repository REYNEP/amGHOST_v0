# If you wanna set any of the OPTIONS.... from your Own CMAKE
#   - Remove FORCE for that option, here in this file
#   - SET(amGHOST_V ON CACHE BOOL "DOC" FORCE)                      [before 'add_subdirectory(amGHOST)']

set(amGHOST_BUILD_STATIC_EXTERN     ON  CACHE BOOL "" FORCE)
set(amGHOST_BUILD_WIN32             ON  CACHE BOOL "" FORCE)
set(amGHOST_BUILD_X11               OFF CACHE BOOL "" FORCE)
set(amGHOST_BUILD_XCB               OFF CACHE BOOL "" FORCE)

set(amGHOST_BUILD_CPP_ONLY          ON  CACHE BOOL "" FORCE)
set(amGHOST_BUILD_TSuccess          ON  CACHE BOOL "" FORCE)

#One of these Should be ON, or we cause Error   [You can have Both ON TOO]
set(amGHOST_VULKAN_WindowSurface    ON  CACHE BOOL "" FORCE)
set(amGHOST_BUILD_OPENGL            OFF CACHE BOOL "" FORCE)    #WIP

set(amGHOST_DEV                     OFF CACHE BOOL "" FORCE)
set(amGHOST_V                       ON  CACHE BOOL "" FORCE)
