#pragma once
#ifndef amGHOST_CONTEXT
#define amGHOST_CONTEXT

#ifndef amGHOST_TYPES
  #include "amGHOST_Types.h"
#endif

//OpenGL/VK For Now
/**
 * HUGELY WIP.... not Completed....
* NOTES: 1. Contexts are bound to windows & only 1 context can be active at a time, that is where all drawing would happen.
*/
class amGHOST_Context
{
 public:
  virtual bool destroyer(void) = 0;
  /**
  * There Could Be Multiple OpenGL Contexts, This is For Switching from One to Another
  * Every window has their very own amGHOST_ContextWGL 
  * Only the Active Rendering Context is where the OpenGL Functions calls are executed, I mean where all the drawing Happens
  */
  virtual bool activate_context(void) = 0;

 protected:
  amGHOST_Context(const amGHOST_Context&) = delete;             //Brendan's Solution
  amGHOST_Context& operator=(const amGHOST_Context&) = delete;  //Brendan's 
 public:
  amGHOST_Context() {}                                          //because of the line below.... there will be no default CONSTRUCTOR
  virtual ~amGHOST_Context() = 0;                               //-Wnon-virtual-dtor
};

#endif //#ifndef amGHOST_CONTEXT