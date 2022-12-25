#pragma once
#ifndef amGHOST_SYSTEM
#define amGHOST_SYSTEM

/** 
 *   Blender's GHOST originally used INCLUDE GUARDS, so I started with that too...
 *      None of these Headers Below Should Include Big Libraries like glew.h
 * 
 *           OpenGL Support is Broken at this Moment [JULY 04 2022]
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
 *        I:  CREATE & DISPOSE
 *       II:      EK & EVENT
 *      III: PROCESS & DISPATCH [event]
 *       IV: Some internally used funcs
 *        V: GENERIC - STUFFS   [window]        [ \todo Pop-Ups, Time, Directories ]
 */
class amGHOST_System
{
   public:
    static inline amGHOST_System* heart = nullptr;              /** C++17 */

   public:
    static         void create_system(void);
    static amGHOST_System* get_system(void);
    /* */         void dispose_system(void);
    /** You should destroyed stuff like Windows Separately, 
        tho we check & try to dispose them all too           */
    
   protected:
    amGHOST_System(const amGHOST_System&) = delete;             /** Brendan's Solution */
    amGHOST_System& operator=(const amGHOST_System&) = delete;  /** Brendan's Solution */

    amGHOST_System() {};                                        /** because of the line below.... there will be no default CONSTRUCTOR */
    virtual ~amGHOST_System() = default;                        /** -Wnon-virtual-dtor   [using G++/GCC] */
                                                                /** making it virtual means, even if you  `delete amGHOST_System *system`  it will call e.g. `~amGHOST_SystemWIN32()` */
   public:
    bool glewLoaded = false;
    virtual bool opengl_load(void) = 0;







   protected:
    amVK_ArrayDYN<amGHOST_Event>          _EventQ = amVK_ArrayDYN<amGHOST_Event>(256);   /** \see \fn _get_nextSpot & \fn dispatch_events()*/
    amVK_ArrayDYN<amGHOST_EventKonsument> T_EKVec = amVK_ArrayDYN<amGHOST_EventKonsument>(8);
    amVK_ArrayDYN<amGHOST_Window *>   T_WindowVec = amVK_ArrayDYN<amGHOST_Window *>(8);
    //amVK_ArrayDYN<amGHOST_Context> T_ContextVec;  [WIP]

   public:
    /**
     * \see EK DOCS:- amGHOST_EK.hh
     * \param event_konsument_proc: Event Konsument Process Pointer   [a.k.a a Function pointer]
     * \return Pointer to that new EK Object/Instance or NULL if already an EK Exists in T_EKVec
     */
    amGHOST_EventKonsument* add_EventKonsument(amGHOST_EKProcPtr event_konsument_proc);


    /**
     * Process Events and send the Events to the EventQ (a std::deque type)
     * \param waitForEvent: Currently waitForEvent is STRONGLY RECOMMENED TO BE 1
     * \return true if there was any event processed, false if waitForEvent is false + No message in queue
     * 
     * TODO: *sleep* support
     */
    virtual bool process_events(bool waitForEvent) = 0;
    /**
     * Dispatch every Event one-by-one to ALL EKs
     * \see process_events Below [virtual function]
     * \return false if any of the EK returns -1, otherwise return true after dispatching all events in _EeventQ
     *         that event is kept.... but not at same address anymore.... tho you can use \fn peek_next_event()
     *         if you use this feature.... then its best that you only work with 1 EK....
     */
    bool dispatch_events(void);

    /** 
     * should be called by user after properly handling amGHOST_kLostSystem, IMPL: XCB 
     * 
     * \return true if Properly handled amGHOST_kLostSystem event.... and system is no more lost
     * 
     * TODO: [HUGE WIP] We gotta work greatly on this... Honestly I don't know if We can recover if SYS is lost
     */
    virtual bool handled_LostSystem(void) {return false;}

    
    inline bool is_eventQ_empty(void) {return _EventQ.neXt == 0;}
    amGHOST_Event peek_next_event()   {return _EventQ.data[0];}




    /** To Add to _EventQ, Internally amGHOST should use this */
    amGHOST_Event* _get_nextSpot(void) {                    /**         IN: amGHOST_System_WIN32::WndProc         */
        if (_EventQ.should_resize()) _EventQ.resize(4);

        _EventQ.neXt++;   /** \todo mix these 2 lines */
        return &_EventQ.data[_EventQ.neXt-1];
    }

    inline void _dismiss_last_event(void) {                 /**         IN: amGHOST_System_WIN32::WndProc         */
        if (_EventQ.neXt != 0) _EventQ.neXt--;
    }











    /**
     *   for more \see amGHOST_Window.h: 
     *      openGL_create_context(), show_window(), hide_window()
     */

    /**
     * Creates a New Window obviously
     * \param title: should be ASCII characters only (as its char*)
     * \param posX: =_= Position in X-axis
     * \param posY: +_+ Position in Y-axis
     * \param sizeX: WIDTH
     * \param sizeY: HEIGHT
     * \param instantShow: Yes, WIN32/X11 does distinguish between creating the window in memory 
     *                     and actually showing the Window on Screen/Monitor. 
     *                     SO WE Give you the choice too.
     * 
     * \todo CAP according to DISPLAY/SCREEN/MONITOR/MULTI_MONITOR resolution
     */
    virtual amGHOST_Window* create_window(const char *title, int posX, int posY, int sizeX, int sizeY, bool instantShow = true) = 0;
    virtual bool destroy_window(amGHOST_Window* window) = 0;
};

#endif //#ifndef amGHOST_SYSTEM