//Include Guard
#ifndef amGHOST_SYSTEM
#define amGHOST_SYSTEM

//None of these Headers Below Should Include Big Libraries like glew.h
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
//OpenGL is Broken at this Moment [JULY 20 2021]

#include <vector>
#include <deque>


/**
PART I: INIT & EXIT
PART II: EK & EVENT SYSTEM Functions  [Main-Loop Functions Too!]
PART III: Load Graphics-LibrariAPI [OpenGL/VULKAN/OpenGLES/DirectX11/DirectX12/Mantle/Metal]
PART IV: GENERIC STUFFS [eg. windowCreation, Pop-Ups, Time, Directories] [Also See amGHOST_Window.hh, amGHOST_Context.hh for more functions]
*/
/**
* You can't just instantiate an amGHOST_System Object, Because this Class contains Pure Virtual Functions (a.k.a This is called an Abstract Class)
* You have to call amGHOST_System::create_system(); to create a System for your Platform
* And at the end of the program amGHOST_System::dispose_system() is preferred to be called or MEMORY Will be leaked
*/
class amGHOST_System
{
 protected:
  /**
  * Flexible Vectors to store just the Pointers. Best way to Track Allocated Classes with the. Just Kidding....
  */
  //TODO: Solve These Variables
  std::deque<amGHOST_Event *> EventQ;
  std::vector<amGHOST_EventKonsument *> T_EKVec;
  std::vector<amGHOST_Window *> T_WindowVec;
  std::vector<amGHOST_Context *> T_ContextVec;

 public:
  //1 System could have multiple rendering Contexts. Maybe even Multiple Graphics APIs. So these Variables....
  bool glewLoaded = false;  //What an Crappy OLD way of doing stuffs. Just do me a Favor and USE Vulkan

  /**
  * Power Of STATIC:- https://www.youtube.com/watch?v=V-BFlMrBtqQ [You can't call Non-Static Functions without Instantiating a Object, Remember when we talked V-Tables are created for every Object that we instantiated?] 
  * Let's say our Compiler doesn't complain even if we do that, What good it would do to make These Functions non-static? Literally nothing. Cz these no use inside other methods/more than 1ce/& amGHOST_System is Abstract Class
  */
  static amGHOST_System* heart; //THE ONE AND ONLY HEART  [def: nullptr in .cpp]


  /**
  * -----------------------------------
  *        ____________________
  *       |PART I: INIT & EXIT|
  *       ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  * -----------------------------------
  */
  static void create_system(void);
  static amGHOST_System* get_system(void);
  void dispose_system(void);





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
  amGHOST_EventKonsument* add_eventKonsument(amGHOST_EKProcPtr event_konsument_proc);
  bool add_T_EKVec(amGHOST_EventKonsument* new_EK, bool checkIfExists); //Currently only for the Sake of usage inside amGHOST_Capi.cpp
  /**
    Dispatch every Event one-by-one to ALL EKs
    \see process_events Below [virtual function]
    \return false if any of the EK returns -1, otherwise return true after dispatching all events in EVENTQ
  */
  bool dispatch_events(void);
  /**
    Checks if any event in Queue
    Usage: in case if you want to know if the eventQ is empty or not after calling process_events();
  */
  bool is_eventQ_empty(void);






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