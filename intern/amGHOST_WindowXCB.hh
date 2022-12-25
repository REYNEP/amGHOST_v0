#pragma once

#include "amGHOST_SystemXCB.hh"
#include <xcb/xcb_atom.h>
#include <xcb/xcb_icccm.h>
#include <unistd.h>

uint32_t _amGHOST_strlen(const char *STR) {
    uint32_t i = 0;
    while(STR[i] != '\0') 
        i++;
    return i;
}

void _amGHOST_strcpy(const char *FROM, char *TO, uint16_t BYTES) {
    for (int i = 0; i < BYTES; i++)
        TO[i] = FROM[i];
}

int _log_clamp(int x, int low, int high, const char *str_var_x) {
    const char *_smth = "[LIMITING] amGHOST_Window(): ";

    if      (x < low)  {amVK_LOG(_smth << str_var_x << " < " << low);}
    else if (x > high) {amVK_LOG(_smth << str_var_x << " > " << high);}

    return x>low ? (x<high ? x:high):low;
}




class amGHOST_WindowXCB : public amGHOST_Window {
   public:
    xcb_window_t XW = 0;        /** X-Window: default nullptr */

   public:
    amGHOST_WindowXCB(const char *title, int posX, int posY, int sizeX, int sizeY) : amGHOST_Window(title, posX, posY, sizeX, sizeY) {

        /** lets just say, 'amGHOST_WindowXCB *wOne = amGHOST_WindowXCB(":re", 0, 0, 1280, 720);' was called */

        amGHOST_SystemXCB *X = (amGHOST_SystemXCB *) amGHOST_System::heart;


         posX = _log_clamp( posX, -32768, 32767, "posX");
         posY = _log_clamp( posY, -32768, 32767, "posY");
        sizeX = _log_clamp(sizeX, 0, 0xFFFF, "sizeX");
        sizeY = _log_clamp(sizeY, 0, 0xFFFF, "sizeY");
        /** \todo CAP according to DISPLAY/SCREEN/MONITOR/MULTI_MONITOR resolution */


        XW = xcb_generate_id(X->XC);
        uint32_t enabled_events = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_STRUCTURE_NOTIFY;
        xcb_void_cookie_t res = xcb_create_window_checked(
            X->XC,                  /** connection */
            XCB_COPY_FROM_PARENT,   /** depth */
            XW,
            X->XW,                  /** root window */
            posX, posY,
            sizeX, sizeY,
            X->window_border_width,
            XCB_WINDOW_CLASS_INPUT_OUTPUT,
            X->XS->root_visual,
            XCB_CW_EVENT_MASK, &enabled_events
                                    /** xcb_change_window_attributes_checked() */
                                    /** TODO: USE: xcb_change_window_attributes_aux() bcz of the last param of that... and advice on using that... */
        );

        if (amGHOST_xcb_error(xcb_request_check(X->XC, res)) == true) {
            destroyer();
            amVK_LOG_EX("amGHOST_WindowXCB():  xcb_create_window_checked() failed.... couldn't create window");
        }

        this->set_title(title);
        this->_set_wm_class_n_type_();
        this->_set_wm_size_hints_();
        this->_set_wm_hints_n_state_();
        this->_set_pid_n_hostname_();
    }
   ~amGHOST_WindowXCB() {destroyer();}

   public:
    bool destroyer(void) {
        amGHOST_SystemXCB *X = (amGHOST_SystemXCB *) amGHOST_System::heart;
        if (XW) {
            /** Automatically UnMapped before Destroy */
            xcb_destroy_window(X->XC, XW);
            xcb_flush(X->XC);
            XW = 0;
            return true;
        }
        else return false;
    }




    void show_window(void) {
        amGHOST_SystemXCB *X = (amGHOST_SystemXCB *) amGHOST_System::heart;
        xcb_map_window(X->XC, XW);
        xcb_flush(X->XC);
    }
    void hide_window(void) {
        amGHOST_SystemXCB *X = (amGHOST_SystemXCB *) amGHOST_System::heart;
        xcb_unmap_window(X->XC, XW);
        xcb_flush(X->XC);
    }

