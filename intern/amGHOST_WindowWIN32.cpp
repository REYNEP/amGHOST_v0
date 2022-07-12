#define VK_USE_PLATFORM_X11_KHR
#include "amVK_Logger.hh"
#include "amGHOST_WindowWIN32.hh"
#include "amGHOST_SystemWIN32.hh" //includes windows.h

amGHOST_WindowWIN32::amGHOST_WindowWIN32(const char *title, int posX, int posY, int sizeX, int sizeY) 
  : amGHOST_Window(title, posX, posY, sizeX, sizeY)
{
  amGHOST_SystemWIN32 *s_win32 = (amGHOST_SystemWIN32 *) amGHOST_System::heart;
  HWND hwnd = ::CreateWindowA(
    s_win32->s_wndClassName,      /** Name of the WNDCLASS [see protected vars of amGHOST_SystemWIN32] */
    title,                        /** Title at the Top bar of the Window */
    WS_OVERLAPPEDWINDOW,          /** Style of the window, all styles: https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles */
     posX, posY,                  /** Horizontal, Vertical Position of window (top-left corner) */
    sizeX, sizeY,                 /** NULL makes the CMD as the parent of this Window, for GUI apps, getDesktopWindow would return the entire Screen as Window */
    (HMENU) nullptr,              /** The Handle to the Menu, remember that wndcls.lpszMenuName should have the menu Name TOO, or it wont work */
    s_win32->s_hInstance,         /** Handle to the Instance the window is gonna get be Linked with.... I still don't why both wndclass and hInstance need this */
    (LPVOID) nullptr              /** using this value as lParam of WndProc, CreatwWindowA Send WM_CREATE message to WndProc */
  );

  if (hwnd == nullptr) {
    amVK_LOG_EX("[create_window FAILED]" << " WIN32 CreateWindowA Failed with exit code:- " << GetLastError() << "[HEX:- " << GetLastError() << "]");
    return;
  } 
  else {
    this->m_hwnd = hwnd;
  }
}

bool amGHOST_WindowWIN32::destroyer() {
  return ::DestroyWindow(this->m_hwnd);  //Sends WM_DESTROY Message and Destroyes the Window
}

void amGHOST_WindowWIN32::show_window(void) {
  ::ShowWindow(this->m_hwnd, SW_SHOW);
  ::SetFocus(this->m_hwnd);
}
void amGHOST_WindowWIN32::hide_window(void) {
  ::ShowWindow(this->m_hwnd, SW_HIDE);
}




/***********************************************************
----------- OpenGL Creation Functios Below [WGL] -----------
***********************************************************/
#ifdef amGHOST_BUILD_OPENGL
  #include "amGHOST_ContextWGL.hh"  //has GLEW.h needed in openGL_clear() + basically all openGL Functions including openGL_create_context()
#endif

amGHOST_Context* amGHOST_WindowWIN32::opengl_create_context(void) {
#ifdef amGHOST_BUILD_OPENGL

  HWND hwnd = this->m_hwnd;
  HDC hdc = ::GetDC(hwnd);
  HGLRC hglrc = nullptr;
  if (hwnd == nullptr || hdc == nullptr) {return nullptr;}
  
  amGHOST_ContextWGL *C = new amGHOST_ContextWGL(hwnd, hdc, hglrc);
  this->m_render_context = (amGHOST_Context *) C;
  C->opengl_clear();
  return (amGHOST_Context*) C;

#else
  amVK_LOG_EX("[macro] amGHOST_BUILD_OPENGL support was disabled when building");
  return (amGHOST_Context *) nullptr;
#endif  //amGHOST_OPENGL
}


/*****************************************************
----------- VULKAN Creation Function Below -----------
*****************************************************/
#if defined(amGHOST_BUILD_VULKAN)

/** 
 * \see #define VK_USE_PLATFORM_WIN32_KHR at top [LINE 1]
 */

VkSurfaceKHR amGHOST_WindowWIN32::create_vulkan_surface(VkInstance instance) {
  //--------- Well, this turned Out to be SHORT, Have Fun ---------
  VkWin32SurfaceCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  createInfo.hwnd = this->m_hwnd;
  createInfo.hinstance = ((amGHOST_SystemWIN32 *)(amGHOST_System::heart))->s_hInstance;
  //Don't Need CreateDevice to do this obviously, Thus Not everything in vulkan_load is not needed to actually create a Vulkan Window Context or rather VK-SURFACE 
  VkSurfaceKHR surface;
  VkResult res = vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface);

  if (res != VK_SUCCESS) {amVK_LOG_EX(res << "Thats the output that vkCreateWin32SurfaceKHR() gave me...."); return nullptr;}
  return surface;
}

#endif  //amGHOST_BUILD_VULKAN


/**********************************************
--------- RenderContext Related Funcs ---------
**********************************************/
void amGHOST_WindowWIN32::activate_context() {
  if (this->m_render_context != nullptr) {
    this->m_render_context->activate_context();
  }
  else {
    amVK_LOG_EX("No Rendering Context Was Created for This Window. Please Create one.. See amGHOST_Window.hh for Rendering Context Options lke Vulkan/Opengl");
  }
}