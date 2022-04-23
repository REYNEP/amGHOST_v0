<h2 align=center> amGHOSTV0.1 - from Blender's GHOST [C++/C]</h2>

---

```cpp
/** Includes all amGHOST headers */
#include "amGHOST_V.hh"

int EK(amGHOST_Event *event) {  /** EventKonsument */
    if (event.m_type == amGHOST_kWindowClose) {
        return -1;
    }
    return 0;
}

int main(void) {
    amGHOST_Window *wOne = amg_sys->create_window('TestTitle', 0, 0, 1280, 720);
    amg_sys->add_eventKonsument(&EK);

    while(true) {
        amg_sys->process_events(true);
        if (amg_sys->dispatch_events() == -1) {     // Dispatches to EventKonsuments (EK)
            break;
            /** 
             * this means, one of UR EK's returned -1, processing an event.... 
             * that event is still there, but at `amg_sys->_EventQ.data[0]`   (index 0)
             * 
             * you should process the next events carefully
             * amg_sys->_EventQ   is  [PUBLIC]
             */
        }
    }

    /** delete wOne, won't work. */
    amg_sys->destroy_window(wOne);
}
```


### instead of `#include "amGHOST_V.hh"`    [NOTE: turn off cmake `amGHOST_V` option]
```cpp
#include "amGHOST_System.hh"

    amGHOST_System::create_system();
    amGHOST_System *amg_sys = amGHOST_System::get_system();
/** amGHOST_System *amg_sys = amGHOST_System::heart;     Works OK */
/** Its okay to call outside main, in global Space */

/** Yes, if you wanna do this... then you will have to build amGHOST from source */
```

> ### Check `/tests/*.cpp` files for more examples   [e.h. EK_resize.cpp]
> ### C-API> [intern/amGHOST_Capi.hh](intern/amGHOST_Capi.hh)




---
##

</br>




## Macros & Features
`amGHOST_BUILD_VULKAN`:
- **Option:** `amGHOST_VULKAN_WindowSurface` [cmake, meson]
- **CMAKE:** `add_submodule(amGHOST)    target_link_libraries(UR_EXE PUBLIC lib-amGHOST)`,   include_dirs & defs will automatically be taken care of
- **MESON:** `sp = subproject('amGHOST')    amGHOST_defs = sp.get_variable('def')`      [so the support is partly for now....]
- **Default:** `#define amGHOST_BUILD_VULKAN` before `#include "amGHOST_System.hh"`
- **FILES:** `amGHOST_Window.hh`

## Build
#### `python make.py`  - That downloads 'extern' folder from [GoogleDrive](https://drive.google.com/file/d/1pGGfm0yh6bExzQlu3Da4-NJP86m6r_3s/)

## ACKNOWLEDGEMENT
All thanks to Blender and Blender's GHOST, really.... This is an Complement re-write of that. 
They have a Lot of Helping Docs in their code.... and It was all possible because Blender is again... OPEN-SOURCE as Always

## LICENSE
amGHOST is licensed under GNU GPL 3.0
See license.txt

</br></br>

---
##

> GHOST: Generic Handy Operating System Toolkit

> After I finished CS50, this is what I started with. ripping off parts from blender's GHOST & making an alternative to GLFW... I didn't plan that big or robust or featurefull api, cz making modifications would be really not be that easy.... & things even flow through above my head, as I'm just another (almost 17yo) kid who just started making (somewhat) standard stuffs....

<!-- If you use `switch-case` remember to do break. cz case is like 'Labels' and if you don't break the flow of code will continue >