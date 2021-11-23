#ifndef amGHOST_V
#define amGHOST_V

#include "amGHOST_System.hh"

/** inline, cz lets have multiple copied of code lines.... instead of Multiple Copies of the function itself */
static inline amGHOST_System *amGHOST_init(void) {
  if (amGHOST_System::heart == nullptr) {
    amGHOST_System::create_system();
  }
  return amGHOST_System::get_system();
}

/** Only problem with this file is, its gonna have this var in every Translation Unit.... 
 *  I can think of a bug, if 2 diff TranslationUnit in a .exe gets loaded in Different Threads, i dont think such tech/tool exists 
 * whatever. duh. 😜 */
static amGHOST_System *amGHOST = amGHOST_init();
#define amGHOST_SYS amGHOST

#endif //amGHOST_V