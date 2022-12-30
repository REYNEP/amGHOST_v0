#pragma once
#ifndef amGHOST_WINDOW
#define amGHOST_WINDOW

#ifndef amGHOST_TYPES
  #include "amGHOST_Types.h"
#endif
#ifndef amGHOST_CONTEXT
  #include "amGHOST_Context.hh"
#endif

/** for CMAKE/MESON, \see readme.md, or if you are using smth else, be sure to declare this yourself */
#if defined(amGHOST_BUILD_VULKAN)
  #if defined(_amGHOST_INC_VULKAN_)
    #include "vulkan/vulkan.h"
  #else
    typedef struct VkSurfaceKHR_T* VkSurfaceKHR;
    typedef struct VkInstance_T* VkInstance;
  #endif
#endif


/**
 *  TODO: Faster \fn get_window() Implementation.... [WIN32]
 *  TODO: Reconsider having set_wm_class() here
 *  TODO: Handle no amGHOST_WindowWIN32 events too....  You will see that a WM_SIZE is reported even before any Win32 windows are created....
 *        see https://github.com/blender/blender/blob/master/intern/ghost/intern/GHOST_SystemWin32.cpp#L1276  haha 😁
 *  TODO: Implement WM_SYSCOMMAND support... 
 *        https://sourceforge.net/p/win32loopl/code/ci/default/tree/LooplessSizeMove.c
 *        https://stackoverflow.com/a/21240384
 * 
 *  TODO: add WM_MOVE Support inside WM_WINDOWPOSCHANGED....
 */
class amGHOST_Window
{
 public:
  amGHOST_Context *m_render_context = nullptr;  /** [WIP] */
  const char               *m_title = "NO_TITLE";
  void             *window_userData = nullptr;
  uint32_t m_posX;
  uint32_t m_posY;
  uint32_t m_sizeX;
  uint32_t m_sizeY;

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
  virtual void set_title(const char *title) {}


  /**
   *   ╻ ╻   ┏━┓┏┳┓╻ ╻╻┏    ┏━╸╻ ╻┏━╸┏┓╻╺┳╸
   *   ╺╋╸   ┣━┫┃┃┃┃┏┛┣┻┓   ┣╸ ┃┏┛┣╸ ┃┗┫ ┃ 
   *   ╹ ╹   ╹ ╹╹ ╹┗┛ ╹ ╹╺━╸┗━╸┗┛ ┗━╸╹ ╹ ╹ 
   */
 public:
  void _MSG_kWindowResized(uint32_t width, uint32_t height) {m_sizeX = width; m_sizeY = height;}


 public:
  /**
   *   ╻ ╻   ┏━┓╻  ┏━┓╺┳╸┏━╸┏━┓┏━┓┏┳┓   ┏━┓┏━┓┏━╸┏━╸╻┏━╸╻┏━╸
   *   ╺╋╸   ┣━┛┃  ┣━┫ ┃ ┣╸ ┃ ┃┣┳┛┃┃┃   ┗━┓┣━┛┣╸ ┃  ┃┣╸ ┃┃  
   *   ╹ ╹   ╹  ┗━╸╹ ╹ ╹ ╹  ┗━┛╹┗╸╹ ╹   ┗━┛╹  ┗━╸┗━╸╹╹  ╹┗━╸
   */
  /** XCB: PLATFORM_SPECIFIC, you must check the docs there.... [ inside amGHOST_WindowXCB.hh ] */
  virtual void set_wm_class(const char *wm_class, bool i_know_the_docs = false) {}
  /** TODO: virtual void set_ewmh_window_type(uint32_t TYPE) {} */
  /** TODO:              XCB_ICCCM_WM_HINT_ICON_PIXMAP          */
  /** TODO:                _NET_WM_STATE_FULLSCREEN             */
  /** TODO:               xcb_icccm_set_wm_size_hints           */



 protected:
  amGHOST_Window(const amGHOST_Window&) = delete;             //Brendan's Solution
  amGHOST_Window& operator=(const amGHOST_Window&) = delete;  //Brendan's Solution
 protected:
  amGHOST_Window() {}                                         //because of the line below.... there will be no default CONSTRUCTOR
  virtual ~amGHOST_Window() = default;                        //-Wnon-virtual-dtor
};

#endif //#ifndef amGHOST_WINDOW