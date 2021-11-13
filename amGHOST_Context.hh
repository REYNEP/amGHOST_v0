#ifndef amGHOST_CONTEXT
#define amGHOST_CONTEXT

//OpenGL/VK For Now
/**
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
};

#endif //#ifndef amGHOST_CONTEXT