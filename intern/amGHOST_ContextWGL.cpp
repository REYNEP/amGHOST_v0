#include "amGHOST_Logger.hh"
#include "amGHOST_ContextWGL.hh"
#include "amGHOST_SystemWIN32.hh" //windows.h + check if glewLoaded [as glewLoaded is not Static] + variables from amGHOST_SystemWIN32.hh eg.s_wndClassName

#include "GL/glew.h"
#include "GL/wglew.h"

/**
* There Could Be Multiple OpenGL Contexts, This is For Switching from One to Another
* Every window has their very own amGHOST_ContextWGL 
* Only the Active Rendering Context is where the OpenGL Functions calls are executed, I mean where all the drawing Happens
*/
bool amGHOST_ContextWGL::activate_context(void) {
  if (!::wglMakeCurrent(this->m_hdc, this->m_hglrc)) {return false;}
  else {return true;}
}


/**
* Currently This is what Initializes New Contexts. This is the CONSTRUCTOR
*/
amGHOST_ContextWGL::amGHOST_ContextWGL(HWND hwnd, HDC hdc, HGLRC hglrc)
  : m_hwnd(hwnd), 
    m_hdc(hdc), 
    m_hglrc(hglrc)
{
  if (amGHOST_System::heart->glewLoaded == false) {
    this->opengl_load();
  }

  
  /**Implemented by GPU Drivers like Nvidia-Proprietary/Mesa/AMD-Adrenaline, [kh-reg-gl/extensions/ARB/WGL_ARB_pixel_format.txt]
  *  wglChoosePixelFormatARB(HDC hdc,                     //Related Funcs Can be Found in the Above Link and also SPECS
  *                          const int *piAttribIList,    //AttribList Obviously
  *                          const FLOAT *pfAttribFList,  //IF Anyone wants a FloatList instead of Int List
  *                          UINT nMaxFormats,            //Maximum Number of PixelFormats to Return
  *                          int *piFormats,              //Array of returned indices of the matching PixelFormats  [WIN32 Compatible]
  *                          UINT *nNumFormats);          //Number of Matching Formats, [Not Bigger than nMaxFormats] [Just a Regular INT Pointer]]
  *
  * I still think m_PixelFormat is something like what ::ChoosePixelFormat returns
  * CONFIRMED:  11. Should we create all new calls for pixel formats, specifically should
  *                 we introduce SetPixelFormatARB? No, this doesn't offer any value over
  *                 the existing SetPixelFormat call.
  */
  bool x = wglChoosePixelFormatARB(hdc,
    this->m_iPixelFormatAttribList,       //The Name says it!
    NULL,                                 //Should be NULL as we Specified Attributes in Above Param
    1,                                    //We didn't want more than 1 PixelFormat     
    &(this->m_iPixelFormat),              //[int m_PixelFormat = 0;] in the header file
    &(this->m_nFormatsUsable)             //[uint m_nFormatsUsable = 0;] should be '1' after function returns, as we passed 1 as the 4th param
  );
  ::SetPixelFormat(hdc, this->m_iPixelFormat, &(this->m_pfd)); 
  //Third Parameter is just for the record of LOGICAL Specs on PixelFormat. 
  //This Literally has 0 Influence on 'Setting The Pixel Format'. Try Passing NULL, It would still work nothing would break! [ref: MSVC]
  //::SetPixelFormat(hdc, this->m_iPixelFormat, NULL);

  //HGLRC wglCreateContextAttribsARB(HDC hDC,   [kh-reg-gl/extensions/ARB/WGL_ARB_create_context.txt]
  //                                 HGLRC hshareContext,
  //                                 const int *attribList); 
  //If <hshareContext> is not 0, then all shareable data (excluding OpenGL texture objects named 0) will be shared by <hshareContext>.
  this->m_hglrc = wglCreateContextAttribsARB(hdc, 0, this->m_iAttribs);


  ::wglMakeCurrent(this->m_hdc, this->m_hglrc);
  LOG("NEW OpenGL Context Created [WGL]");
  std::cout << glGetString(GL_VERSION) << std::endl;
}



