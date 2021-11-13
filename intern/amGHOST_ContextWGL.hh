#define WIN32_LEAN_AND_MEAN 1
#include <windows.h> //Need: <sdkddkver.h>, <winresrc.h>, "WinDef.h" [HWND m_hwnd; HDC m_hdc; HGLRC m_hglrc] "wingdi.h" [PIXELFORMATDESCRIPTOR m_pfd]

//[kh-reg-gl] Mentioned in this file is equal to https://www.khronos.org/registry/OpenGL/
//GL Commons from glew.h
#define GL_ZERO 0
#define GL_FALSE 0
#define GL_TRUE 1
#define GL_ONE 1

//WGL_ARB_create_context
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB 0x2093
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define ERROR_INVALID_VERSION_ARB 0x2095
#define ERROR_INVALID_PROFILE_ARB 0x2096

//WGL_ARB_pixel_format
#define WGL_NUMBER_PIXEL_FORMATS_ARB 0x2000
#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_DRAW_TO_BITMAP_ARB 0x2002
#define WGL_ACCELERATION_ARB 0x2003
#define WGL_NEED_PALETTE_ARB 0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB 0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB 0x2006
#define WGL_SWAP_METHOD_ARB 0x2007
#define WGL_NUMBER_OVERLAYS_ARB 0x2008
#define WGL_NUMBER_UNDERLAYS_ARB 0x2009
#define WGL_TRANSPARENT_ARB 0x200A
#define WGL_SHARE_DEPTH_ARB 0x200C
#define WGL_SHARE_STENCIL_ARB 0x200D
#define WGL_SHARE_ACCUM_ARB 0x200E
#define WGL_SUPPORT_GDI_ARB 0x200F
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_STEREO_ARB 0x2012
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_RED_BITS_ARB 0x2015
#define WGL_RED_SHIFT_ARB 0x2016
#define WGL_GREEN_BITS_ARB 0x2017
#define WGL_GREEN_SHIFT_ARB 0x2018
#define WGL_BLUE_BITS_ARB 0x2019
#define WGL_BLUE_SHIFT_ARB 0x201A
#define WGL_ALPHA_BITS_ARB 0x201B
#define WGL_ALPHA_SHIFT_ARB 0x201C
#define WGL_ACCUM_BITS_ARB 0x201D
#define WGL_ACCUM_RED_BITS_ARB 0x201E
#define WGL_ACCUM_GREEN_BITS_ARB 0x201F
#define WGL_ACCUM_BLUE_BITS_ARB 0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB 0x2021
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_STENCIL_BITS_ARB 0x2023
#define WGL_AUX_BUFFERS_ARB 0x2024
#define WGL_NO_ACCELERATION_ARB 0x2025
#define WGL_GENERIC_ACCELERATION_ARB 0x2026
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_SWAP_EXCHANGE_ARB 0x2028
#define WGL_SWAP_COPY_ARB 0x2029
#define WGL_SWAP_UNDEFINED_ARB 0x202A
#define WGL_TYPE_RGBA_ARB 0x202B
#define WGL_TYPE_COLORINDEX_ARB 0x202C
#define WGL_TRANSPARENT_RED_VALUE_ARB 0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB 0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B

//WGL_ARB_create_context_profile
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126


#ifndef amGHOST_CONTEXT
  #include "amGHOST_Context.hh"
#endif


class amGHOST_ContextWGL : public amGHOST_Context
{
 public:
  /**
  * Currently This is what Initializes New Contexts. This is the CONSTRUCTOR
  */
  amGHOST_ContextWGL(HWND hwnd, HDC hdc, HGLRC hglrc);

  bool destroyer(void) {return true;}
  /**
  * DESTRUCTOR
  */
  ~amGHOST_ContextWGL() {}

  /**
  * NOTE: THIS SHOULD BE INSIDE THIS CLASS, Because Otherwise if it was in it's Parent Module or Perhaps amGHOST_Window or amGHOST_WindowWIN32
  *       Then that would force us to always have glew Functions Linked and glew.h loaded in our Builds
  *       But we Want flexibility to Choose Only One, Like only Vulkan or ONLY OpenGL. or Even BOTH at the Same time for the Sake Of Both Runtime Availability
  *       The Only thing that we Can do is to Have a Virtual function for this in amGHOST_Context and then Implement Dummy functions containing Message like "This is a VK Context Associated with the Current Window. This is not a OpenGL Context"
  * Things that we Need to DO Once [a.k.a Creating a Dummy Window and OpenGL Context for the Sake of Loading GLEW]
  * Then we Can do what is in the CONSTRUCTOR untill GLEW is Unloaded
  * Currently Static because amGHOST_SystemWIN32.cpp uses this without creating any Object of this class, SO We made this static
  */
  static bool opengl_load(void);
  /**
  * There Could Be Multiple OpenGL Contexts, This is For Switching from One to Another
  * Every window has their very own amGHOST_ContextWGL 
  * Only the Active Rendering Context is where the OpenGL Functions calls are executed, I mean where all the drawing Happens
  */
  bool activate_context(void);
  void opengl_clear(void);

 private:
  HWND m_hwnd;
  HDC m_hdc;
  HGLRC m_hglrc;

  //[WGL_ARB_pixel_format] for the Full List
  //This is used in wglChoosePixelFormatARB(hdc, m_PixelFormatAttribList...); [inside CONSTRUCTOR]. That requires it to be (const int *)
  //Basically All Member Functions and Variables Throughout amGHOST are snakeCase, but I wanted these variables to look like how people names these, to keep up with the WORLD-WIDE STANDARD
  const int m_iPixelFormatAttribList[15] = {
    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
    WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 24,
    WGL_STENCIL_BITS_ARB, 8,
    0 // End of attributes list
  };

  //Other APIs Might Refer this as iPixelFormat and the above one iPixelFormatAttribs / iPixelFormatAttribList and the below one iAttribs / iAttribList / iAttributes
  int m_iPixelFormat = 0;      //[WIN32 Compatible] Would have been an Array if we didn't pass '1' at 4thParam in wglChoosePixelFormatARB() [inside CONSTRUCTOR]
  UINT m_nFormatsUsable = 0;  //Number of Usable Formats returned from wglChoosePixelFormatARB() [inside CONSTRUCTOR]

  //[WGL_ARB_create_context] for Full List
  int m_iAttribs[9] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3, 
    WGL_CONTEXT_MINOR_VERSION_ARB, 2, 
    WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0 // End of attributes list
  };

  static PIXELFORMATDESCRIPTOR m_pfd; //For The Sake of opengl_init
};