    /** changes XCB_ATOM_WM_NAME & _NET_WM_NAME */
    void set_title(const char *title) {
        amGHOST_SystemXCB *X = (amGHOST_SystemXCB *) amGHOST_System::heart;

        xcb_change_property(
            X->XC, XCB_PROP_MODE_REPLACE, XW,
            XCB_ATOM_WM_NAME,
            XCB_ATOM_STRING, 8,
            _amGHOST_strlen(title), title
        );

        xcb_change_property(
            X->XC, XCB_PROP_MODE_REPLACE, XW,
            X->EWMH._NET_WM_NAME,
            X->EWMH.UTF8_STRING, 8,
            _amGHOST_strlen(title), title
        );
    }

    /** 
     * NOTE: if you wanna call this one yourself. turn off 'instant_show' option of create_window()...
     *   also dont forget to call 'show_window()' after calling this function
     * 
     * CHANGES: XCB_ATOM_WM_CLASS
     *
     * \param wm_class: e.g. "re\0re", ....FORMAT:("<instance_name>/0<class_name>")
     * 
     * \ref \see
     *  Tronche: https://tronche.com/gui/x/xlib/ICC/client-to-window-manager/wm-class.html https://tronche.com/gui/x/icccm/sec-4.html#WM_CLASS
     *      Map: https://github.com/rust-windowing/glutin/issues/879
     *            [Has to be set before Window is Mapped].... 
     *            spotify doesn't, so BSPC rules doesn't work on official spotify
     *           https://github.com/dasJ/spotifywm & https://community.spotify.com/t5/Desktop-Linux/WM-CLASS-window-property-on-Linux/m-p/1511491/highlight/true#M4227
     * 
     */
    void set_wm_class(const char *wm_class, bool i_know_the_docs = false) {
        amGHOST_SystemXCB *X = (amGHOST_SystemXCB *) amGHOST_System::heart;

        /** ---- CHECKS & LOGS ---- */
        uint32_t z = _amGHOST_strlen(wm_class)+ 1;
        uint32_t x = _amGHOST_strlen(wm_class + z) + z;
        if (x > 128)     {amVK_LOG("set_wm_class(): WM_CLASS is long.... (more than 128 bytes)")}
        else if (x > 64) {_LOG("set_wm_class(): WM_CLASS seems  long.... (more than 64 bytes)");}

        if (!i_know_the_docs)
            if (x > (2*z)) {amVK_LOG("set_wm_class(): should be like 're\\0re'")}


        /** ---- WM_CLASS ---- */
        xcb_change_property(
            X->XC, XCB_PROP_MODE_REPLACE, XW,
            XCB_ATOM_WM_CLASS,
            XCB_ATOM_STRING, 8,
            x, wm_class
        );

        /** ICCCM
        xcb_icccm_set_wm_class(
            X->XC, XW,
            5, "re\0""re"
        );
        */
    }

    







    /**
     * CHANGES:  _NET_WM_WINDOW_TYPE, & XCB_ATOM_WM_CLASS
     * 
     * These are the basic ones... that 
     * 
     * TODO: Add/Allow chaning WM_CLASS on runtime.... and after a decade or so, on other app's windows toooo.... 
     *  REF: https://unix.stackexchange.com/questions/521019/specifying-the-wm-class-of-a-program
     *       https://unix.stackexchange.com/questions/574814/how-to-define-wm-class-for-terminal-commands-in-desktop-files
     *       https://stackoverflow.com/questions/36650865/set-wm-class-with-wnck-xprop-or-something-else
     */
    void _set_wm_class_n_type_(void) {
        amGHOST_SystemXCB *X = (amGHOST_SystemXCB *) amGHOST_System::heart;

        char     exe_path[257];
        uint16_t exe_name_len;
        char *   exe_name;

        {
            /** /proc/self: https://man7.org/linux/man-pages/man5/proc.5.html */
            uint16_t bytes = readlink("/proc/self/exe", exe_path, 256);
            exe_path[bytes] = '\0';

            uint16_t k = 0;
            for (uint16_t i = 0; i < bytes; i++) {
                if (exe_path[i] == '/')
                    k = i+1;
            }

            exe_name_len = bytes - k;
            exe_name     = exe_path + k;
        }

        /**
         * WM_CLASS = <instance_name>\0<class_name>\0   'that last \0 is for C/C++ memory safety as you know'
         */
        char *wm_class = new char[(exe_name_len + 1) * 2];

        wm_class[exe_name_len] = '\0';
        wm_class[exe_name_len*2 + 1] = '\0';

        _amGHOST_strcpy(exe_name, wm_class,                    exe_name_len);
        _amGHOST_strcpy(exe_name, wm_class + exe_name_len + 1, exe_name_len);

        
        /** ---- WM_CLASS ---- */
        set_wm_class(wm_class);
        //delete wm_class;


        /** ---- WM_TYPES ---- */
        xcb_change_property(
            X->XC, XCB_PROP_MODE_REPLACE, XW,
            X->EWMH._NET_WM_WINDOW_TYPE,
            XCB_ATOM_ATOM, 32,
            1, &X->EWMH._NET_WM_WINDOW_TYPE_NORMAL
        );
    }

