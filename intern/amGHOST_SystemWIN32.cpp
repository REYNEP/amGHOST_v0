#include "amGHOST_Logger.hh"
#include "amGHOST_SystemWIN32.hh"
#include "amGHOST_WindowWIN32.hh"

//Variables with 's_' are defined inside amGHOST_SystemWIN32 class

amGHOST_SystemWIN32::amGHOST_SystemWIN32() {
  this->reg_wc();
  this->init_rawInput();
}

/**
* DESTRUCTOR
* All the Windows, Contexts and Things are destroyed when dispose_system() is called....[Inside amGHOST_System.cpp]
* But some functions like UnregisterClassA are only Win32 SPecific that's Why we have this DESTRUCTOR
*/
bool amGHOST_SystemWIN32::destroyer(void) {
  if (!::UnregisterClassA(s_wndClassName, s_hInstance)) {
    LOG_DEV("Window Class wasn't found or a Window using this class still exists exists....");
    return false;
  }
  return true;
}





/** 
 * |------------|
 * - LibamGHOST -
 * |------------|
*/
#ifdef amGHOST_BUILD_OPENGL
  #include "amGHOST_ContextWGL.hh"
#endif
bool amGHOST_SystemWIN32::opengl_load(void) {
#ifdef amGHOST_BUILD_OPENGL
  return amGHOST_ContextWGL::opengl_load();
#else
  LOG("amGHOST_OPENGL macro Not defined when compiling.... [WARNING: Note that If you define Multiple Context Macros like amGHOST_VULKAN and amGHOST_OPENGL, your Application can get heavy!!!!]");
  return false;
#endif
}




amGHOST_Window* amGHOST_SystemWIN32::create_window(char *title, int posX, int posY, int sizeX, int sizeY, bool instantShow) {
  amGHOST_WindowWIN32 *newCreatedWindow = new amGHOST_WindowWIN32(title, posX, posY, sizeX, sizeY);
  if (!newCreatedWindow->m_hwnd) {  //ERROR Checking
    newCreatedWindow->destroyer();
    return NULL;
  }
  else {
    if (instantShow) {newCreatedWindow->show_window();}
    T_WindowVec.push_back((amGHOST_Window *) newCreatedWindow);
  }

  return (amGHOST_Window *) newCreatedWindow;
}

//TODO: UPdate this and Decide what to do with all those vectors inside amGHOST_System.hh
bool amGHOST_SystemWIN32::destroy_window(amGHOST_Window* window) {
  if (window) {
    amGHOST_WindowWIN32 *window_conv = (amGHOST_WindowWIN32 *) window;
    window_conv->destroyer();

    int index = 0;
    for (amGHOST_Window *W : T_WindowVec) {
      if (W == window) {
        T_WindowVec.erase(T_WindowVec.begin() + index);
        break;
      }
      index += 0;
    }

    return true;
  }
  return false;
}



/*
ATTENTION, EVENT SYSTEM CODE BELOW 😉 
*/
bool amGHOST_SystemWIN32::process_events(bool waitForEvent) {
  MSG msg;

  if (!::PeekMessageA(&msg, NULL, 0, 0, PM_NOREMOVE)) {
    if (waitForEvent) {
    #if 0
      ::Sleep(1);
    #else
      /* GHOST SUPPORTS TIMERS For Tasks 

      GHOST_TUns64 next = timerMgr->nextFireTime();
      GHOST_TInt64 maxSleep = next - getMilliSeconds();

      if (next == GHOST_kFireTimeNever) {
        ::WaitMessage();
      }
      else if (maxSleep >= 0.0) {
        ::SetTimer(NULL, 0, maxSleep, NULL);
        ::WaitMessage();
        ::KillTimer(NULL, 0);
      */
      ::WaitMessage();  
      /*Seems Like WaitMessage Actually doesn't go into sleep, rather it returns after a certail time, say for 500ms
        LOG("WaitMessage Returned")   // [You will see this getting printed a couple of times persecond]
        TODO: Implement Blender's SetTimer Approach as that is also Win32 Library Function */
      goto weGotEvent;
    #endif
    }

    return false;
  }

  weGotEvent:
  {
    while (::PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);    //Dispatches The Message to a WindowProcedure a.k.a WndProc [SEE BELOW]
    }
  }
  return true;
}




