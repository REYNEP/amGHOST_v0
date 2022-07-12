#pragma once

#include "amGHOST_SystemX11.hh"

#ifndef amGHOST_WINDOW
  #include "amGHOST_Window.hh"
#endif


/**
 *      NOTE:
 * 1. [ ATTRIBS]: XS_Attribs  This varaible is inside amGHOST_SystemX11.... Universal for all X11 Windows now
 * 
 *      TODO:
 * 1. [FEATUREE]:   BORDER    XCreateSimpleWindow border
 * 2. [FEATUREE]: BACKGROUND  XCreateSimpleWindow background
 * 
 *      RESC:
 * 1. [XCreateSimpleWindow] https://github.com/mirror/libX11/blob/5faa8dc0b364f55f19034c370436074646d9ad54/src/CrWindow.c
 */
class amGHOST_WindowX11 : public amGHOST_Window {
 public:
  amGHOST_WindowX11(const char *title, int posX, int posY, int sizeX, int sizeY) : amGHOST_Window(title, posX, posY, sizeX, sizeY) {
    s_X11 = (amGHOST_SystemX11 *) amGHOST_System::heart;

    Screen *XS = s_X11->get_DefaultScreen();

    /**
    XW = XCreateWindow(
      s_X11->XD,
      XS->root,
      posX, posY,
      sizeX, sizeY,

      s_X11->XW_border_width,

      XS->root_depth,
      CopyFromParent,       /** MACRO: InputOutput or InputOnly *
      XS->root_visual,

      CWOverrideRedirect | CWBackPixel | CWBorderPixel | CWEventMask,
     &s_X11->XW_attribs
    );

    
    */

    XW = XCreateSimpleWindow(s_X11->XD, XS->root, posX, posY, sizeX, sizeY, 10, XS->black_pixel, XS->white_pixel);
    XFillRectangle(s_X11->XD, XW, XS->default_gc, 20, 20, 10, 10);
    XDrawString(s_X11->XD, XW, XS->default_gc, 10, 50, "HELLO", 5);
    if (!XW) {
      amVK_LOG_EX("[create_window FAILED]: " << "XCreateWindow()");
      return;
    }
  }
 ~amGHOST_WindowX11()   {destroyer();}
  bool destroyer(void)  {
    if (XW) { XDestroyWindow(s_X11->XD, XW);  return true; }
    else      return false;
  }


 public:
  Window XW = (XID) nullptr;    /** type XID, unlike [X11] Display */
  amGHOST_SystemX11 *s_X11;


 public:
  /**
   * These Literally Does what these say in the Name....
   * TAGS: Minimize Window, Hide Window, 
   */
  void show_window(void) { XMapWindow(s_X11->XD, XW);   XFlush(s_X11->XD);}
  void hide_window(void) { XUnmapWindow(s_X11->XD, XW); }

  /**
   * OPENGL SECTION [WGL]
   * IMPL: in [amGHOST_SystemX11.cpp]
   */
  amGHOST_Context* opengl_create_context(void);
  void           opengl_activate_context(void);

  /**
   * VULKAN SECTION
   */
#if defined(amGHOST_BUILD_VULKAN)
  VkSurfaceKHR create_vulkan_surface(VkInstance instance);
#endif
};