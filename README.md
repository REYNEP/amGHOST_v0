#### Build using `python make.py`  - That downloads 'extern' folder from [GoogleDrive](https://drive.google.com/file/d/1dkGd6qD6N6dUO35gNzz5GMV-UpdluBBm/view?usp=sharing)

## amGHOSTV0.1b - [from Blender's GHOST]
##### <ins>GHOST - Generic Handy Operating System Toolkit</ins>
After finished CS50, this is what I started with. ripping off parts from blender's GHOST & making an alternative to GLFW... I didn't that big or robust or featurefull api, cz making modifications would be really not that easy & may even go above my head, as I'm just another (almost 17yo) smone who just started making (somewhat) standard stuffs....

#### & amGHOST is pretty Simple:
```cpp
#include "amGHOST_System.hh"    //includes all amGHOST headers

//Its okay to call outside main, in global Space
amGHOST_System::create_system();
amGHOST_System *amg_sys = amGHOST_System::get_system();
//amGHOST_System *amg_sys = amGHOST_System::heart;   Works too!, heart stores the 1 and only system

int main(void) {
    amGHOST_Window *wOne = amg_sys->create_window('TestTitle', 0, 0, 1280, 720);

    while(true) {
        amg_sys->process_events();
        //I thought it would be better to just not have shouldWindowClose(), rather a simple way to do that.... you know Explicit is better than Implicit
        bool shouldClose = false;
        std::deque<amGHOST_Event *> event_queue = amg_sys::EventQ;

        for (int i = 0,  n = (int) EventQ.size(); i < n; i++) {
            amGHOST_Event *event = EventQ.front();
            
            if (event.get_type() == amGHOST_kWindowClose) {shouldClose = true;}
            EventQ.pop_front();
        }

        if (shouldClose) {
            break;
        }
    }

    //Just to be safer.... yes there could be a C++ class desctuctor way of doing this, but for now lets use Universal way for C/C++
    amg_sys->destroy_window(wOne);
}
```



#### EK [DispatchSystem]:
```cpp
#include "amGHOST_System.hh"    //includes all amGHOST headers

/**
  ┏━╸╻┏ 
  ┣╸ ┣┻┓
  ┗━╸╹ ╹
*/
int windowCloseAgent(amGHOST_Event *event) {        //This is the Signature/Type of EK
  if (event->get_type() == amGHOST_kWindowClose) {
    return -1;  //Code to trigger dispatch_event() to return false
  }

  return 0; //Everything worked OK, every value other that '-1' is Considered OK
}

amGHOST_System::create_system();
amGHOST_System *amg_sys = amGHOST_System::get_system();

//can be called after system was created
amg_sys->add_eventKonsument(&windowCloseAgent);

int main(void) {
    amGHOST_Window *wOne = amg_sys->create_window('TestTitle', 0, 0, 1280, 720);

    while(true) {
        amg_sys->process_events();
        if (amg_sys->dispatch_events() == false) {break;}
    }

    amg_sys->destroy_window(wOne);
}
```


</br>











## How powerfull is EK?
```cpp
/**
  ┏━╸╻┏ 
  ┣╸ ┣┻┓
  ┗━╸╹ ╹
*/
typedef enum windowState__ {
  idling = 0,
  resizing = 1,
  resized = 2       //TODO: Test & Explain what events & why....
} windowState;

// This is the way!    [to change values of YOUR variables from within a EK like windowResizeAgent]
// P.S. Watch Mandalorian
windowState wndState;
uint32_t resizeCount = 0;

//+ Most probably by the time you get back to this EK in MAIN LOOP [after 1 iteration]
//+ you'll most probably receive more than a couple of Tens of Messages
//+ I mean if its a game engine....
//+ dispatch_events() will call every registered EK for every single Event in EventQ
int windowResizeAgent(amGHOST_Event *event) {
  //Windows actually sends 1 Resized status right after RESIZING.... so its not like resize is done
  if (event->get_type() == amGHOST_kWindowResizing) {
    if (wndState == idling) {
      wndState = resizing;
    }
  }
  else if (event->get_type() == amGHOST_kWindowResized) {
    wndState = resized;
  }

  return 0;
}
```
### If you use `switch-case` remember to do break. cz case is like 'Labels' and if you don't break the flow of code will continue



</br></br>

## ACKNOWLEDGEMENT
All thanks to Blender and Blender's GHOST, really.... This is an Complement re-write of that. 
They have a Lot of Helping Docs in their code.... and It was all possible because Blender is again... OPEN-SOURCE as Always

## License
amGHOST is licensed under GNU GPL 3.0
See license.txt