/** 
 * -------------------------------------
 * LOWER LEVEL (1LVL LOW) FUNCTIONS.... 
 * THINGS THAT FUNCTIONS ABOVE CALL
 * MOST OF THESE are Platform Specific
 * [CURRENTLY THESE ARE ALL STATIC]
 * -------------------------------------
 *
 * \function Register the WNDCLASS
 */
bool amGHOST_SystemWIN32::reg_wc() {
  LOG_DEV("WNDCLASS has been created in amGHOST_Win32.h" << std::endl << " HOW U DOIN'? Never Really Thought a Preprocessor Could be This Amazing....");
  WNDCLASS wcTMP;

  //openGL requires it to be CS_OWNDC, see https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)
  wcTMP.style = CS_OWNDC;
  //'&WndProc' is the same as 'WndProc', WndProc is a method of amGHOST_Win32 (a.k.a amGHOST_Win32::WndProc) [Yes, We can use func ptr as argument]
  wcTMP.lpfnWndProc = WndProc;
  //Instance that the Windows using this WNDCLASS would get Linked with and communicate THROUGH, eg. MSG messages, events [THINK of it like a gate]
  wcTMP.hInstance = s_hInstance;
  //Better Off leaving these at 0. Simply saying these're Params to allocate bytes to the WNDClass. Access extra Bytes with get/setClassLong()
  wcTMP.cbClsExtra = 0;
  wcTMP.cbWndExtra = 0;
  //LoadIcon Needs NULL param when loading system Standard Icon, This Probably Only Loads the ICON only, And Displaying it is on a whole other lvl
  wcTMP.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
  //LoadCursor Also Needs NULL Param when Loading System Standard Cursor, This Loads the Cursor which is Used as Default, later you can change
  wcTMP.hCursor = ::LoadCursor(NULL, IDC_ARROW);
  //I would like to call this background color
  wcTMP.hbrBackground = 0x00000000;
  //You will need to pass name of the menu that you create and want to associate with the window
  wcTMP.lpszMenuName = NULL;
  //CreateWindow will need this, (although i think passing &wc in CreateWindow would have been better, as WNDCLASS is a typedef struct)
  wcTMP.lpszClassName = s_wndClassName;


  s_wc = wcTMP;
  //TODO: USE RegisterClassW
  if (!::RegisterClass(&(s_wc))) {return false;}
  return true;
}

void amGHOST_SystemWIN32::init_rawInput() {
  //The Code Below Was Copied From Blender's GHOST....Except for the the devices[0].dwFlags Line

  #define DEVICE_COUNT 1

  RAWINPUTDEVICE devices[DEVICE_COUNT];
  memset(devices, 0, DEVICE_COUNT * sizeof(RAWINPUTDEVICE));

  // Initiates WM_INPUT messages from keyboard
  // That way GHOST can retrieve true keys
  devices[0].usUsagePage = 0x01;
  devices[0].usUsage = 0x06;  //Means that It's a Keyboard that we are Talking about
  devices[0].dwFlags = RIDEV_NOLEGACY; //Means that the keyboards will no longer do WM_KEYDOWN or WM_SYSKEYDOWN, dwFlags can only be 1 flag even if the name vibes multiple

  if (::RegisterRawInputDevices(devices, DEVICE_COUNT, sizeof(RAWINPUTDEVICE)))
    ;  // yay!
  else 
    LOG_DEV("RegisterRawInputDevices Failed!!");

  #undef DEVICE_COUNT
}

