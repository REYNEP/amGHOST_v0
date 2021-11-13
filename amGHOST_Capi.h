#define amGHOST_CAPI

#ifndef __cplusplus 
  #include <stdbool.h>
#endif

#ifndef amGHOST_TYPES
  #include "amGHOST_Types.h"
#endif


/**
PART I: INIT & EXIT
PART II: EK & EVENT SYSTEM Functions  [Main-Loop Functions Too!]
PART III: GPU & Rendering Contexts [OpenGL/VULKAN/OpenGLES/DirectX11/DirectX12/Mantle/Metal]
PART IV: GENERIC STUFFS [eg. windowCreation, Pop-Ups, Time, Directories]
*/
#ifdef __cplusplus
extern "C" {
#endif


/** 
* -----------------------------------
*        ____________________
*       |PART I: INIT & EXIT|
*       ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
* -----------------------------------
*/

/**
  Creates a new amGHOST System, Should be Created Only Once [Bcz, one single amGHOST System convers one whole OS platform]
*/
void amGHOST_CreateSystem(void);

/**
  Why need this function? Bcz the function above doesn't return the System. As there can be only 1 Sys at a time, there ain't any point in returning a handle or smthng
  But we Have this func, cause it returns the amg_sys PTR saved in the amGHOST_C-api [The Heart Variable], not the one inside amGHOST_System
  \return the Existing amGHOST System [NULL if it doesn't]. 
*/
amGHOST_SystemHandle amGHOST_GetSystem(void);

/**
  Destroyes the Created system
*/
void amGHOST_DisposeSystem(void);




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
  WHY DID WE CHOOSE THE NAME EVENTKONSUMENT? If you really wanna find out then check out amGHOST_EK.h
  \param event_konsument_proc: this parameter is of type "amGHOST_EKProcPtr", a type which is declared in amGHOST_types.h.... also note that EK means EventKonsument
*/
bool amGHOST_AddEventKonsument(amGHOST_EKProcPtr_Capi event_konsument_proc);

/**
  Processes the Platform Specific Events, and Turns them info amGHOST ones, So that you can GET the events or DISPOSE Events to your EventKonsuments
*/
void amGHOST_ProcessEvents(bool waitForEvent);
/**
  DISPOSE Events to your EventKonsuments
*/
bool amGHOST_DispatchEvents(void);

/**
  amGHOST_GetEvent, amGHOST_GetEvents
*/

/**
  \return EventType \see amGHOST_Types
  \todo Explain EventType and EventData Combinations
  \param evnt: The EventHandle, Usually you get the EventHandles from DisposeEvents and GetEvent/s Functions
*/
amGHOST_TEventType amGHOST_GetEventType(amGHOST_EventHandle evnt);
/**
  \return EventData \see amGHOST_Types
*/
amGHOST_TEventData amGHOST_GetEventData(amGHOST_EventHandle evnt);
/**
  \return the Window that the Event was Called to
*/
amGHOST_WindowHandle amGHOST_GetEventWin(amGHOST_EventHandle evnt);




/** 
* ---------------------------------------------
*      ___________________________________
*     |PART III: GPU & Rendering Contexts|
*     ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
* ---------------------------------------------
*/
/**
  FOR EVERY WINDOW, You have to create the OpenGL Context before you can do any kind of OpenGL Drawing or
  ---- [Context could be also said 'Rendering Context' More Specifically, Cause that's All that this is about] ----
  You can use the ActivateRenderingContext to Activate another already created Context
*/
amGHOST_ContextHandle amGHOST_CreateContextOpenGL(amGHOST_WindowHandle window);
void amGHOST_ActivateContextOpenGL(amGHOST_ContextHandle context);




/** 
* -----------------------------------
*      ________________________
*     |PART IV: GENERIC STUFFS|
*     ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
* -----------------------------------
*/

/**
  Creates a New Window obviously
  \param title: should be ASCII characters only (as its char*)
  \param posX: =_= Position in X-axis
  \param posY: +_+ Position in Y-axis
  \param sizeX: WIDTH
  \param sizeY: HEIGHT
*/
amGHOST_WindowHandle amGHOST_CreateWindow(
  char *title,
  int posX, 
  int posY, 
  int sizeX, 
  int sizeY
);
void amGHOST_ShowWindow(amGHOST_WindowHandle window);
/** 
  Opposite of amGHOST_CreateWindow, Windows and Linux suggests people to do this before exiting program
*/
amGHOST_TSuccess amGHOST_DestroyWindow(amGHOST_WindowHandle window);

#ifdef __cplusplus
}
#endif