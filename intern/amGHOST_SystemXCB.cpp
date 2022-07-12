#include "amGHOST_SystemXCB.hh"
#include "amGHOST_WindowXCB.hh"

/** 
 * _NET_WM_NAME:     a member var    of    xcb_ewmh_connection_t
 *      WM_NAME: XCB_ATOM_WM_NAME    of    xcb_atom_enum_t
 *  UTF8_STRING:     a member var    of    xcb_ewmh_connection_t
 * 
 * 
 * DOCS: 
 *  https://github.com/bakkeby/dusk/wiki/EWMH
 * 
 *  https://specifications.freedesktop.org/wm-spec/1.3/ar01s05.html
 *      ALT: https://specifications.freedesktop.org/wm-spec/wm-spec-1.3.html
 *  https://github.com/parkouss/pyewmh/blob/master/ewmh/ewmh.py
 *      ALT: https://ewmh.readthedocs.io/en/latest/ewmh.html
 *  https://en.wikipedia.org/wiki/Extended_Window_Manager_Hints
 * 
 * 
 * \var EWMH._NET_WM_NAME = get_atom("_NET_WM_NAME") = 322
 * \var EWMH.UTF8_STRING  = get_atom("UTF8_STRING")  = 344
 * \enum XCB_ATOM_WM_NAME = get_atom("WM_NAME")      = 39
 * 
 * EWMH = amGHOST_SystemXCB::EWMH
 */

#include "string.h"
xcb_atom_t amGHOST_SystemXCB::get_atom(const char *atom_name) {
    xcb_generic_error_t *RES = nullptr;
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(XC, xcb_intern_atom(XC, true, strlen(atom_name), atom_name), &RES);

    if (  RES != nullptr) amGHOST_xcb_error(RES);
    if (reply == nullptr) return XCB_ATOM_NONE;
    
    /** ---- There will always be a Reply ---- */
    xcb_atom_t atom = reply->atom;

    if (atom == XCB_ATOM_NONE) {
        _LOG("atom '" << atom_name << "' not found");
    }

    free(reply);
    return atom;
}















const char *amGHOST_xcb_conn_error(int RES) {
    switch(RES)
    {
        case(XCB_CONN_ERROR):
            return "xcb connection errors because of socket, pipe and other stream errors.";
        case(XCB_CONN_CLOSED_EXT_NOTSUPPORTED):
            return "xcb connection shutdown because of extension not supported";
        case(XCB_CONN_CLOSED_MEM_INSUFFICIENT):
            return "malloc(), calloc() and realloc() error upon failure, for eg ENOMEM";


        case(XCB_CONN_CLOSED_REQ_LEN_EXCEED):
            return "Connection closed, exceeding request length that server accepts.";
        case(XCB_CONN_CLOSED_PARSE_ERR):
            return "Connection closed, error during parsing display string.";


        case(XCB_CONN_CLOSED_INVALID_SCREEN):
            return "Connection closed because the server does not have a screen matching the display.";
        case(XCB_CONN_CLOSED_FDPASSING_FAILED):
            return "Connection closed because some FD passing operation failed";
        default:
            return "XCB Connect worked!";
    }
}

bool amGHOST_xcb_error(xcb_generic_error_t *RES) {
    if (RES) {
        amGHOST_SystemXCB *X = (amGHOST_SystemXCB *) amGHOST_System::heart;

        amVK_LOG_EX(
            "\n" <<
            "[XCB ERROR] \n" <<
            "  response_type : " << xcb_errors_get_name_for_core_event(X->XE, RES->response_type, nullptr) << "\n" <<
            "        uint8_t : " << RES->response_type << "\n" <<
            "     error_code : " << xcb_errors_get_name_for_error(X->XE, RES->error_code, nullptr) << "\n" <<
            "        uint8_t : " << RES->error_code << "\n" <<
            "       sequence : " << RES->sequence << "\n" <<
            "    resource_id : " << RES->resource_id << "\n" <<
            "\n" <<
            "     minor_code : " << xcb_errors_get_name_for_minor_code(X->XE, RES->major_code, RES->minor_code) << "\n" <<
            "       uint16_t : " << RES->minor_code << "\n" <<
            "     major_code : " << xcb_errors_get_name_for_major_code(X->XE, RES->major_code) << "\n" <<
            "        uint8_t : " << RES->major_code << "\n" <<
            "  pad0  uint8_t : " << RES->pad0 << "\n" <<
            "pad[5] uint32_t : " << RES->pad[0] << "\n" <<
            "  full_sequence : " << RES->full_sequence<< "\n" <<
            "\n"
        );
        return true;
    }
    else return false;
}