void amGHOST_ContextWGL::opengl_clear(void) {
  glClearColor(.6f, .6f, .6f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  bool swapped = ::SwapBuffers(::GetDC(this->m_hwnd));
  if (!swapped) {
    LOG("::SwapBuffers failure, GetLastError() returns" << std::hex << ::GetLastError());
  }

  LOG("glClearColor and glCLear Called");
}








/**
  * |------------------|
  * - FORBIDDEN STUFFS -
  * |------------------|
*/
PIXELFORMATDESCRIPTOR amGHOST_ContextWGL::m_pfd = {   //Static Member
  sizeof(PIXELFORMATDESCRIPTOR),  //Size of this pfd structure. Should be set to sizeof(PIXELFORMATDESCRIPTOR).
  1,                              //Version of this pfd structure. Should be set to 1.
  PFD_DRAW_TO_WINDOW |            //(Flags for pdf) Enables Directly Drawing to Window 
  PFD_SUPPORT_OPENGL |            //OpenGL Support  [https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-pixelformatdescriptor]
  PFD_DOUBLEBUFFER,               //DoubleBuffer Support
  PFD_TYPE_RGBA,                  //The kind of framebuffer. RGBA or palette [color-index value, PFD_TYPE_COLORINDEX].
  32,                             //Colordepth of the framebuffer.
  0, 0, 0, 0, 0, 0, 0, 0,         //Red Bits, Red Shift, Blue Bits, Blue Sh... Alpha Bits, Alpha Shift [Only works with PDF_TYPE_RGBA] 
  0, 0, 0, 0, 0,        //pdf Accumulation Buffer info [http://what-when-how.com/opengl-programming-guide/the-accumulation-buffer-the-framebuffer-opengl-programming-part-1/]
  24,                   // Number of bits for the depthbuffer
  8,                    // Number of bits for the stencilbuffer
  0,                    // Number of Aux buffers in the framebuffer.
  PFD_MAIN_PLANE,       // NO LONGER USED [Don't Worry about these last 5]
  0,                    // Overlay/Underlay Layers specs, I think The above one should be PFD_OVERLAY_PLANE/PFD_UNDERLAY_PLANE when using this option
  0, 0, 0               // 1st, 3rd one No longer USED, 2nd one specified some kind of mask
};

/**
* The Idea is to Create a Dummy WINDOW and a Dummy CONTEXT and then load glew. Thus we will have all Available OpenGL Functions
* WARNING: Should not be Called More than Once.
*/
bool amGHOST_ContextWGL::opengl_load(void) {
  /*
    - First we Create a Dummy OpenGL Context then call the Function that Extends openGL a.k.a GLEW
  */
  LOG("opengl_load is called");
  bool success = false;


  //----------------Creating a Dummy Window
  PIXELFORMATDESCRIPTOR dummyPFD = amGHOST_ContextWGL::m_pfd;
  amGHOST_SystemWIN32 *amg_sys_win32 = (amGHOST_SystemWIN32 *) amGHOST_System::heart;

  HWND dummyHWND = ::CreateWindowA(
    amg_sys_win32->s_wndClassName,   //We use the window Class that we made for Creating Normal Windows, See amGHOST_SystemWIN32 Constructor and reg_wc function that it calls
    "DUMMY_WND",
    WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
    0, 0,
    64, 64,
    (HWND) NULL,
    (HMENU) NULL,
    amg_sys_win32->s_hInstance,
    (LPVOID) NULL
  );  //We Don't Do a Show Window, So This Window Won't be shown

  HDC dummyHDC = ::GetDC(dummyHWND);
  int dummyPF = ::ChoosePixelFormat(dummyHDC, &dummyPFD);

  ::SetPixelFormat(dummyHDC, dummyPF, &dummyPFD);

  HGLRC dummyHGLRC = ::wglCreateContext(dummyHDC);
  if (::wglMakeCurrent(dummyHDC, dummyHGLRC) == false) {
    LOG_DEV("wglMakeCurrent Failed for dummyHGLRC");
    if (dummyHGLRC == NULL) {LOG("dummyHGLRC is NULL");}
  }



  /*
  * The MAIN PART, Calling glewInit
  * --------------------------------
  * Why We did What we did above
  * - GLEW Needs an created OpenGL Context to Extend openGL, because GLEW Needs to get the GPU/VENDOR Information
  * - GLEW also calls glGetString at the Very Start, Which doesn't really give any info till the current thread's Rendering Context is OpenGL [See wglMakeCurrent]
  * - For extending GLEW Calls glewGetProcAddress which is also mostly platform specific
  */
  if (glewInit() != GLEW_OK) {return false;}
  else {success = true;}
  LOG(glGetString(GL_VERSION));

  //WHY WE PASSING STRINGS? Probably Becasue of So so Many Functions and Macros and Stuffs
  if(wglewIsSupported("WGL_ARB_create_context") == 0) {
    LOG_DEV("CLEARLY IT DIDNT WORK [TALKING about Loading OpenGL (GLEW)], because without WGL_ARB_create_context we can wouldn't be able to Extend any OpenGL Context");
    return false;
  }

  //Clean-UP
  ::wglMakeCurrent(NULL, NULL);
  ::wglDeleteContext(dummyHGLRC);
  ::ReleaseDC(dummyHWND, dummyHDC);
  ::DestroyWindow(dummyHWND);

  if (success == true) {
    LOG("OpenGL Loaded by GLEW....");
    amGHOST_System::heart->glewLoaded = true;
    return true;
  }
  else {
    return false;
  }
}