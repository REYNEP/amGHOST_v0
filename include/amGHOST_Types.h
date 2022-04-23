//This File should be C Friendly
#ifndef amGHOST_TYPES
#define amGHOST_TYPES
//Include Guard

#include <stdint.h>  //int32_t
#include <stddef.h>

//Code Copied from Blender GHOST_types.h
#define amGHOST_CREATE_HANDLE(name) \
    typedef struct name##__ { \
        int unused; \
    } * name

amGHOST_CREATE_HANDLE(amGHOST_VkSurface_rsrcHandle);
amGHOST_CREATE_HANDLE(amGHOST_WindowHandle);
amGHOST_CREATE_HANDLE(amGHOST_SystemHandle);
amGHOST_CREATE_HANDLE(amGHOST_EventHandle);
amGHOST_CREATE_HANDLE(amGHOST_EKHandle);
amGHOST_CREATE_HANDLE(amGHOST_ContextHandle);
//MAYBE NAME amGHOST_WindowHandle to amGHOST_ptrWindow

#ifdef amGHOST_BUILD_TSuccess
  //Fancy-errrrrr.... way of dealing with return Codes..... hahhahaha
  typedef enum {amGHOST_kFailure = 0, amGHOST_kSuccess} amGHOST_TSuccess;
#else
  #define amGHOST_TSuccess bool
  #define amGHOST_kFailure false
  #define amGHOST_kSuccess true
#endif


/**
  Definition of a callback routine that receives events.
  \param event:- The event received.
  \param userdata:- The callback's user data, supplied to GHOST_CreateSystem.
  \return -1 will cause dispatch_events to immidiately return false
 *         you can return 0 to be safe
  Having the return type of int gives someone the ability to deal with different Return Situations
 */
#ifdef __cplusplus
class amGHOST_Event;
typedef int (*amGHOST_EKProcPtr)(amGHOST_Event *);
#endif
#ifndef amGHOST_BUILD_CPP_ONLY
typedef int (*amGHOST_EKProcPtr_Capi)(amGHOST_EventHandle);
#endif


/** Bcz of the reason that CPUs can have enum of different size. we go like vulkan. */
typedef enum {
    amGHOST_kUnknownEvent,
    amGHOST_kRawEvent,

    amGHOST_kKeyDown,
    amGHOST_kKeyUp,

    amGHOST_kMCursorMove,    // Mouse move event
    amGHOST_kMButton,        // Mouse button event  [Down or UP]
    amGHOST_kMWheel,         // Mouse wheel event

    amGHOST_kWindowCreated,  //see GHOST_kEventWindowActivate
    amGHOST_kWindowClose,
    amGHOST_kWindowClosed,   //see GHOST_kEventWindowDeactivate

    amGHOST_kWindowResizing,
    amGHOST_kWindowResized   
} amGHOST_EventType;
typedef int32_t amGHOST_TEventType;

/** not used. from Blender GHOST */
typedef enum {
  amGHOST_kButtonMaskNone = 0,
  amGHOST_kButtonMaskLeft = 1,
  amGHOST_kButtonMaskMiddle = 2,
  amGHOST_kButtonMaskRight = 4,
  amGHOST_kButtonMaskButton4 = 8,
  amGHOST_kButtonMaskButton5 = 16,
  /* Trackballs and programmable buttons. */
  amGHOST_kButtonMaskButton6 = 32,
  amGHOST_kButtonMaskButton7 = 64,
  amGHOST_kButtonNumMasks = 128
} amGHOST_ButtonMask;
typedef int32_t amGHOST_TButtonMask;

/** USED */
typedef enum {
  amGHOST_kLMBDown = 1,
  amGHOST_kMMBDown = 2,
  amGHOST_kRMBDown = 4,

  amGHOST_kLMBUp = 8,
  amGHOST_kMMBUp = 16,
  amGHOST_kRMBUp = 32,

  amGHOST_kWheelDown = 64,
  amGHOST_kWheelUp = 128,
  amGHOST_kWheelPlus = amGHOST_kWheelUp,
  amGHOST_kWheelMinus = amGHOST_kWheelDown
  
} amGHOST_MiceInput;
typedef int32_t amGHOST_TMiceInput;