void amGHOST_SystemXCB::print_screen_info(void) {
    /** ---- SCREEN DATA ----*/
    xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(XC));

    while(screen_iter.rem) {
        xcb_screen_t *SD = screen_iter.data;

        amVK_LOG(
            "\n" <<
            "[XCB INFO] Screen " << SD->root << "\n" <<
            "     width      : " << SD->width_in_pixels << "\n" <<
            "    height      : " << SD->height_in_pixels << "\n" <<
            "     width (mm) : " << SD->width_in_millimeters << "\n" <<
            "    height (mm) : " << SD->height_in_millimeters << "\n" <<
            "    black_pixel : " << SD->black_pixel << "\n" <<
            "    white_pixel : " << SD->white_pixel << "\n" <<
            "\n" <<
            "   current_input_masks : " << SD->current_input_masks << "\n" <<
            "    min_installed_maps : " << SD->min_installed_maps << "\n" <<
            "    max_installed_maps : " << SD->max_installed_maps << "\n" <<
            "        backing_stores : " << SD->backing_stores << "\n" <<
            "           save_unders : " << SD->save_unders << "\n" <<
            "            root_depth : " << SD->root_depth << "\n" <<
            "    allowed_depths_len : " << SD->allowed_depths_len << "\n" <<
            "\n"
        );

        /** \todo try xcb_list_installed_colormaps with root_window */
        /** \todo try what i3 is doing... https://github.com/Airblader/i3/blob/a85750f7cee38383e9e0ba699727c48b58a5eeae/libi3/get_colorpixel.c#L70 */

        xcb_screen_next(&screen_iter);
    }
}

















amGHOST_Window *amGHOST_SystemXCB::create_window(const char *title, int posX, int posY,  int sizeX, int sizeY, bool instantShow) {
    amGHOST_WindowXCB *REY = new amGHOST_WindowXCB(title, posX, posY, sizeX, sizeY);
    if (!REY->XW) {
        delete REY;
        return nullptr;
    }
    else {
        if (instantShow) {REY->show_window();}

        if (T_WindowVec.should_resize()) {T_WindowVec.resize();}
        amVK_ArrayDYN_PUSH_BACK_SAFE(T_WindowVec) = (amGHOST_Window *) REY;
    }
    return (amGHOST_Window *) REY;
}

bool amGHOST_SystemXCB::destroy_window(amGHOST_Window* window) {
    if (window) {
        amGHOST_WindowXCB *REY = (amGHOST_WindowXCB *) window;
        REY->destroyer();

        for (uint32_t i = 0, n = T_WindowVec.neXt; i < n; i++) {
            if (T_WindowVec.data[i] == window) { T_WindowVec.lazy_erase(i); break; }
        }

        return true;
    }
    return false;
}

#include <unistd.h>
bool amGHOST_SystemXCB::process_events(bool waitForEvent) {
    sleep(1);
    return true;
}
bool amGHOST_SystemXCB::opengl_load(void) {
    amVK_LOG("WIP OPENGL"); return false;
}

















