#pragma once

#define XLIB_ILLEGAL_ACCESS
#include <X11/Xlib.h>
#include "amVK_Logger.hh"

#ifndef amGHOST_SYSTEM
  #include "amGHOST_System.hh" //Has Event, EK, Window, Types included
#endif
#ifndef amGHOST_WINDOW
  #include "amGHOST_Window.hh"
#endif



/**
 *      TODO: 
 * In general, when a request terminates with an error, the request has no side effects (that is, there is no partial execution). 
 * The only requests for which this is not true are ChangeWindowAttributes, ChangeGC, PolyText8, PolyText16, FreeColors, StoreColors and ChangeKeyboardControl.
 * 
 * More on ERRORS: https://stackoverflow.com/a/4457377
 */
int amGHOST_X11_ErrorHandler(Display *display, XErrorEvent *event);
int amGHOST_X11_IOErrorHandler(Display * /*display*/);



/**
 *      NOTE:
 *  1.  [CONSTRUCTOR]:    XOpenDisplay()  defaults to the value of the DISPLAY environment variable.   [ \see tronche.com for more on this (display_name) ]
 *  2.  XLIB_ILLEGAL_ACCESS:              `Display *XD`  -   that is now accessible
 *  3.       [XFLUSH]:    XNextEvent()    WINDOW WONT BE SHOWN untill     XNextEvent is called....   that may seem like it... cz XNextEvent flushes XDisplay.... thats needed after XMapWindow, iguess
 *                                        [REF:  https://linux.die.net/man/3/xpending]
 *                                              https://en.wikipedia.org/wiki/Xlib#Protocol_and_events
 * 
 *      TODO:
 * 1. [MultiDisplay]:   `Display* XD`    variable means.... this only supports only 1 display???
 * 2. [MultiDisplay]:                    Multiple screens can be controlled by a single X server.
 * 3. [ERROR_HANDLE]:  IOErrorHandler    Not handled properly... cz no XErrorEvent is given in this case.... see RESC-3
 * 4. XFLUSH needed after XMAP / XUnmapWindow   [show_window/hide_window]???
 * 
 *      RESC:
 *    1. https://tronche.com/gui/x/xlib/display/opening.html
 *    2. https://github.com/mirror/libX11/blob/5faa8dc0b364f55f19034c370436074646d9ad54/include/X11/Xlib.h#L123
 *    3. https://tronche.com/gui/x/xlib/event-handling/protocol-errors/XSetIOErrorHandler.html
 *    4. https://www.x.org/releases/X11R7.6/doc/xproto/x11protocol.html#errors
 *    5. [I Hate Xlib, so Should you](https://www.remlab.net/op/xlib.shtml)
 *    6. XCB: [https://github.com/wyatt8740/x11-window-hider/blob/master/hide.c]
 */
class amGHOST_SystemX11 : public amGHOST_System {
 public:
  amGHOST_SystemX11() {
    XD = XOpenDisplay(nullptr);
    if (XD == nullptr) {
      amVK_LOG_EX("XOpenDisplay(nullptr) failed....");
      return;
    }

    XW_attribs.override_redirect = False;
    XW_attribs.background_pixel = 0;
    XW_attribs.border_pixel = 0;
    XW_attribs.event_mask = ExposureMask | KeyPressMask;

    /** nullptr = default handler */
      XSetErrorHandler(amGHOST_X11_ErrorHandler);
    XSetIOErrorHandler(amGHOST_X11_IOErrorHandler);
  }
 ~amGHOST_SystemX11() {
    if (XD != nullptr) XCloseDisplay(XD);
  }

 public:
  Display *XD = nullptr;
  /** X11 Window Attributes */
  XSetWindowAttributes XW_attribs{};
  uint32_t XW_border_width = 0;


 public:
  inline Screen *get_DefaultScreen(void) {return &XD->screens[XD->default_screen];}
  inline Screen *get_DefaultScreen(Display *D) {return &D->screens[D->default_screen];}


  /**
   * base 'amGHOST_System' stuffs
   */
  amGHOST_Window* create_window(const char *title, int posX, int posY,  int sizeX, int sizeY, bool instantShow = true);
  bool destroy_window(amGHOST_Window* window);
  bool process_events(bool waitForEvent);
  bool opengl_load(void);
};