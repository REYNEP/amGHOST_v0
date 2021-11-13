//Include Guard
#ifndef amGHOST_WINDOW
#define amGHOST_WINDOW

#ifndef amGHOST_TYPES
  #include "amGHOST_Types.h"
#endif
#ifndef amGHOST_CONTEXT
  #include "amGHOST_Context.hh"
#endif

#if defined(amGHOST_BUILD_VULKAN) || defined(amGHOST_BUILD)  //Add this macro to Files that you wanna Use create_vulkan_surface
  #include "vulkan/vulkan.h"    //Vulkan is PlatformAgnostic, so no This could be added by default
#endif

#include <cstddef>  //GCC NULL Error

class amGHOST_Window
{
 public:
  // THESE SHOULD BE UPDATED as WINDOW Also Updates
  amGHOST_Context *m_render_context = NULL;
  const char *m_title;
  int m_posX;
  int m_posY;
  int m_sizeX;
  int m_sizeY;

  amGHOST_Window(char *title, int posX, int posY, int sizeX, int sizeY)
  : m_title(title),
    m_posX(posX),
    m_posY(posY),
    m_sizeX(sizeX),
    m_sizeY(sizeY)
  {}
  virtual bool destroyer(void) = 0;

  virtual void show_window(void) = 0;
  virtual void hide_window(void) = 0;
  virtual amGHOST_Context* opengl_create_context(void) = 0;
#if defined(amGHOST_BUILD_VULKAN) || defined(amGHOST_VULKAN)
  virtual VkSurfaceKHR create_vulkan_surface(VkInstance instance) = 0;
#endif
  virtual void activate_context(void) = 0;
};

#endif //#ifndef amGHOST_WINDOW