#pragma once
#ifndef amGHOST_V
#define amGHOST_V

/** CMAKE / MESON option */
#ifndef amGHOST_V_DISABLED

/** has Include Guard.... */
#include "amGHOST_System.hh"

/** \see amGHOST_System.cpp */
#ifdef amGHOST_V_IMPLIMENTATION

    static inline amGHOST_System *amGHOST_init(void) {
        if (amGHOST_System::heart == nullptr) {
            amGHOST_System::create_system();
        }
        return amGHOST_System::get_system();
    }
    amGHOST_System *amGHOST_X = amGHOST_init();

    #define amGHOST_SYS    amGHOST_X

#else

    /** This used to be static.... */
    extern amGHOST_System *amGHOST_X;
    #define amGHOST_SYS    amGHOST_X
    
#endif

#endif // amGHOST_V_DISABLED

#endif // amGHOST_V