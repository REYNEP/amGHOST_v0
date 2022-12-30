#ifndef amGHOST_SystemWIN32_HH
#define amGHOST_SystemWIN32_HH

#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

#ifndef amGHOST_SYSTEM
  #include "amGHOST_System.hh"
#endif
#ifndef amGHOST_WINDOW
  #include "amGHOST_Window.hh"
#endif
#ifndef amGHOST_EVENT
  #include "amGHOST_Event.hh"
#endif



typedef struct amGHOST_ResourceWIN32__ {
 public:
  HWND    hwnd      =     nullptr;
  HMODULE hInstance =     nullptr;
} amGHOST_ResourceWIN32;


/**
 *  \fn reg_wc() & \fn init_rawInput() is really old 😄 I am just gonna let them be like this... not change them ever... maybe make Different versions 🤔 
 */
class amGHOST_SystemWIN32 : public amGHOST_System
{
 public:
  amGHOST_SystemWIN32() {_LOG("amGHOST_SystemWIN32 Initialized.");    reg_wc(); init_rawInput();}
 ~amGHOST_SystemWIN32() {_LOG("amGHOST_SystemWIN32 Destructing.");  unreg_wc(); }


  /** PureVirtual Funcs from amGHOST_System */
  amGHOST_Window* create_window(const char *title, int posX, int posY, int sizeX, int sizeY, bool instantShow = true);
  bool destroy_window(amGHOST_Window* window);
  bool process_events(bool waitForEvent);
  bool opengl_load(void);



 public:
  /** Win32 Related variables */
  WNDCLASS s_wc;                                      /** Every Window in win32 must have a WNDCLASS */
  const char* s_wndClassName = "amGHOST_WindowClass"; /** which we link by passing in s_wndClassName to \fn CreateWindowA */
  HMODULE s_hInstance = ::GetModuleHandleA(nullptr);  /** \param nullptr returns initial main EXE file's Handle */

 protected:
  amGHOST_Window *get_window(HWND hwnd);
  bool reg_wc(void);
  void unreg_wc(void);

  /**
   * Called in CONSTRUCTOR
   * Neccessary things that are done for getting WM_INPUT Message From input Device/Devices
   * TODO: Update docs on this One
   */
  static void init_rawInput(void);
  static void process_keyEvent(amGHOST_Window *window, RAWKEYBOARD *raw, amGHOST_Event* event);
  

 public:
  /**
   * called by process_events() -> DispatchMessageA
   * 
   * win32 (Window Procedure Function) - Deals with OS Messages and Events, hwnd is the Handle to Window
   * 
   *   C++ treats member functions and free functions as different - member functions need to have access to a this pointer, and typically that's passed in as a hidden first parameter.
   *   You can, however, declare WndProc as a static member function, which eliminates the this pointer. [https://stackoverflow.com/a/17221900]
   * 
   * EASTER:- Let's See if anyone can use the Power of this being Public, I mean you could just call amGHOST_SystemWIN32::WndProc() pass in params, That's It POWERRRRRR
   */
  static LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif