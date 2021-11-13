#include <stdio.h>
#include <stdlib.h>
#include "amGHOST_Capi.h"

int amghost_event_proc(amGHOST_EventHandle event);

int main(void) {
  amGHOST_CreateSystem();

  amGHOST_WindowHandle wOne = amGHOST_CreateWindow("TestWindowCPP", 0, 0, 1280, 720, true);
  amGHOST_CreateContextOpenGL(wOne);

  amGHOST_AddEventKonsument(&amghost_event_proc);
  //Blender Standard MAIN LOOP (a.k.a EVENT LOOP)
  while(1) {
    amGHOST_ProcessEvents(true);
    if (!amGHOST_DispatchEvents()) {break;}
  }

  amGHOST_DestroyWindow(wOne);
  amGHOST_DisposeSystem();
  return 1;
}

//[EventKonsument] EK Function          See: amGHOST_EK.h for the concept of EventConsumerFunctions
int amghost_event_proc(amGHOST_EventHandle event) {
  char *keyStatus = (char *) malloc(10);
  
  amGHOST_TEventType type = amGHOST_GetEventType(event);

  if (type == amGHOST_kKeyDown) {
    keyStatus = "Key Down";
  }
  else if (type == amGHOST_kKeyUp) {
    keyStatus = "Key Up";
  }

  amGHOST_TEventData keyData = amGHOST_GetEventData(event); 

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