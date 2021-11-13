#if defined(amGHOST_BUILD_VULKAN) & defined(amGHOST_WindowWIN32_CPP)    //Won't need to Include this Anywhere Else   [vulkan/vulkan.h is in IWindow.h]
  #define VK_USE_PLATFORM_WIN32_KHR   //Vulkan Loader adds WIN32 specific Functions, thats all i care about now on this MACRO
#endif
#ifndef amGHOST_WINDOW
  #include "amGHOST_Window.hh"
#endif
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>  //Need: <sdkddkver.h>, <winresrc.h>, "WinDef.h" [HWND m_hwnd; HDC m_hdc; HGLRC m_hglrc]

#ifndef amGHOST_CONTEXT
  #include "amGHOST_Context.hh"
#endif

class amGHOST_WindowWIN32 : public amGHOST_Window
{
 public:
  HWND m_hwnd = NULL;
  /**
  * CONSTRUCTOR
  */
  amGHOST_WindowWIN32(char *title, int posX, int posY, int sizeX, int sizeY);

  bool destroyer(void);
  /**
  * DESTRUCTOR
  */
  ~amGHOST_WindowWIN32() {};


  /**
  * These Literally Does what these says in the Name....
  */
  void show_window(void);
  void hide_window(void);

  /**
  OPENGL SECTION [WGL]
  */
  amGHOST_Context* opengl_create_context(void);
  void activate_context(void);

  /**
    VULKAN SECTION
  */
#if defined(amGHOST_BUILD_VULKAN) || defined(amGHOST_VULKAN)
  VkSurfaceKHR create_vulkan_surface(VkInstance instance);
#endif
};