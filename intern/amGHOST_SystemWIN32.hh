#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

#ifndef amGHOST_SYSTEM
  #include "amGHOST_System.hh" //Has Event, EK, Window, Types included
#endif
#ifndef amGHOST_WINDOW
  #include "amGHOST_Window.hh"
#endif
#ifndef amGHOST_EVENT
  #include "amGHOST_Event.hh"
#endif


/**
* Currently Only amGHOST_ContextVK's CONSTRUCTOR is the Only Use, 
* We Needed to save Platform Specific Things like hwnd, hInstance [Just Like we do in amGHOST_ContextWGL.hh], Thus also the CONSTRUCTOR had to have OS_Specific Impl.
* FOrtunately for OpenGL we have multiple Files WGL/OGL/EGL, But not for Vulkan. We wanted to Keep Vulkan As CLEAN as Possible, 
* [We Even Made a Separate amVK Library for that reason, to Have a Better Looking and FASTER UNDERSTANDABLE IMPL.]
* So we created a amGHOST_ResourceHandle in amGHOST_Types.h and OS_Specific Implementations like [amGHOST_ResourceWIN32/X11] in OS Specific amGHOST_SystemOS.hh 
*/
typedef struct amGHOST_ResourceWIN32__ {
 public:
  HWND    hwnd      =     nullptr;
  HMODULE hInstance =     nullptr;
} amGHOST_ResourceWIN32;

class amGHOST_SystemWIN32 : public amGHOST_System
{
 public:
  amGHOST_SystemWIN32();
  bool destroyer(void);
  ~amGHOST_SystemWIN32() {};

  /**
    Either You can try to guess which is what, or you can \see Parameter Documentation in amGHOST_System.h
  */
  amGHOST_Window* create_window(char *title, int posX, int posY, int sizeX, int sizeY, bool instantShow = true);
  bool destroy_window(amGHOST_Window* window);
  bool process_events(bool waitForEvent);
  bool opengl_load(void);





  /** 
    -------------------------------------
    LOWER LEVEL (1LVL LOW) FUNCTIONS.... 
    THINGS THAT FUNCTIONS ABOVE CALL
    MOST OF THESE are Platform Specific
    -------------------------------------

    \function Register the WNDCLASS
  */
  
  //VARIABLES which are Needed Throughout amGHOST_SystemWIN32, amGHOSTWindowWIN32, amGHOST_ContextWGL.... I mean these are just WIN32 related vars
  WNDCLASS s_wc;
  const char* s_wndClassName = "amGHOST_WindowClass"; //wc.lpszClassName has to be set to this, and should be passed to windows that we want to create
  HMODULE s_hInstance = ::GetModuleHandleA(nullptr);  //NULL returns initial main EXE file's Handle
 protected:
  /**
  * Create and Register the s_wc variable that you can see above.
  * NOTE: Every Window in win32 must have a WNDCLASS 
  * which we link by passing in s_wndClassName to CreateWindowA Win32 Function
  * This function could have been static, But we didn't make it bceause, in this file ususally there are already a lot of static funcs as WndProc is where we handle Events and MSVC needed that to be static
  */
  bool reg_wc(void);

  /**
    Neccessary things that are done for getting WM_INPUT Message From input Device/Devices
    TODO: Update docs on this One
  */
  static void init_rawInput(void);

  /**
    \see process_events(), that function will call the below function for Processing Keys
  */
  static void process_keyEvent(amGHOST_Window *window, RAWKEYBOARD *raw, amGHOST_Event* event);
  

  amGHOST_Window *get_window(HWND hwnd);
  /**
    win32 (Window Procedure Function) - Deals with OS Messages and Events, hwnd is the Handle to Window

    C++ treats member functions and free functions as different - member functions need to have access to a this pointer, and typically that's passed in as a hidden first parameter.
    You can, however, declare WndProc as a static member function, which eliminates the this pointer. [https://stackoverflow.com/a/17221900]
  */
 public:
  //EASTER:- Let's See if anyone can use the Power of this being Public, I mean you could just call amGHOST_SystemWIN32::WndProc() pass in params, That's It POWERRRRRR
  static LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};