#define VK_USE_PLATFORM_X11_KHR
#include "amGHOST_SystemX11.hh"
#include "amGHOST_WindowX11.hh"

/**
 * These callbacks can be used for debugging, so we can break-point on an X11 error.
 *
 * Dummy function to get around IO Handler exiting if device invalid
 * Basically it will not crash blender now if you have a X device that
 * is configured but not plugged in.
 */
int amGHOST_X11_ErrorHandler(Display *display, XErrorEvent *event)
{
    char error_code_str[512];

    XGetErrorText(display, event->error_code, error_code_str, sizeof(error_code_str));

    amVK_LOG(
        "Received X11 Error:" <<
        "\terror code: "   << event->error_code   << "\n" <<
        "\trequest code: " << event->request_code << "\n" <<
        "\tminor code: "   << event->minor_code   << "\n" <<
        "\terror text: "   << error_code_str
    );

    /* No exit! - but keep lint happy */
    return 0;
}

int amGHOST_X11_IOErrorHandler(Display * /*display*/)
{
    amVK_LOG("Ignoring Xlib error: error IO\n");

    /* No exit! - but keep lint happy */
    return 0;
}











amGHOST_Window* amGHOST_SystemX11::create_window(const char *title, int posX, int posY,  int sizeX, int sizeY, bool instantShow) {
    amGHOST_WindowX11 *REY = new amGHOST_WindowX11(    title,     posX,     posY,      sizeX,     sizeY);

    if (!REY->XW) {
        REY->destroyer();
        return nullptr;
    }
    else {
        if (instantShow) {REY->show_window();}    // XMapWindow()

        if (T_WindowVec.should_resize()) {T_WindowVec.resize();}
        amVK_ArrayDYN_PUSH_BACK(T_WindowVec) = (amGHOST_Window *) REY;
    }

    return (amGHOST_Window *) REY;
}

bool amGHOST_SystemX11::destroy_window(amGHOST_Window* window) {
    if (window) {
        amGHOST_WindowX11 *window_conv = (amGHOST_WindowX11 *) window;
        window_conv->destroyer();

        for (uint32_t i = 0, n = T_WindowVec.neXt; i < n; i++) {
        if (T_WindowVec.data[i] == window) { T_WindowVec.lazy_erase(i); break; }
        }

        return true;
    }
    return false;
}


bool amGHOST_SystemX11::process_events(bool waitForEvent) {
    XEvent REY;
    XNextEvent(XD, &REY);
    return false;
}









#ifdef amGHOST_BUILD_OPENGL
    #include "amGHOST_ContextGLX11.hh"  //has GLEW.h needed in openGL_clear() + basically all openGL Functions including openGL_create_context()
#endif


amGHOST_Context* amGHOST_WindowX11::opengl_create_context(void) {
#ifdef amGHOST_BUILD_OPENGL

    amVK_LOG_EX("GLX on X11 systems is under construction....");
    return (amGHOST_Context *) nullptr;

#else
    amVK_LOG_EX("[macro] amGHOST_BUILD_OPENGL support was disabled when building");
    return (amGHOST_Context *) nullptr;
#endif
}

void amGHOST_WindowX11::opengl_activate_context(void) {
#ifdef amGHOST_BUILD_OPENGL

    amVK_LOG_EX("GLX on X11 systems is under construction....");
    return;

#else
    amVK_LOG_EX("[macro] amGHOST_BUILD_OPENGL support was disabled when building");
    return;
#endif
}



bool amGHOST_SystemX11::opengl_load() {
#ifdef amGHOST_BUILD_OPENGL

    amVK_LOG_EX("GLX on X11 systems is under construction....");
    return (amGHOST_Context *) nullptr;
  
#else
    amVK_LOG_EX("[macro] amGHOST_BUILD_OPENGL support was disabled when building");
    return false;
#endif
}







/**
 * VULKAN
 * \see #define VK_USE_PLATFORM_X11_KHR at top [LINE 1]
 */
#if defined(amGHOST_BUILD_VULKAN)
VkSurfaceKHR amGHOST_WindowX11::create_vulkan_surface(VkInstance instance) {
    return nullptr;
}
#endif
