#include "amGHOST_Logger.hh"
#include "amGHOST_System.hh"

/**
  ----------------------
  INit & EXit
  ----------------------
*/
amGHOST_System *amGHOST_System::heart = NULL; //Otherwise get LNK2001 Error

#if defined(_WIN32)
  #include "amGHOST_SystemWIN32.hh"
#endif

void amGHOST_System::create_system(void) {
  if (heart) {
    LOG("[amGHOST_System::create_system]: A GHOST System Already Exists, Please Destroy it before you can create Another System.");
    return;
  }

  #if defined(_WIN32)
    heart = (amGHOST_System *) new amGHOST_SystemWIN32();
  #endif
}

void amGHOST_System::dispose_system(void) {
  for (amGHOST_EventKonsument *EK : T_EKVec) {
    delete EK;
  }
  for (amGHOST_Context *C : T_ContextVec) {
    C->destroyer();
  }
  for (amGHOST_Window *W : T_WindowVec) {
    W->destroyer();
  }
  
  if (heart) {
    delete heart;
    heart = NULL;
  }
}

amGHOST_System* amGHOST_System::get_system(void) {
  if (!heart) {
    LOG("NO GHOST System Exists, Please Create One Before you can Get one, Returning NULL back");
  }
  return heart;
}






/**
* ----------------------------
* Event System Functions
* ----------------------------
* \see EK DOCS:- amGHOST_EK.hh
*/

/**
* \see process_events() virtual function
* \param event_konsument_proc: Event Konsument Process Pointer
* \return Poitner to that new EK Object/Instance
**/
amGHOST_EventKonsument* amGHOST_System::add_eventKonsument(amGHOST_EKProcPtr event_konsument_proc) {
  amGHOST_EventKonsument *new_EK = NULL;

  for (amGHOST_EventKonsument *EK : T_EKVec) {
    if (EK->get_EKproc() == event_konsument_proc) {
      LOG("Already an Event konsument added with that exact same function that you have passed");
      return new_EK;
    }
  }

  new_EK = new amGHOST_EventKonsument(event_konsument_proc);  //created on the heap-memory
  T_EKVec.push_back(new_EK);

  return new_EK;
}

bool amGHOST_System::add_T_EKVec(amGHOST_EventKonsument* new_EK, bool checkIfExists) {
  if (checkIfExists) {
    for (amGHOST_EventKonsument *EK : T_EKVec) {
      if (EK->get_EKproc() == new_EK->get_EKproc()) {
        LOG("Already an Event konsument added with that exact same function that you have passed");
        return false;
      }
    }
  }

  for (amGHOST_EventKonsument *EK : T_EKVec) {
    if (EK == new_EK) {
      LOG("This EK already Exists");
    }
  }

  T_EKVec.push_back(new_EK);
  return true;
}


/**
* Dispastch Events to all EKs
* \see process_events() virtual function
* \return false if any of the EK returns -1, otherwise return true after dispatching all events in EVENTQ
*/
bool amGHOST_System::dispatch_events(void) {
  for (int i = 0, n = (int) EventQ.size(); i < n; i++) {
    for (amGHOST_EventKonsument *EK : T_EKVec) {
      if (EK->dispatch_event(EventQ.front()) == -1) {
        EventQ.pop_front(); 
        return false;
      }
    }

    //The Line below should be called after all EK's has been called
    EventQ.pop_front();
  }
  return true;
}
bool amGHOST_System::is_eventQ_empty(void) {
  return EventQ.empty();
}