typedef struct {
  int32_t x;
  int32_t y;
} amGHOST_TMicePos;

//Feel free to collapse this
typedef enum {
    amGHOST_kKeyUnknown = -1,

    amGHOST_kKey0 = '0',
    amGHOST_kKey1,
    amGHOST_kKey2,
    amGHOST_kKey3,
    amGHOST_kKey4,
    amGHOST_kKey5,
    amGHOST_kKey6,
    amGHOST_kKey7,
    amGHOST_kKey8,
    amGHOST_kKey9,

    amGHOST_kKeyA = 'A',
    amGHOST_kKeyB,
    amGHOST_kKeyC,
    amGHOST_kKeyD,
    amGHOST_kKeyE,
    amGHOST_kKeyF,
    amGHOST_kKeyG,
    amGHOST_kKeyH,
    amGHOST_kKeyI,
    amGHOST_kKeyJ,
    amGHOST_kKeyK,
    amGHOST_kKeyL,
    amGHOST_kKeyM,
    amGHOST_kKeyN,
    amGHOST_kKeyO,
    amGHOST_kKeyP,
    amGHOST_kKeyQ,
    amGHOST_kKeyR,
    amGHOST_kKeyS,
    amGHOST_kKeyT,
    amGHOST_kKeyU,
    amGHOST_kKeyV,
    amGHOST_kKeyW,
    amGHOST_kKeyX,
    amGHOST_kKeyY,
    amGHOST_kKeyZ,

    //Numpad keys
    amGHOST_kKeyNumpad0,
    amGHOST_kKeyNumpad1,
    amGHOST_kKeyNumpad2,
    amGHOST_kKeyNumpad3,
    amGHOST_kKeyNumpad4,
    amGHOST_kKeyNumpad5,
    amGHOST_kKeyNumpad6,
    amGHOST_kKeyNumpad7,
    amGHOST_kKeyNumpad8,
    amGHOST_kKeyNumpad9,
    amGHOST_kKeyNumpadPeriod,
    amGHOST_kKeyNumpadEnter,
    amGHOST_kKeyNumpadPlus,
    amGHOST_kKeyNumpadMinus,
    amGHOST_kKeyNumpadAsterisk,
    amGHOST_kKeyNumpadSlash,

    //Funcction Keys
    amGHOST_kKeyF1,
    amGHOST_kKeyF2,
    amGHOST_kKeyF3,
    amGHOST_kKeyF4,
    amGHOST_kKeyF5,
    amGHOST_kKeyF6,
    amGHOST_kKeyF7,
    amGHOST_kKeyF8,
    amGHOST_kKeyF9,
    amGHOST_kKeyF10,
    amGHOST_kKeyF11,
    amGHOST_kKeyF12,
    amGHOST_kKeyF13,
    amGHOST_kKeyF14,
    amGHOST_kKeyF15,
    amGHOST_kKeyF16,
    amGHOST_kKeyF17,
    amGHOST_kKeyF18,
    amGHOST_kKeyF19,
    amGHOST_kKeyF20,
    amGHOST_kKeyF21,
    amGHOST_kKeyF22,
    amGHOST_kKeyF23,
    amGHOST_kKeyF24,

    //Multimedia keypad buttons
    amGHOST_kKeyMediaPlay,
    amGHOST_kKeyMediaStop,
    amGHOST_kKeyMediaFirst,
    amGHOST_kKeyMediaLast
} amGHOST_Key;
typedef int32_t amGHOST_TKey;

typedef union {
  amGHOST_TKey key[2];
  amGHOST_TMicePos mousePos;
} amGHOST_TEventData;

#endif //#ifndef amGHOST_TYPES