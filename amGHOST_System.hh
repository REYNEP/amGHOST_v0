#pragma once
#ifndef amGHOST_SYSTEM
#define amGHOST_SYSTEM

/** 
 * None of these Headers Below Should Include Big Libraries like glew.h
 * Blender's GHOST originally used INCLUDE GUARDS, so I started with that too...
 * 
 *   OpenGL Support is Broken at this Moment [JULY 20 2021]
 * 
 *   NOW: trying to make a deque workaround
 *  \todo: change ifdef(_WIN32) usage....
 */

#ifndef amGHOST_TYPES
  #include "amGHOST_Types.h"
#endif
#ifndef amGHOST_EVENT
  #include "amGHOST_Event.hh"
#endif
#ifndef amGHOST_EK
  #include "amGHOST_EK.hh"
#endif
#ifndef amGHOST_CONTEXT
  #include "amGHOST_Context.hh"
#endif
#ifndef amGHOST_WINDOW
  #include "amGHOST_Window.hh"
#endif

#include <amVK_Logger.hh>
#include <amVK_Utils.hh>



/**
 * PART I: INIT & EXIT
 * PART II: EK & EVENT SYSTEM Functions  [Main-Loop Functions Too!]
 * PART III: Load Graphics-LibrariAPI [OpenGL/VULKAN/OpenGLES/DirectX11/DirectX12/Mantle/Metal]
 * PART IV: GENERIC STUFFS [eg. windowCreation, Pop-Ups, Time, Directories] [Also See amGHOST_Window.hh, amGHOST_Context.hh for more functions]

 * You can't just instantiate an amGHOST_System Object, Because this Class contains Pure Virtual Functions (a.k.a This is called an Abstract Class)
 * You have to call amGHOST_System::create_system(); to create a System for your Platform
 * And at the end of the program amGHOST_System::dispose_system() is preferred to be called or MEMORY Will be leaked
 * 
 * 1 System could have multiple rendering Contexts. 
 * Maybe even Multiple Graphics APIs. So Variables like 'glewLoaded'....
 */
class amGHOST_System
{
  protected:
    amVK_ArrayDYN<amGHOST_Event> _EventQ = amVK_ArrayDYN<amGHOST_Event>(256);   /** \see \fn _get_nextSpot & \fn dispatch_events()*/
    amVK_ArrayDYN<amGHOST_EventKonsument> T_EKVec = amVK_ArrayDYN<amGHOST_EventKonsument>(8);
    amVK_ArrayDYN<amGHOST_Window *> T_WindowVec = amVK_ArrayDYN<amGHOST_Window *>(8);
    //amVK_ArrayDYN<amGHOST_Context> T_ContextVec;  [WIP]

  public:
    bool glewLoaded = false;  /** What an Crappy OLD way of doing stuffs. Just do me a Favor and USE Vulkan */

    /**
     * Power Of STATIC:- https://www.youtube.com/watch?v=V-BFlMrBtqQ [You can't call Non-Static Functions without Instantiating a Object, Remember when we talked V-Tables are created for every Object that we instantiated?] 
     * Let's say our Compiler doesn't complain even if we do that, What good it would do to make These Functions non-static? Literally nothing. Cz these no use inside other methods/more than 1ce/& amGHOST_System is Abstract Class
     * 
     * Note: this is something I wrote around MID-2020, i might be wrong 😉, I was just 15years old back then
     * THE ONE AND ONLY HEART  [def: nullptr in .cpp]
     */
    static inline amGHOST_System* heart = nullptr;              /** C++17 */



    /**
     * -----------------------------------
     *        ____________________
     *       |PART I: INIT & EXIT|
     *       ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
     * -----------------------------------
     */
    static void create_system(void);
    static amGHOST_System* get_system(void);

    // You gotta destroy Windows Separately
    void dispose_system(void);

  protected:
    amGHOST_System(const amGHOST_System&) = delete;             /** Brendan's Solution */
    amGHOST_System& operator=(const amGHOST_System&) = delete;  /** Brendan's Solution */
    amGHOST_System() {};                                        /** because of the line below.... there will be no default CONSTRUCTOR */
    virtual ~amGHOST_System() = default;                        /** -Wnon-virtual-dtor   [using G++/GCC] */
                                                                /** making it virtual means, even if you  `delete amGHOST_System *system`  it will call e.g. `~amGHOST_SystemWIN32()` */



  public:
  /**
   * ---------------------------------------------
   *     _____________________________________
   *    |PART II: EK & Event System Functions|
   *    ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
   * ---------------------------------------------
   * Support for C-API and Mixing Both Capi and CPP kind of EKs in the same amGHOST System
   * Also \see amGHOST_EK.hh for EK doc
   * And If you are a Dev of amGHOST, See amGHOST_Capi.cpp file's This Part
   */

