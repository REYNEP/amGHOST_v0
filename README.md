<h2 align=center> amGHOSTV0.1 - from Blender's GHOST [C++/C]</h2>

---

```cpp
/** Includes all amGHOST headers */
#include "amGHOST_V.hh"

int EK(amGHOST_Event *event) {  /** EventKonsument */
    if (event->m_type == amGHOST_kWindowClose) {
        return -1;
    }
    return 0;
}

int main(void) {
    // amG_SYS is a macro defined in amGHOST_V.hh
    amGHOST_Window *wOne = amG_SYS->create_window('TestTitle', 0, 0, 1280, 720);
    amG_SYS->add_EventKonsument(&EK);

    while(true) {
        amG_SYS->process_events(true);
        if (amG_SYS->dispatch_events() == -1) {     // Dispatches to EventKonsuments (EK)
            break;
            /** 
             * this means, one of UR EK's returned -1, processing an event.... 
             * that event is still there, but at `amG_SYS->_EventQ.data[0]`   (index 0)
             * 
             * you should process the next events carefully
             * amG_SYS->_EventQ   is  [PUBLIC]
             */
        }
    }

    /** delete wOne, won't work. */
    amG_SYS->destroy_window(wOne);
}
```


### instead of `#include "amGHOST_V.hh"`    [NOTE: You GOTTA turn off cmake `amGHOST_V` option]
```cpp
#include "amGHOST_System.hh"

    amGHOST_System::create_system();
    amGHOST_System *amG_SYS = amGHOST_System::get_system();
/** amGHOST_System *amG_SYS = amGHOST_System::heart;     Works OK */
/** Its okay to call outside main, in global Space */

/** Yes, if you wanna do this... then you will have to build amGHOST from source */
```


> ### Check `/tests/*.cpp` files for more examples   [e.h. EK_resize.cpp]

---
##

## Build
- `python make.py`  - ***it's interactive!!!*** That downloads 'extern' folder from [GoogleDrive](https://drive.google.com/file/d/1pGGfm0yh6bExzQlu3Da4-NJP86m6r_3s/)
    - ***GLEW*** [with CMakeLists.txt]
    - ***Vulkan-Sdk-Lunarg*** [Headers + Vulkan-Loader lib (`lib-vulkan.so vulkan.lib`]
- You can make UR own copies of [amGHOST_Options.cmake](build_files/amGHOST_Options.cmake) & [amGHOST_Variables.cmake](build_files/amGHOST_Variables.cmake) </br> 
    - do `list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/path/to/<Options/Variables.cmake>)` before `add_subdirectory(amGHOST)`
- CMake: `add_subdirectory(amGHOST)     target_link_libraries(re lib-amGHOST)`  [DOCS in [CMakeLists.txt](CMakeLists.txt)]
- MESON: `sp = subproject('amGHOST')    amGHOST_defs = sp.get_variable('def')`  [DOCS in [meson.build](meson.build)]

## Macros & Features
`amGHOST_BUILD_VULKAN`:
- **Option:** `amGHOST_VULKAN_WindowSurface` [cmake, meson]
- **FILES:** `amGHOST_Window.hh`

`_LOG` (from `amVK`):
- **Option:** `amGHOST_DEV` [cmake, (NO) meson]
- **FILES:** `Literally Everywhere....`

## SMTH IN THE WAY
`amGHOST_System`:
* You can't just instantiate an `amGHOST_System` Object, cz this class contains *Pure-Virtual* funcs (a.k.a This is called an Abstract Class)
* You have to call `amGHOST_System::create_system();`
* And at the end of the program `amGHOST_System::dispose_system()` is preferred to be called or MEMORY Will be leaked
* 1 System could have multiple rendering Contexts. Maybe even multiple Graphics APIs (OpenGL/VULKAN/DX12)

## ACKNOWLEDGEMENT
Thanks to Blender, mainly Blender's GHOST, really.... This is an Complete re-write of that. 
They have a Lot of Helping Docs in their code.... and It was all possible because Blender is again... OPEN-SOURCE as Always

## LICENSE
amGHOST is licensed under BOOST LICENSE

</br></br>

---
##

> GHOST: Generic Handy Operating System Toolkit

> After I finished CS50, this is what I started with. ripping off parts from blender's GHOST & making an alternative to GLFW... I didn't plan that big or robust or featurefull api, cz making modifications would be really not be that easy.... & things even flow through above my head, as I'm just another (almost 17yo) kid who just started making (somewhat) standard stuffs....

<!-- If you use `switch-case` remember to do break. cz case is like 'Labels' and if you don't break the flow of code will continue >