#include "amGHOST_System.hh" //amGHOST_System Includes Every Other needed Header File
#include "GL/glew.h"
#include <iostream>

int amghost_event_proc(amGHOST_Event *event);

int main(void) {
  // Create System
  amGHOST_System::create_system();
  amGHOST_System *amg_sys = amGHOST_System::get_system();

  //call all the amazing LibamGHOST things
  amGHOST_Window *wOne = amg_sys->create_window("TestWindowCPP", 0, 0, 1280, 720, true);
  wOne->openGL_create_context();  //Also Makes it current Context. use wOne->openGL_activate_context() for reactivating

  amGHOST_EK *ekOne = amg_sys->add_EventKonsument(&amghost_event_proc);

  //Blender Standard MAIN LOOP (a.k.a EVENT LOOP)
  while(1) {
    amg_sys->process_events(true);
    if (!amg_sys->dispatch_events()) {break;}
  }

  amg_sys->destroy_window(wOne);
  amg_sys->dispose_system();  //amGHSOT_System::dispose_system() won't work, dispose_system is not static
  return 1;
}


//[EventKonsument] EK Function          See: amGHOST_EK.h for the concept of EventConsumerFunctions
int amghost_event_proc(amGHOST_Event *event) {
  char *keyStatus = (char *) malloc(10);
  
  if (event->m_type == amGHOST_kKeyDown) {
    keyStatus = "Key Down";
  }
  else if (event->m_type == amGHOST_kKeyUp) {
    keyStatus = "Key Up";
  }

  amGHOST_TEventData keyData = event->m_data; 

  if (keyData >= (amGHOST_TEventData)48 && keyData <= (amGHOST_TEventData)90) {
    printf("%s: %c\n", keyStatus, keyData);
  }

  else if (keyData >= amGHOST_kKeyF1 && keyData <= amGHOST_kKeyF24) {
    printf("%s: F%d\n", keyStatus, keyData - amGHOST_kKeyF1 + 1);
  }

  else if (keyData == amGHOST_kKeyUnknown) {
    printf("KeyDown/Up: Unknown Key\n");
  }

  else {
    printf("Not really a Keyboard Event\n");
  }

  return 0;
}