  /**
    \see EK DOCS:- amGHOST_EK.hh
    \param event_konsument_proc: Event Konsument Process Pointer
    \return Poitner to that new EK Object/Instance or NULL if already an EK Exists in T_EKVec
  */
  amGHOST_EventKonsument* add_eventKonsument(amGHOST_EKProcPtr event_konsument_proc) {
    for (uint32_t k = 0, n = T_EKVec.neXt; k < n; k++) {
      if (T_EKVec.data[k].get_EKproc() == event_konsument_proc) {
        amVK_LOG("Already an Event konsument added with that exact same function that you have passed");
        return nullptr;
      }
    }

    if (T_EKVec.should_resize()) {T_EKVec.resize(2);}
    amVK_ARRAY_PUSH_BACK(T_EKVec) = amGHOST_EventKonsument(event_konsument_proc);

    return T_EKVec.data + T_EKVec.neXt-1;
  }
  inline bool add_T_EKVec(amGHOST_EventKonsument* new_EK, bool checkIfExists) { //Currently only for the Sake of usage inside amGHOST_Capi.cpp
    return add_eventKonsument(new_EK->get_EKproc()) == nullptr ? false : true;
  }



  /**
    Dispatch every Event one-by-one to ALL EKs
    \see process_events Below [virtual function]
    \return false if any of the EK returns -1, otherwise return true after dispatching all events in EVENTQ
            that event is kept.... but not at same address anymore....
            if you use this feature.... then its best that you only work with 1 EK....
  */
  bool dispatch_events(void) {
    //amVK_LOG("Dispatching events: " << _EventQ.neXt);
    for (uint32_t i = 0, n = _EventQ.neXt; i < n; i++) 
    {
      for (uint32_t k = 0, n = T_EKVec.neXt; k < n; k++) 
      {
        //amVK_LOG("  TO: " << (uint64_t)T_EKVec.data+k << "    nd the event: " << (uint64_t)_EventQ.data + i << "     []: " << _EventQ.data[i].m_type);
        if (T_EKVec.data[k].dispatch_event(_EventQ.data + i) == -1) {
          if (i > 0) {
            /** this won't be that expensive, anyway */
            amVK_memcpy(_EventQ.data, _EventQ.data + i, sizeof(amGHOST_Event) * (_EventQ.neXt-i));
          }
          amVK_LOG_EX("Returning FALSE");
          return false;
        }
      }
    }

    _EventQ.neXt = 0;
    return true;
  }
  /**
   *   Checks if any event in Queue
   *   Usage: in case if you want to know if the eventQ is empty or not after calling process_events();
   */
  bool is_eventQ_empty(void) {
    return _EventQ.neXt == 0;
  }

  amGHOST_Event* _get_nextSpot(void) {
    if (_EventQ.should_resize()) {
      _EventQ.resize(4);
    }

    _EventQ.neXt++;   /** \todo mix these 2 lines */
    //amVK_LOG("next SPOT: " << _EventQ.neXt-1);
    return &_EventQ.data[_EventQ.neXt-1];
  }

  /** used inside amGHSOT_System_WIN32::WndProc */
  void _dismiss_last_event(void) {
    if (_EventQ.neXt != 0) {
      _EventQ.neXt--;
    }
  }






  /** 
  * ---------------------------------------------
  *      ___________________________________
  *     |PART III: Load Graphics-LibraryAPI|
  *     ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  * ---------------------------------------------
  */
  virtual bool opengl_load(void) = 0;


  /** 
  * -----------------------------------
  *      ________________________
  *     |PART IV: GENERIC STUFFS|
  *     ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  * -----------------------------------
  * for more \see amGHOST_Window.h : openGL_create_context(), show_window(), hide_window()
  */

  /**
    Creates a New Window obviously
    \param title: should be ASCII characters only (as its char*)
    \param posX: =_= Position in X-axis
    \param posY: +_+ Position in Y-axis
    \param sizeX: WIDTH
    \param sizeY: HEIGHT
    \param instantShow: Yes, WIN32/X11 does distinguish between creating the window in memory and actually showing the Window on Screen/Monitor. SO WE Give you the choice too. Later you can use window::show_window
  */
  virtual amGHOST_Window* create_window(char *title, int posX, int posY, int sizeX, int sizeY, bool instantShow = true) = 0;
  virtual bool destroy_window(amGHOST_Window* window) = 0;

  /**
    Process Events and send the Events to the EventQ (a std::deque type)
    \param waitForEvent: Currently waitForEvent is STRONGLY RECOMMENED TO BE 1
    \return true if there was any event processed, false if waitForEvent is false + No message in queue
  */
  virtual bool process_events(bool waitForEvent) = 0;
};

#endif //#ifndef amGHOST_SYSTEM