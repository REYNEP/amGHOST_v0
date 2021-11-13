//Include Guard
//amGHOST_ErrorKonsument can be found in amGHOST_Debug.hh
#ifndef amGHOST_EK
#define amGHOST_EK amGHOST_EventKonsument

#ifndef amGHOST_TYPES
  #include "amGHOST_Types.h"
#endif
class amGHOST_Event;

/** 
 * WHY DID I CHOOSE THE NAME "EventKonsument"? Because.... I was stuck finding name for this functionality. Getting Angry, Exhausted, Haunted by Names at night, In my dreams
 * Was Stuck in a loop for over days, close to a week, I needed something to free my mind, or some would event take this act of mine as a result of Adrenaline Rush
 * You can Think that, It;s TOtally fine.... I am gonna be okay.... I just wanted that bad feeling about naming these functions and types to endif
 * SO I just put the German translation of that word. WHY German? Because.... I have a dream to go to netherlands.... that's it.... yeah I do like Dutch and German People
 * we need to have multiple amGHOST_EventKonsument, so we make a std::vector
 * Then again std::vector can be consisted of structs.... and in C++ structs and classes are basically the same, So we make amGHOST_EventKonsument Class
 * Thus We Get Another Advantage, now we can have separate dispose_event() function inside of every EK OBJECT that will be created
 */
class amGHOST_EventKonsument
{
 public:
  amGHOST_EventKonsument(amGHOST_EKProcPtr EK) : m_EK(EK) {}
  ~amGHOST_EventKonsument() {}

  /**
  *\return Whatever the heck EK_Proc returns. There might occur some Special Exceptions like:-
  * '-1' return value from EK_Proc will cause amGHOST_System::dispatch_events(); function to return false 
  * More Special Values to be added Soon
  * [Note: This function is only called by the amGHOST_Syste::dispatch_events()]
  */
  int dispatch_event(amGHOST_Event *event) {
    return m_EK(event);
  }

  /**
  * Sends Back the Function Pointer. Because in amGHOST_System.cpp inside of add_eventKonsument function EK->m_EK_proc wasn't working as m_EK_proc is protected member
  */
  inline amGHOST_EKProcPtr get_EKproc(void) {
    return m_EK;
  }

 protected:
  amGHOST_EKProcPtr m_EK;
};


/**
* Note TO Dev: See amGHOST_Capi.cc EK part, Because C-API Does a Trick to Solve the Issue where 
* C doesn't have the Concept of Classes. But amGHOST CPP api does use (amGHOST_Event*) Class pointer as EK_Func parameter
* Thats why EK_Funcs are differnt in C [from amGHOST_Types.h: typedef int (*amGHOST_EKProcPtr_Capi)(amGHOST_EventHandle);] 
*/

#endif //#ifndef amGHOST_EK