amGHOST_Event* amGHOST_SystemWIN32::process_keyEvent(amGHOST_Window* window, RAWKEYBOARD *raw) {
  short vKey = raw->VKey;
  amGHOST_TKey key;

  if ((vKey >= '0') && (vKey <= '9')) {
    // VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39)
    key = (amGHOST_TKey)(vKey - '0' + amGHOST_kKey0);
  }
  else if ((vKey >= 'A') && (vKey <= 'Z')) {
    // VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A)
    key = (amGHOST_TKey)(vKey - 'A' + amGHOST_kKeyA);
  }
  else if ((vKey >= VK_F1) && (vKey <= VK_F24)) {
    // VK_F1 thru VK_F24 are (0x41 - 0x5A)
    key = (amGHOST_TKey)(vKey - VK_F1 + amGHOST_kKeyF1);
  }
  else {
    switch(vKey) {
      default:
        key = amGHOST_kKeyUnknown;
        break;
    }
  }


  amGHOST_Event* event = NULL;
  /** 
    \see: https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawkeyboard
    You see the flags Section? there are 4 Flags, 
    RI_KEY_MAKE is when every bit is zero
    RI_KEY_BREAK means first bit is ON
    RI_KEY_E0 means second bit is ON
    RI_KEY_E1 means third bit is ON
  */
  if (raw->Flags & RI_KEY_BREAK) {
    LOG(raw->Flags);
    event = new amGHOST_Event(amGHOST_kKeyUp, window, key);
  }
  else {
    LOG(raw->Flags);
    event = new amGHOST_Event(amGHOST_kKeyDown, window, key);
  }
  

  return event;
}



/** 
 * -------------------------------------
 * SECOND LOWER LEVEL (2LVL LOW) FUNCTIONS.... 
 * WINDOWS SPECIFIC FUNCTIONS [see .h of this file, these are all static funcs]
 * MOST Of these are 'MUST REQUIREMENT' For Win32 Apps/Programs
 * Some are Executed Automatically or By other WIN32 Functions
 * -------------------------------------
 *
 * For Docs On these Functions, Check MSDN
 */
