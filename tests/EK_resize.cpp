#include <stdint.h>
#include "amGHOST_V.hh"

/**
  ┏━╸╻┏ 
  ┣╸ ┣┻┓
  ┗━╸╹ ╹
*/
typedef enum windowState__ {
  idling = 0,
  resizing = 1,
  resized = 2
} windowState;

windowState wndState;
uint32_t resizeCount = 0;

/** 
 * Windows actually sends 1 Resized status right after RESIZING.... 
 * so its not like resize is done even if a `amGHOST_kWindowResized` is posted....
 *  [for win32]
 */
int windowResizeAgent(amGHOST_Event *event) {
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




int EK(amGHOST_Event *event) {  /** EventKonsument */
    if (event.m_type == amGHOST_kWindowClose) {
        return -1;
    }
    return 0;
}

int main(void) {
    amGHOST_Window *wOne = amGHOST_SYS->create_window('TestTitle', 0, 0, 1280, 720);
    amGHOST_SYS->add_EventKonsument(&EK);
    amGHOST_SYS->add_EventKonsument(&windowResizeAgent);

    while(true) {
        amGHOST_SYS->process_events(true);
        if (amGHOST_SYS->dispatch_events() == -1) {     // Dispatches to EventKonsuments (EK)
            break;
            /** 
             * this means, one of UR EK's returned -1, processing an event.... 
             * that event is still there, but at `amg_sys->_EventQ.data[0]`   (index 0)
             * 
             * you should process the next events carefully
             * amg_sys->_EventQ   is  [PUBLIC]
             */
        }

        if (wndState = )
    }

    /** delete wOne, won't work. */
    amGHOST_SYS->destroy_window(wOne);
}