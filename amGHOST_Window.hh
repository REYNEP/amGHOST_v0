#pragma once
#ifndef amGHOST_WINDOW
#define amGHOST_WINDOW

#ifndef amGHOST_TYPES
  #include "amGHOST_Types.h"
#endif
#ifndef amGHOST_CONTEXT
  #include "amGHOST_Context.hh"
#endif

/** define this macro yourself or for CMAKE/MESON, \see readme.md */
#if defined(amGHOST_BUILD_VULKAN)
  #include "vulkan/vulkan.h"
#endif

class amGHOST_Window
{
 public:
  amGHOST_Context *m_render_context = nullptr;  /** [WIP] */
  const char *m_title = "NO_TITLE";
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

 protected:
  amGHOST_Window(const amGHOST_Window&) = delete;             //Brendan's Solution
  amGHOST_Window& operator=(const amGHOST_Window&) = delete;  //Brendan's Solution
 protected:
  amGHOST_Window() {}                                         //because of the line below.... there will be no default CONSTRUCTOR
  virtual ~amGHOST_Window() = default;                        //-Wnon-virtual-dtor
};

#endif //#ifndef amGHOST_WINDOW