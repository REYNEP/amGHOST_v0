#ifndef amGHOST_LOGGER
#define amGHOST_LOGGER
//Include Guard

#include <iostream>

// LOGGER
#define LOG(x) std::cout << x << std::endl

//See: https://stackoverflow.com/questions/27174929/what-is-a-best-way-to-check-if-compiler-supports-func-etc
//__FUNCTION__, __func__ are not Macros, rather they are Variables
//But the reason why LOG_DEV Works is because LOG_DEV uses another function LOG which doesn't let __LINE__ to be converted directly on this files line 23
#if defined(__GNUC__)
  #define __FUNCINFO__ __FUNCTION__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
  #define __FUNCINFO__ __func__
#elif defined(_MSC_VER)
  #define __FUNCINFO__ __func__
#else
  #define __FUNCINFO__ "[FuncInfo not supported by current compiler]"
#endif

#ifdef amGHOST_DEV_CMAKE
  #define LOG_DEV(x) LOG(__FILE__ << " || Function: " << __FUNCINFO__ << "() - Line" << __LINE__ << std::endl << "Log-Message:- "<< x << std::endl)
#else
  #define LOG_DEV(x)
#endif

// MSVC Has Got a Huge amount of Predefined Macros:- https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros
#define LOG_MANGLED_MSVC(x) LOG_DEV(x << " MANGLED NAME: " << __FUNCDNAME__)

#endif //#ifndef amGHOST_LOGGER