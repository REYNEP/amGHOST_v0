#pragma once
#ifndef amGHOST_EVENT
#define amGHOST_EVENT

#ifndef amGHOST_TYPES
  #include "amGHOST_Types.h"
#endif
#ifndef amGHOST_WINDOW
  #include "amGHOST_Window.hh"
#endif

/**
 * describes a single event like KeyPress, WindowClose, Resize....
 * 
 *  TODO:
 *    Joystick,
 *    GraphicsTablet,
 *    any HID (HardwareInputDevice)
 *    & TimeStamp of the Event Support (win32 has it)
 * 
 * We Needed a Struct, And as we know in C++ structs and classes are the same
 * All of these below are inside amGHOST_Types.h meaning that these are C compliant
 * Blender's GHOST Declares these inside of protected, But we don't because, we wanted to let ppl just type event.m_type and access that
 * 
 */
class amGHOST_Event 
{
 public:
  amGHOST_TEventType m_type = amGHOST_kUnknownEvent;
  amGHOST_Window* m_win = nullptr;

  /** Intoduce furthur Pointers into extended data, like GraphicsTablet */
  union TEventData {
    amGHOST_TKey key[2];          // 2nd one is dummy
    amGHOST_MiceInput mouse[2];   // 2nd one is dummy
    amGHOST_TMicePos mousePos;
  } m_data;



  /**
    CONSTRUCTOR & DESTRUCTOR
  */
  amGHOST_Event() {}
  amGHOST_Event(amGHOST_TEventType type, amGHOST_Window* win, amGHOST_TKey key)          : m_type(type), m_win(win) { m_data.key[0] = key; }
  amGHOST_Event(amGHOST_TEventType type, amGHOST_Window* win, amGHOST_MiceInput mouseIN) : m_type(type), m_win(win) { m_data.mouse[0] = mouseIN; }
  amGHOST_Event(amGHOST_TEventType type, amGHOST_Window* win, amGHOST_TMicePos mousePos) : m_type(type), m_win(win) { m_data.mousePos = mousePos; }

  ~amGHOST_Event() {}



  /**
   * Returns the event type.
   * \return The event type.
   */
  inline amGHOST_TEventType get_type(void) {
    return m_type;
  }

  /**
   * Returns the window this event was generated on,
   * or NULL if it is a 'system' event.
   * \return The generating window
   */
  inline const amGHOST_Window* get_window(void) {
    return m_win;
  }

  /**
   * Returns KeyBoard/Mouse/MoreDevicesToBeAdded
   * or NULL if it is a 'system' event.
   */
  inline amGHOST_TKey get_key(void) {
    return m_data.key[0];
  }

  inline amGHOST_TMicePos get_mousePos(void) {
    return m_data.mousePos;
  }

  /** C-API Support for now */
  inline amGHOST_TEventData get_data(void) {
    return {m_data.key[0], m_data.key[1]};
  }
};

#endif //#ifndef amGHOST_EVENT