/**
 *      2022: A Journey into XCB
 *
 * Okay, so the thing is XCB dones't have any written Source Code....
 * a File ['XProto.c'](https://github.com/StarchLinux/libxcb/blob/master/xproto.c) is generated during build...
 *
 * After a lotta attemps to build XCB myself... couple days of head banging on the table...
 * I finally realized that I could just search for XProto.c online.... 😆
 * 
 * And thanks to the good folk StarchLinux.... I survived the night, I could finally sleep... 
 *              instead of being up all night, cooking ways to bring my death
 * 
 * 
 *   1.   So Here's the   \fn xcb_create_window()     IMPL: https://github.com/StarchLinux/libxcb/blob/master/xproto.c#L1861          [commit 01d31d7 on Aug 18, 2013]
 *   2.   it calls upon   \fn xcb_send_request()      IMPL: https://github.com/freedesktop/xcb-libxcb/blob/master/src/xcb_out.c#L353  [commit 8584c0e on May 14, 2015]
 *        in that file it actually ends up calling
 * 
 *   3.      \fn   xcb_send_request_with_fds64()      IMPL: https://github.com/freedesktop/xcb-libxcb/blob/master/src/xcb_out.c#L221
 *        theres smth called  'FD' in that file.... i didn't understand that part, but did get the next main thing...
 * 
 *   4.   it calls upom    \fn    send_request()      IMPL: https://github.com/freedesktop/xcb-libxcb/blob/master/src/xcb_out.c#L42
 *   5.   that calls upon  \fn   _xcb_out_send()      IMPL: https://github.com/freedesktop/xcb-libxcb/blob/master/src/xcb_out.c#L454
 *   6.   and that upon    \fn  _xcb_conn_wait()      IMPL: https://github.com/freedesktop/xcb-libxcb/blob/master/src/xcb_conn.c#L435 [commit b0e6c2d on Aug 18, 2014]
 * 
 *         if we are to trace that vector input.... 
 *   7.                    \fn       write_vec()      IMPL: https://github.com/freedesktop/xcb-libxcb/blob/master/src/xcb_conn.c#L212
 * 
 *   8.           \fn write_vec() -> writev()    now this function I think... is used to talk to the XServer... 
 * 
 * 
 * 
 *  REF:
 *   - https://stackoverflow.com/questions/55088655/where-can-i-find-the-implementation-of-xcb-library
 *   
 *  RULE OF THUMB:
 *   - `This function just takes the given arguments and sends them to the X11 server. No magic is applied.`
 *   - every function inside `XProto.c` is kinda the same.... in this way....
 * 
 *  
 *          so XCB is kinda about sending the MESSAGE through the CONNECTION to the XSERVER....
 * 
 *  FROM STACK:
 *      In the X.org X11 server, the PolyLine request is handled by the function ProcPolyLine. 
 *      Its implementation will most likely not enlighten you much either: https://cgit.freedesktop.org/xorg/xserver/tree/dix/dispatch.c#n1802
 * 
 * 
 * 
 *  Enter XSERVER:
 *    1.    \fn ProcCreateWindow()      https://github.com/freedesktop/xorg-xserver/blob/master/dix/dispatch.c#L739         [commit 6b47321 on Apr 28, 2021]
 *         Now the actual Implimentation is actually here.... for most cases....
 *    2.    \fn CreateWindow()          https://github.com/freedesktop/xorg-xserver/blob/master/dix/window.c#L756           [commit 23e8372 on Jul 6, 2020]
 *         But for this case.... Proc on called upon CreateWindow of   'dix'
 * 
 *  DIX: Device Independent X   (REF: https://www.x.org/wiki/Development/Documentation/Glossary/)
 */