amGHOST_Window *amGHOST_SystemWIN32::get_window(HWND hwnd) {
  for (amGHOST_Window *W : T_WindowVec) {
    amGHOST_WindowWIN32 *W_conv = (amGHOST_WindowWIN32 *) W;
    if (W_conv->m_hwnd == hwnd) {
      return W_conv;
    }
  }
  return NULL;
}
LRESULT WINAPI amGHOST_SystemWIN32::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  LRESULT lResult = 0;
  if (!hwnd) {
    // Events without valid hwnd
    LOG("GHOST_SystemWin32::wndProc: event without window\n");
    return lResult;
  }

  //amGHOST_Event members
  amGHOST_SystemWIN32 *m_system = (amGHOST_SystemWIN32 *) get_system();
  amGHOST_Window *event_win = m_system->get_window(hwnd);
  
  amGHOST_Event *event = NULL;

  /** LOG Events....
  if ((msg != 132) && (msg != 160) && (msg != 512) && (msg != 32)){
    LOG("wndProc: " << msg);
  } */
  switch (msg)  //must break after each case.... cz case is like 'Label'  and if you don't break the flow of code will continue
  {
    case WM_PAINT:
      PAINTSTRUCT ps;
      BeginPaint(hwnd, &ps);
      EndPaint(hwnd, &ps);
      break;
    case WM_DESTROY:
      event = new amGHOST_Event(amGHOST_kWindowClosed, event_win, amGHOST_kKeyUnknown);
      break;
    case WM_CLOSE:
      event = new amGHOST_Event(amGHOST_kWindowClose, event_win, amGHOST_kKeyUnknown);
      break;
    case WM_SIZING:
      event = new amGHOST_Event(amGHOST_kWindowResizing, event_win, amGHOST_kKeyUnknown);
      break;
    case WM_SIZE:
      event = new amGHOST_Event(amGHOST_kWindowResized, event_win, amGHOST_kKeyUnknown);
      break;

    /* ----------------------------------------------------------------
     * Keyboard Events Processed With WM_INPUT.... 
     * [Mouse can be too... But neither MouseMove or MouseButtonDown gens WM_INPUT]
     * ---------------------------------------------------------------- */
    case WM_INPUT: {
      /** ~~~~~~~~~~~~~~~~~~
      * [BLENDER's GHOST Implimentation was a Major Inspiration for this Section]
      * ----------------------------------------------------------------------------------
      * Every KEYBOARD, MOUSE, OTHER-RAW DEVICE BUTTONS Should be Processed here
      * Because WM_KEYDOWN, WM_XBUTTONDOWN are Legacy Things of windows.
      * WM_XBUTTONDOWN supports only 2 Other Mouse Buttons (Win32's VKeys too)
      * WM_KEYDOWN Isn't Sent when any kind of key is Pressed with ALT-key DOWN
      * ----------------------------------------------------------------------------------
      * Look at these For Furthur Explorations:-
      * https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getasynckeystate
      */ // ~~~~~~~~~~~~~~~~

      // Below 4Lines copied from blenderGHOST code
      // check WM_INPUT from input sink when ghost window is not in the foreground
      if (wParam == RIM_INPUTSINK) {
        if (::GetFocus() != hwnd)  // WM_INPUT message not for this window
          return 0;
      }  // else wParam == RIM_INPUT

      RAWINPUT raw;   UINT rawSize = sizeof(RAWINPUT);
      //Can't call lParam.header.dwtype directly, the Function below is the only way
      ::GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &raw, &rawSize, sizeof(RAWINPUTHEADER));

      switch(raw.header.dwType) 
      {
        case RIM_TYPEKEYBOARD: 
        {
          RAWKEYBOARD *raw_key_data_ptr = &(raw.data.keyboard);
          
          //TODO Error Handling inside process_keyEvent
          event = process_keyEvent(event_win, raw_key_data_ptr);
          if (!event) {
            LOG("KEY IGNORED: " << msg << "(amGHOST_SystemWIN32::WndProc key event)");
          }
        }

        //TODO: Include HID SUPPORT
      }
      break;
    }
    case WM_MOUSEMOVE:
    {
      RAWINPUT raw;
      RAWINPUT *raw_ptr = &raw;
      UINT rawSize = sizeof(RAWINPUT);
      //Can't call lParam.header.dwtype directly, the Function below is the only way
      ::GetRawInputData((HRAWINPUT)lParam, RID_INPUT, raw_ptr, &rawSize, sizeof(RAWINPUTHEADER));

      RAWMOUSE *raw_mouse_data_ptr = &(raw.data.mouse);

      int xPosRelative = raw_mouse_data_ptr->lLastX;
      int yPosRelative = raw_mouse_data_ptr->lLastY;
      if ((xPosRelative != 0) || (yPosRelative != 0)) {
        POINT point;
        amGHOST_TMicePos mousePos;
        if (::GetCursorPos(&point)) {
          mousePos.x = point.x;
          mousePos.y = point.y;
        }

        event = new amGHOST_Event(amGHOST_kMCursorMove, event_win, mousePos);
      }
      break;
    }
    case WM_MOUSEWHEEL:
      event = new amGHOST_Event(amGHOST_kMWheel, event_win, ((short)HIWORD(wParam) < 0) ? amGHOST_kWheelDown : amGHOST_kWheelUp);
      break;

    case WM_LBUTTONDOWN:
      event = new amGHOST_Event(amGHOST_kMButton, event_win, amGHOST_kLMBDown);
      break;
    case WM_MBUTTONDOWN:
      event = new amGHOST_Event(amGHOST_kMButton, event_win, amGHOST_kMMBDown);
      break;
    case WM_RBUTTONDOWN:
      event = new amGHOST_Event(amGHOST_kMButton, event_win, amGHOST_kRMBDown);
      break;

    case WM_LBUTTONUP:
      event = new amGHOST_Event(amGHOST_kMButton, event_win, amGHOST_kLMBUp);
      break;
    case WM_MBUTTONUP:
      event = new amGHOST_Event(amGHOST_kMButton, event_win, amGHOST_kMMBUp);
      break;
    case WM_RBUTTONUP:
      event = new amGHOST_Event(amGHOST_kMButton, event_win, amGHOST_kRMBUp);
      break;

    case WM_CONTEXTMENU:
      break;  //on WM_RBUTTONUP this is given too....

    case WM_KEYDOWN:
      break;
    case WM_SYSKEYDOWN:
      break;
    case WM_KEYUP:
      break;
    case WM_SYSKEYUP:
      break;
    
    // WITHOUT THE DEFAULT, Windows might Think of a program as Not Responsing
    default:
      lResult = ::DefWindowProc(hwnd, msg, wParam, lParam);
      break;
  }

  if (event) {
    //HERE WE QUEUE the EVENT in the EventQ which is a Member of amGHOST_System
    m_system->add_toEventQ(event);  
  }

  return lResult;
}
void amGHOST_SystemWIN32::add_toEventQ(amGHOST_Event *event) {
  EventQ.push_back(event);
}