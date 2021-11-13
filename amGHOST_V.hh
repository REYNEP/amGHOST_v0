#ifndef amGHOST_V
#define amGHOST_V
#include "amGHOST_System.hh"

static int amGHOST_init(void) {
  if (amGHOST_System::heart == nullptr) {
    amGHOST_System::create_system();
  }
  return 0;
}
static int amGHOST_V2_init_return_var_delete = amGHOST_init();

static amGHOST_System *amGHOST = amGHOST_System::get_system();
#define amGHOST_SYS amGHOST

#endif //amGHOST_V