/**
 * XCB (X11)  VS   WAYLAND
 * 
 * Even if you can see the users screaming all over the internet that WAYLAND is freaking broken....
 *      trust me its still worth the time, otherwise BLENDER wouldn't have looked at it
 * 
 * Start Here: https://venam.nixers.net/blog/unix/2016/10/25/x11-blah-blah.html
 * 
 * NOTES:
 *  - A Display Server (e.g. Xephyr a XServer) can just output to a Window
 *  - First the Terms:
 *    - The X server, Wayland compositors, and Mir are implementations of display servers.
 *    - Xlib and XCB are libraries implementing the client-side of the Xserver/X11 Protocal.
 *    - XServer the oldest display manager that is still alive today. All its predecessors are deprecated.
 *    
 *    - Wayland is a protocol. It essentially is a standardized way in which a compositor 
 *    - (the thing that draws the windows, handles the input, etc) can speak to individual programs, and vice-versa.
 *    - There are many implementations of that protocol, and they are called Wayland compositor. e.g. (weston compositor - the official one, overrated)
 * 
 *    - Wayland is more about the compositor... e.g.
 *    - When you hear someone say that they switched to wayland, it probably means that they switched to a wayland compositor.
 *    - From WIKI: A display server using the Wayland protocol is called a Wayland compositor, because it additionally performs the task of a compositing window manager.
 *    
 *    - FROM WIKI: WAYLAND: is a communication protocol that specifies the communication between a display server and its clients, as well as a C library implementation of that protocol.
 * 
 * 
 *  - The focus on graphic device drivers, EGL, OpenGL, OpenVG, etc. Mesa3D, it’s all about composition. In X it’s done by an extension.
 *  
 * 
 *  - ARCH: Display servers using the Wayland protocol are called compositors because they also act as compositing window managers
 *  - WIKI: The composition manager is a separate, additional feature in X, while Wayland merges display server and compositor as a single function.
 * 
 * See the ARCH page and other stuff it links to for more....
 * ARCH WAYLAND: https://wiki.archlinux.org/title/wayland
 * WAYLAND Compositor VS XSERVER: https://en.wikipedia.org/wiki/Wayland_(display_server_protocol)#Differences_between_Wayland_and_X
 * 
 * 
 * 
 *      Xlib and xcb are both libraries to write code that interfaces with the x11 server. 
 *      They are the helper layer that clients use instead of implementing all the specifications and communication of the X11 server. 
 *      They just include the library and call the functions. It would be awkward for applications to spit raw X protocol.
 * 
 * 
 * 
 * WAYLAND PROTOCAL:
 *  - The protocol is a decription of “asynchronous object-oriented” actions. 
 *  - THERE's A HELL LOT OF REFERENCES in the wikipedia page's end... about 127+ references
 * 
 * 
 * XSERVER is just OLD:....
 *  - 'XCB has no equivalent to Xlib's XStringToKeysym' : https://bbs.archlinux.org/viewtopic.php?id=59840    [https://lists.freedesktop.org/archives/xcb/2008-March/003343.html]
 *  - Tho it had someone, Kristian Hogsberg, and his AIGLX: https://en.wikipedia.org/wiki/AIGLX               [https://en.wikipedia.org/wiki/Compositing_window_manager#Linux]
 *  - XCB MODULES: https://xcb.freedesktop.org/manual/modules.html      [API actually, you can basically find all the functions here, All at Once]
 *  - X11 PROTOCAL SPECS: https://www.x.org/releases/X11R7.7/doc/index.html#protocol
 *  - The worst THing so far is... there's just enormous things left in the TODO: https://xcb.freedesktop.org/XCBToDo/   https://xcb.freedesktop.org/TODO/
 *  - XCB TUTS: https://xcb.freedesktop.org/tutorial/ https://www.x.org/releases/current/doc/libxcb/tutorial/index.html#intro 
 *  - XCB PUBS: https://xcb.freedesktop.org/Publications/
 *  - X11 PDF: https://www.x.org/releases/current/doc/libX11/libX11/libX11.pdf
 *  - XPROTO: https://www.x.org/releases/current/doc/xproto/x11protocol.pdf
 *  - A CONFIRMATION on WRITEV: https://www.x.org/wiki/guide/xlib-and-xcb/
 * 
 * 
 * MONITORS in WAYLAND VS XSERVER: https://zamundaaa.github.io/wayland/2021/12/14/about-gaming-on-wayland.html
 * EXPERIENCE WITH NVIDIA ON WAYLAND:  https://blog.devgenius.io/wayland-and-nvidia-in-2022-2f0407fb34f4
 * WATCH THIS XPLAIN: https://magcius.github.io/xplain/article/
 * Go to all other links from that    blah-blah BLOG,     ARCH WIKI (WAYLAND),    Wikipedia Page (WAYLAND COMP vs XSERVER)
 */




