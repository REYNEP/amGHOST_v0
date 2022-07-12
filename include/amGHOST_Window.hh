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

  amGHOST_Window(const char *title, int posX, int posY, int sizeX, int sizeY)
  : m_title(title),
    m_posX(posX),
    m_posY(posY),
    m_sizeX(sizeX),
    m_sizeY(sizeY)
  {}

  /** 
   * Bcz, I wanted it [deleting amGHOST_Window] to be on the hands of the amGHOST user... 
   * e.g. dispose_system must not ***delete*** objects 
   */
  virtual bool destroyer(void) = 0;

  /**
   * OPENGL
   */
  virtual amGHOST_Context* opengl_create_context(void)   = 0;
  virtual void             opengl_activate_context(void) = 0;

  /**
   * VULKAN
   */
  #if defined(amGHOST_BUILD_VULKAN)
    virtual VkSurfaceKHR create_vulkan_surface(VkInstance instance) = 0;
  #endif

  /**
   * These Literally Does what these say in the Name....
   * TAGS: Minimize Window, Hide Window, 
   */
  virtual void show_window(void) = 0;
  virtual void hide_window(void) = 0;

  /** TAGS: change title, window name, Top Menu Bar Text, Window Title, \todo impliment in all SYS and make ABSOLUTE VIRTUAL */
  virtual void set_title(const char *title) {};



 protected:
  amGHOST_Window(const amGHOST_Window&) = delete;             //Brendan's Solution
  amGHOST_Window& operator=(const amGHOST_Window&) = delete;  //Brendan's Solution
 protected:
  amGHOST_Window() {}                                         //because of the line below.... there will be no default CONSTRUCTOR
  virtual ~amGHOST_Window() = default;                        //-Wnon-virtual-dtor
};

#endif //#ifndef amGHOST_WINDOW