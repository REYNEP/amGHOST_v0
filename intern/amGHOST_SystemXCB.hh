#pragma once

#include "amGHOST_System.hh"
#include <xcb/xcb.h>
#include <xcb/xcb_errors.h>     /** libxcb-errors */
#include <xcb/xcb_ewmh.h>       /** libxcb-ewmh   */

/**  also requires amGHOST_DEV (option) to be ON  */
#define amGHOST_ATOM_INSPECT 0

bool amGHOST_xcb_error(xcb_generic_error_t *RES);
const char *amGHOST_xcb_conn_error(int RES);


class amGHOST_SystemXCB : public amGHOST_System
{
   public:
    amGHOST_SystemXCB(void) {
        XC = xcb_connect(NULL, &SC);    /** ---- DISPLAY env is used, cz param NULL ---- */

        if (_XC_ERRATUM_()) { amVK_LOG_EX("xcb_connect() error, couldn't create a connection to xcb.");
             xcb_disconnect(XC); 
             XC = nullptr;
        }

        XS = xcb_setup_roots_iterator(xcb_get_setup(XC)).data;
        XW = XS->root;


        /** ----- XE ----- */
        if (xcb_errors_context_new(XC, &XE) == -1)
            _XE_ERRATUM_();

        /** ---- EWMH ---- */
            _EWMH_INIT_N_ERRATUM_();
        
    }
   ~amGHOST_SystemXCB(void) {
        if (EWMH.screens != nullptr) xcb_ewmh_connection_wipe(&EWMH);
        if (XE != nullptr) xcb_errors_context_free(XE);
        if (XC != nullptr) xcb_disconnect(XC);
    }
    
   private:
    /** \return true if ERROR [an amVK_LOG is printed, you should print your own amVK_LOG_EX afterwards....]*/
    bool _XC_ERRATUM_(bool print_err = true) {
        int res = xcb_connection_has_error(XC);
        if (res != 0) {
            if (print_err) {amVK_LOG("XCB_CONNECTION error:- \n    - " << amGHOST_xcb_conn_error(res));}
            return true;
        }
        else return false;
    }

    void _XE_ERRATUM_(void) {
        amVK_LOG_EX("amGHOST_SystemXCB() error:- \n" <<
                 "  xcb_errors_context_new() failed, maybe cz of malloc problems...." << 
                 "  or bcz xcb_connection ended up in an error state.");
    }

    /** 
     * TODO: if ERROR Happens... use get_atom() to get the _NET_WM_NAME like ATOMS... 
     */
    void _EWMH_INIT_N_ERRATUM_(void) {
        xcb_generic_error_t *err = nullptr;

        if (xcb_ewmh_init_atoms_replies(&EWMH, xcb_ewmh_init_atoms(XC, &EWMH), &err) == 0) {
            amVK_LOG_EX("Can't initialize EWMH atoms.\n");

            /** TODO: should we do smth like call \fn xcb_ewmh_connection_wipe() ??? ask the XCB devs....*/
        }

        amGHOST_xcb_error(err);
    }











   public:
    xcb_connection_t *XC = nullptr;         /** https://github.com/freedesktop/xcb-libxcb/blob/master/src/xcbint.h#L195   [ 5b40681 on Jun 12, 2015 ] */
    xcb_window_t      XW = 0;               /** ROOT WINDOW... a.k.a the monitor/display.... not change/swappable.... so its like a universal constant*/
    xcb_screen_t     *XS = nullptr;         /** ROOT SCREEN */
    int SC = 0;                             /** Default Screen [Don't know much about these screens....]*/

    xcb_errors_context_t   *XE = nullptr;   /** https://gitlab.freedesktop.org/xorg/lib/libxcb-errors */
    xcb_ewmh_connection_t EWMH = {};        /** Extended Window Manager Hints - see get_atom() docs inside amGHOST_SystemXCB.cpp */

    /** U CN MODIFY These. */
   public:
    uint16_t window_border_width = 0;       /** changable... \todo how, i forgot 😅 */
    char *wm_class = nullptr;               /** \see amGHOST_WindowXCB::set_wm_class() */









    /** 
     * Prints all the   xcb_screen_t   info
     * \see IMPL:
     */
    void print_screen_info(void);

    /**
     * SEE DOCS at IMPL: INSIDE amGHOST_SystemXCB.cpp
     * \param atom_name: e.g. "_NET_WM_NAME" / "WM_NAME" / "UTF8_STRING" / "_MOTIF_WM_HINTS" 
     * \return xcb_atom_t: XCB_ATOM_NONE if not found....
     */
    xcb_atom_t get_atom(const char *atom_name);

    /**
     * PureVirtual amGHOST Stuffs....
     */
    amGHOST_Window* create_window(const char *title, int posX, int posY,  int sizeX, int sizeY, bool instantShow = true);
    bool destroy_window(amGHOST_Window* window);
    bool process_events(bool waitForEvent);
    bool opengl_load(void);

    /**
     * CALLED by process_events
     */
    bool _process_event(xcb_generic_event_t *EVE);
    amGHOST_Window *get_window(xcb_window_t XW);

    /**
     * \return true if Properly handled amGHOST_kLostSystemXCB, e.g. XCB Connection is restored
     */
    bool handled_LostSystem(void);

    bool _sent_xcb_lost_event_ = false;
    /** sends amGHOST_kLostSystemXCB to all windows */
    void _send_xcb_lost_event_(void);
};