/**
 * X11 = X = X Window System [X Window System], its just the FrameWork. You can also say 
 *       X = X-Protocol                         bcz, its a part of the X Window System/Framework
 * 
 * Now X.Org Server = XServer is a IMPL: of that FrameWork/System/Protocol
 * & Implementations of the client side of the protocol are available e.g. in the form of Xlib and XCB.
 * 
 * XW: [RootWindow]: in XServer, this can't be changed.... Thats why other implimentations of X11 exists.... e.g. Xephyr or Xnext
 * 
 *      XEPHYR: https://en.wikipedia.org/wiki/List_of_display_servers
 *      Then comes the idea of VIRTUAL ROOT WINDOW.... in XServer, those are simply Re-Parenting Window Managers....
 *          https://en.wikipedia.org/wiki/Root_window
 *          you can just go ahead and use a literal application window as PARENT of another window....
 *          \todo Tho I am not sure of the concequences yet....
 *          EXAMPLE: https://github.com/da4089/tvtwm/blob/9c1bd81be9e8024ce56b4ef58e347702bdee4757/add_window.c#L450-L464
 *          You can go look through other Window managers too... if you want to....
 * 
 */

/**
 * XCB ERRORS: amGHOST_xcb_error()
 *    Right now we are using libxcb-errors....
 *      that gives us names to MINOR_CODE, MAJOR_CODE, ERROR_CODE, and probably response_type too...
 * 
 *    But on the internet there is literally zero info... about those errors....
 *      so some point in time we gotta develop out own fucking thingy....
 */


/**
 * ICCCM - 
 *    start here: https://tronche.com/gui/x/icccm/sec-4.html
 *      & HERE: https://www.uninformativ.de/blog/postings/2016-01-05/0/POSTING-en.html
 * 
 * EWMH: Extended Window Manager Hints
 *    start here: https://en.wikipedia.org/wiki/Extended_Window_Manager_Hints
 *      file: /usr/include/xcb/xcb_errors.h
 *      pakg: libxcb-erros - https://gitlab.freedesktop.org/xorg/lib/libxcb-errors/-/tree/master/src
 * 
 * 
 *      https://tronche.com/gui/x/xlib/ICC/client-to-window-manager/wm-normal-hints.html
 * 
 * 
 * EXAMPLES FROM THE INTERNET:
 *    https://stackoverflow.com/questions/59756680/how-to-display-xcb-windows-name-in-gnomes-alt-tab-view
 */


/**
 * COLORMAPS: https://xcb.freedesktop.org/colorsandpixmaps/
 *    \see amGHOST_SystemXCB::print_screen_info()
 *    xcb_screen_t.default_colormap  ... 
 * 
 *      now whats that all about???
 * 
 * https://tronche.com/gui/x/xlib/color/#:~:text=Xlib%20provides%20functions%20that%20you,typically%20results%20in%20different%20colors.
 * https://tronche.com/gui/x/xlib/color/XCopyColormapAndFree.html
 * 
 *    also \see xcb_lookup_color_unchecked
 * https://stackoverflow.com/questions/41279160/xcb-library-how-to-draw-in-colours
 * 
 *    also \see xcb_list_installed_colormaps_cmaps
 * check out this line... whats it for, I wonder: https://github.com/Airblader/i3/blob/a85750f7cee38383e9e0ba699727c48b58a5eeae/libi3/get_colorpixel.c#L70
 */