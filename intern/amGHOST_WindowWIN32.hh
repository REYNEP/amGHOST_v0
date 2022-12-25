#include "amGHOST_SystemWIN32.hh"
#ifndef amGHOST_WINDOW
  #include "amGHOST_Window.hh"
#endif
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>  //Need: <sdkddkver.h>, <winresrc.h>, "WinDef.h" [HWND m_hwnd; HDC m_hdc; HGLRC m_hglrc]

#ifndef amGHOST_CONTEXT
  #include "amGHOST_Context.hh"
#endif

class amGHOST_WindowWIN32 : public amGHOST_Window
{
   public:
    HWND m_hwnd = NULL;

   public:
    amGHOST_WindowWIN32(const char *title, int posX, int posY, int sizeX, int sizeY) : amGHOST_Window(title, posX, posY, sizeX, sizeY) {
        amGHOST_SystemWIN32 *s_win32 = (amGHOST_SystemWIN32 *) amGHOST_System::heart;
        HWND hwnd = ::CreateWindowA(
            s_win32->s_wndClassName,      /** Name of the WNDCLASS [see protected vars of amGHOST_SystemWIN32] */
            title,                        /** Title at the Top bar of the Window */
            WS_OVERLAPPEDWINDOW,          /** Style of the window, all styles: https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles */
            posX, posY,                   /** Horizontal, Vertical Position of window (top-left corner) */
            sizeX, sizeY,                 /** NULL makes the CMD as the parent of this Window, for GUI apps, getDesktopWindow would return the entire Screen as Window */
            (HWND)  nullptr,              /** parent window */
            (HMENU) nullptr,              /** The Handle to the Menu, remember that wndcls.lpszMenuName should have the menu Name TOO, or it wont work */
            s_win32->s_hInstance,         /** Handle to the Instance the window is gonna get be Linked with.... I still don't why both wndclass and hInstance need this */
            (LPVOID) nullptr              /** using this value as lParam of WndProc, CreatwWindowA Send WM_CREATE message to WndProc */
        );

        if (hwnd == nullptr) {
            amVK_LOG_EX("[create_window FAILED]" << " WIN32 CreateWindowA Failed with exit code:- " << GetLastError() << "[HEX:- " << GetLastError() << "]");
            return;
        } 
        else this->m_hwnd = hwnd;
    }

    bool destroyer(void)    { return ::DestroyWindow(this->m_hwnd);}  //Sends WM_DESTROY Message and Destroyes the Window
    ~amGHOST_WindowWIN32()  {};





   public:
    void show_window(void) {
        ::ShowWindow(this->m_hwnd, SW_SHOW);
        ::SetFocus(this->m_hwnd);
    }
    void hide_window(void) {
        ::ShowWindow(this->m_hwnd, SW_HIDE);
    }






    /** OPENGL SECTION [WGL] */
    amGHOST_Context* opengl_create_context(void);
    void           opengl_activate_context(void) {
        if (this->m_render_context != nullptr)
            this->m_render_context->activate_context();

        else {
            amVK_LOG_EX("No Rendering Context Was Created for This Window. Please Create one.. See amGHOST_Window.hh for Rendering Context Options lke Vulkan/Opengl");
        }
    }

    /** VULKAN SECTION */
#if defined(amGHOST_BUILD_VULKAN)
    VkSurfaceKHR create_vulkan_surface(VkInstance instance);
#endif
};