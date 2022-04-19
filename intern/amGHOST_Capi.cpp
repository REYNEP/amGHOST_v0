#include <iostream>
#include <vector>

#include "amGHOST_Capi.h"
#ifndef amGHOST_LOGGER
  #include "amVK_Logger.hh"
#endif
#ifndef amGHOST_SYSTEM
  #include "amGHOST_System.hh"  //We Don't Need to Include Anything. Because amGHOST_System is Supposed to be a Standalone Include Thing
#endif

/**
PART I: INIT & EXIT
PART II: EK & EVENT SYSTEM Functions  [Main-Loop Functions Too!]
PART III: GPU & Rendering Contexts [OpenGL/VULKAN/OpenGLES/DirectX11/DirectX12/Mantle/Metal]
PART IV: GENERIC STUFFS [eg. windowCreation, Pop-Ups, Time, Directories]
*/


/** 
* -----------------------------------
*        ____________________
*       |PART I: INIT & EXIT|
*       ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
* -----------------------------------
*/

// One and Only amGHOST_System
static amGHOST_System *heart;

void amGHOST_CreateSystem(void) {
  amGHOST_System::create_system();
  heart = amGHOST_System::get_system();
}

amGHOST_SystemHandle amGHOST_GetSystem(void) {
  return (amGHOST_SystemHandle) heart;  // a.k.a amGHOST_System::get_system()
}

void amGHOST_DisposeSystem(void) {
  heart->dispose_system();  //Bcz, we had amGHOST_System::create_system [a.k.a not any CONSTRUCTOR/DESTRUCTOR]
}




/**
* ---------------------------------------------
*     _____________________________________
*    |PART II: EK & Event System Functions|
*    ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
* ---------------------------------------------
* Support for C-API and Mixing Both Capi and CPP kind of EKs in the same amGHOST System
* Also \see amGHOST_EK.hh for EK doc
*/

/**
* in amGHOST_Types.h you will see that there are two Types of amGHOST_EKProcPtr. 2nd one is amGHOST_EKProcPtr_Capi which was made Because
* C doesn't have support for classes but in amGHOST CPP api we wanted to pass (amGHOST_Event*) Class pointer to EK Function parameter which gives us More Direct amGHOST_Event Data access for CPP api usage
* Solution:
* _EK_Workaround_() is a Function which will be used to mimic the Behaviour of a standard amGHSOT_EkProcPtr type of Function
* if user creates multiple C-API EKs, We EXPLICITLY create multiple amGHOST_EventKonsument Objects with that same _EK_Workaround_ function and push_back to Heart's T_EKVec
* You might notice that in amGHOST_System::add_eventKonsument, we didn't allow creation of Multiple EKs with same EKFunc.... and you might ask why is this then breaking the habit?
* I Assure this does not. Because we also have a _EKVec_Capi_ vector here. which contains all the C-API Functions. & before push_backing to Heart's T_EKVec, we do check if that function already exists
*/
static std::vector<amGHOST_EKProcPtr_Capi> _EKProcVec_Capi_ {};
static amGHOST_Event *_last_event_ = nullptr;              //maintained inside _EK_Workaround_
static unsigned short counter_EK_Called = 0;            //Will be Reset if event passed to _EK_Workaround_ != last_event

static int _EK_Workaround_(amGHOST_Event *event) {
  if (event != _last_event_) {
    if (counter_EK_Called != _EKProcVec_Capi_.size()) {
      amVK_LOG("Not all C-Api EKs were Called for the Last amGHOST Supported Event. C-API-EKs Created: " << _EKProcVec_Capi_.size() << " & Called [For lastEvent]: " << counter_EK_Called << "\nNOTE: EKs are Ordered based on WHEN you called amGHOST_AddEventKonsument()");
    }

    _last_event_ = event;
    counter_EK_Called = 0;
  }

  amGHOST_EKProcPtr_Capi EK_Proc = _EKProcVec_Capi_[counter_EK_Called];
  counter_EK_Called += 1;

  return EK_Proc((amGHOST_EventHandle) event);
  return 0;
}

bool amGHOST_AddEventKonsument(amGHOST_EKProcPtr_Capi event_konsument_proc) {
  for (auto EKProc : _EKProcVec_Capi_) {
    if (EKProc == event_konsument_proc) {
      amVK_LOG("Did not create an EK. Already an Event konsument added with that exact same function that you have passed");
      return false;
    }
  }

  _EKProcVec_Capi_.push_back(event_konsument_proc);
  counter_EK_Called += 1;

  amGHOST_EventKonsument *new_EK = new amGHOST_EventKonsument(&_EK_Workaround_);
  heart->add_T_EKVec(new_EK, false);  //Otherwise _EK_Workaround_'s Log message is Printed
  return true;
}

// See T_EKVec in amGHOST.h, in 'Event System Functions' Section
void amGHOST_ProcessEvents(bool waitForEvent) {
  heart->process_events(waitForEvent);
}
bool amGHOST_DispatchEvents(void) {
  bool result = heart->dispatch_events();
  return result;
}

/**
 * \param evnt: Event Handle that usually a EK gets when disposing Events
 * \return: See amGHOST_TEventType, amGHOST_EventData inside amGHOST_Types.h 
 * \todo: add doxygen linking to all the EVENT related types
 */
amGHOST_TEventType amGHOST_GetEventType(amGHOST_EventHandle evnt) {
  amGHOST_Event *event = (amGHOST_Event *) evnt;
  return event->get_type();
}
amGHOST_TEventData amGHOST_GetEventData(amGHOST_EventHandle evnt) {
  amGHOST_Event *event = (amGHOST_Event *) evnt;
  return event->get_data();
}
amGHOST_WindowHandle amGHOST_GetEventWin(amGHOST_EventHandle evnt) {
  amGHOST_Event *event = (amGHOST_Event *) evnt;
  return (amGHOST_WindowHandle) event->get_window();
}






/** 
* ---------------------------------------------
*      ___________________________________
*     |PART III: GPU & Rendering Contexts|
*     ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
* ---------------------------------------------
*/
amGHOST_ContextHandle amGHOST_CreateContextOpenGL(amGHOST_WindowHandle window) {
  amGHOST_Window *W = (amGHOST_Window *)window;
  return (amGHOST_ContextHandle) W->opengl_create_context();
}

void amGHOST_ActivateContextOpenGL(amGHOST_WindowHandle window) {
  amGHOST_Window *W = (amGHOST_Window *)window;
  W->activate_context();
}



/** 
* -----------------------------------
*      ________________________
*     |PART IV: GENERIC STUFFS|
*     ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
* -----------------------------------
*/
amGHOST_WindowHandle amGHOST_CreateWindow(char *title, int posX, int posY, int sizeX, int sizeY) {
  return (amGHOST_WindowHandle) heart->create_window(title, posX, posY, sizeX, sizeY);
}
void amGHOST_ShowWindow(amGHOST_WindowHandle window) {
  amGHOST_Window *W = (amGHOST_Window *)window;
  W->show_window();
}

amGHOST_TSuccess amGHOST_DestroyWindow(amGHOST_WindowHandle window) {
  if(heart->destroy_window((amGHOST_Window *) window)) {
    return amGHOST_kSuccess;
  }

  amVK_LOG("Couldn't Destroy the Window");
  //TODO:- Specify which variable it was stored in, the memory address. maybe the line number and file that the variable was at,
  //TODO:- Dont Just Return, Make sure that no memory is leaked or stays in RAM after program is closed

  return amGHOST_kFailure;
}