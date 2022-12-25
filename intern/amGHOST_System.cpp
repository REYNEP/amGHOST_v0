#include "amGHOST_V.hh"
#include "amVK_Logger.hh"
#include "amGHOST_System.hh"

#if defined(amGHOST_BUILD_WIN32) || defined(_WIN32)
  #include "amGHOST_SystemWIN32.hh"
#elif defined(amGHOST_BUILD_X11)
  #include "amGHOST_SystemX11.hh"
#elif defined(amGHOST_BUILD_XCB) || defined(__unix__)
  #include "amGHOST_SystemXCB.hh"
#endif

void amGHOST_System::create_system(void) {
  if (heart) {
    amVK_LOG("[amGHOST_System::create_system]: A GHOST System Already Exists, Please Destroy it before you can create Another System.");
    return;
  }

  #if defined(amGHOST_BUILD_WIN32) || defined(_WIN32)
    heart = (amGHOST_System *) new amGHOST_SystemWIN32();
  #elif defined(amGHOST_BUILD_X11)
    heart = (amGHOST_System *) new amGHOST_SystemX11();
  #elif defined(amGHOST_BUILD_XCB) || defined(__unix__)
    heart = (amGHOST_System *) new amGHOST_SystemXCB();
  #endif
}

void amGHOST_System::dispose_system(void) {
  //for (amGHOST_EventKonsument *EK : T_EKVec) {
  //  delete EK;
  //}

  T_EKVec._delete();

  //for (amGHOST_Context *C : T_ContextVec) {
  //  C->destroyer();
  //}
  
  //for (amGHOST_Window *W : T_WindowVec) {
  //  W->destroyer();
  //}

  for (uint32_t i = 0, n = T_WindowVec.neXt; i < n; i++) {
    T_WindowVec.data[i]->destroyer();
  }
  /** TODO: empty T_WindowVec & ZERO-INIT */
  
  if (heart) {
    delete heart;
    heart = nullptr;
  }
}

amGHOST_System* amGHOST_System::get_system(void) {
  if (!heart) {
    amVK_LOG_EX("NO GHOST System Exists, Please Create One Before you can Get one, Returning NULL back");
    return nullptr;
  }
  return heart;
}








/**
 * Dispatch every Event one-by-one to ALL EKs
 * \todo Write all the events captured to a file....   [enable LIMIT... (store only the last 100 events or so....)]
 */
bool amGHOST_System::dispatch_events(void) {
  //amVK_LOG("Dispatching events: " << _EventQ.neXt);
  for (uint32_t i = 0, n = _EventQ.neXt; i < n; i++) 
  {
    for (uint32_t k = 0, n = T_EKVec.neXt; k < n; k++) 
    {
      //amVK_LOG("  TO: " << (uint64_t)T_EKVec.data+k << "    nd the event: " << (uint64_t)_EventQ.data + i << "     []: " << _EventQ.data[i].m_type);
      if (T_EKVec.data[k].dispatch_event(_EventQ.data + i) == -1) 
      {
        if (i > 0) {
          /** this won't be that expensive, anyway */
          amVK_memcpy(_EventQ.data, _EventQ.data + i, sizeof(amGHOST_Event) * (_EventQ.neXt-i));
          _EventQ.neXt -= i;
        }
        _EventQ.neXt = 0;
        amVK_LOG_EX("Returning FALSE");
        return false;
      }
    }
  }

  _EventQ.neXt = 0;
  return true;
}

amGHOST_EventKonsument* amGHOST_System::add_EventKonsument(amGHOST_EKProcPtr event_konsument_proc) {
  for (uint32_t k = 0, n = T_EKVec.neXt; k < n; k++) {
    if (T_EKVec.data[k].get_EKproc() == event_konsument_proc) {
      amVK_LOG("Already an Event konsument added with that exact same function that you have passed");
      return nullptr;
    }
  }

  if (T_EKVec.should_resize()) {T_EKVec.resize(2);}
  amVK_ARRAY_PUSH_BACK_SAFE(T_EKVec) = amGHOST_EventKonsument(event_konsument_proc);

  return T_EKVec.data + T_EKVec.neXt-1;
}