    void _set_wm_hints_n_state_(void) {
        amGHOST_SystemXCB *X = (amGHOST_SystemXCB *) amGHOST_System::heart;

        /** ---- WM_HINTS ---- */

        xcb_icccm_wm_hints_t _wm_hints_ = {
            XCB_ICCCM_WM_HINT_INPUT |
            XCB_ICCCM_WM_HINT_STATE,
            true, XCB_ICCCM_WM_STATE_NORMAL,
        };

        xcb_icccm_set_wm_hints(
            X->XC, XW,
            &_wm_hints_
        );

        /** 
        xcb_ewmh_request_change_wm_state(
            &X->EWMH, X->SC, XW,
            XCB_EWMH_WM_STATE_ADD,
            X->EWMH._NET_WM_STATE_MODAL,
            X->EWMH._NET_WM_STATE_ABOVE,
            XCB_EWMH_CLIENT_SOURCE_TYPE_NORMAL
        );
        */

        xcb_atom_t _state_[1] = {
            X->EWMH._NET_WM_STATE_MODAL
        };
        xcb_ewmh_set_wm_state(
            &X->EWMH, XW,
            1, _state_
        );
    }

    void _set_wm_size_hints_(void) {
        int32_t minX = 1000, minY = 500, maxX = 65535, maxY = 65535;

        xcb_size_hints_t _normal_size_ = {
            XCB_ICCCM_SIZE_HINT_P_POSITION | 
            XCB_ICCCM_SIZE_HINT_P_SIZE     | 
            XCB_ICCCM_SIZE_HINT_P_MIN_SIZE | 
            XCB_ICCCM_SIZE_HINT_P_MAX_SIZE,

            m_posX, m_posY, 
            m_sizeX, m_sizeY,
            minX, minY,
            maxX, maxY,

            /** resize increments, min/max aspect ratios, base size, window gravity */
        };

        amGHOST_SystemXCB *X = (amGHOST_SystemXCB *) amGHOST_System::heart;
        xcb_icccm_set_wm_size_hints(
            X->XC, XW, 
            XCB_ATOM_WM_NORMAL_HINTS,
            &_normal_size_
        );
    }

    void _set_pid_n_hostname_(void) {
        amGHOST_SystemXCB *X = (amGHOST_SystemXCB *) amGHOST_System::heart;

        pid_t pid = getpid();

        xcb_change_property(
            X->XC, XCB_PROP_MODE_APPEND, XW,
            X->EWMH._NET_WM_PID,
            XCB_ATOM_CARDINAL, 32,
            1, &pid
        );


        char hostname[64];
        for (int i = 0; i < 8; i++)
            ((uint64_t *)hostname)[i] = 0;      // '\0' everybyte

        gethostname(hostname, sizeof(hostname));

        xcb_icccm_set_wm_client_machine(
            X->XC, XW,
            XCB_ATOM_STRING, 8,
            _amGHOST_strlen(hostname), hostname
        );
    }

















    amGHOST_Context* opengl_create_context(void)    {amVK_LOG("WIP OPENGL"); return nullptr;}
    void             opengl_activate_context(void)  {amVK_LOG("WIP OPENGL");}

#if defined(amGHOST_BUILD_VULKAN)
    /** IMPL: in inside amGHOST_SystemXCB.cpp */
    VkSurfaceKHR create_vulkan_surface(VkInstance instance);
#endif
};