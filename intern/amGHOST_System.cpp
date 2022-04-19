#include "amVK_Logger.hh"
#include "amGHOST_System.hh"

#define amGHOST_V_IMPLIMENTATION
#include "amGHOST_V.hh"

#if defined(amGHOST_BUILD_WIN32) || defined(_WIN32)
  #include "amGHOST_SystemWIN32.hh"
#endif

void amGHOST_System::create_system(void) {
  if (heart) {
    amVK_LOG("[amGHOST_System::create_system]: A GHOST System Already Exists, Please Destroy it before you can create Another System.");
    return;
  }

  #if defined(amGHOST_BUILD_WIN32) || defined(_WIN32)
    heart = (amGHOST_System *) new amGHOST_SystemWIN32();
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