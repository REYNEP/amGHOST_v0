#define VK_USE_PLATFORM_WIN32_KHR
#define _amGHOST_INC_VULKAN_
#include "amVK_Logger.hh"
#include "amGHOST_WindowWIN32.hh"
#include "amGHOST_SystemWIN32.hh" //includes windows.h




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