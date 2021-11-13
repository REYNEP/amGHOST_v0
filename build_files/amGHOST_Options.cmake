set(amGHOST_BUILD_STATIC_EXTERN     ON  CACHE BOOL "" FORCE)
set(amGHOST_BUILD_WIN32             ON  CACHE BOOL "" FORCE)


set(amGHOST_BUILD_CPP_ONLY          ON  CACHE BOOL "" FORCE)
set(amGHOST_BUILD_TSuccess          ON  CACHE BOOL "" FORCE)

#One of these Should be ON, or we cause Error   [You can have Both ON TOO]
set(amGHOST_VULKAN_WindowSurface    ON  CACHE BOOL "" FORCE)
set(amGHOST_BUILD_OPENGL            OFF CACHE BOOL "" FORCE)