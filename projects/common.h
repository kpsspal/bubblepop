#define WIN32_LEAN_AND_MEAN
#define WINDOWS
#define SECURITY_WIN32

#define WINVER         0x0600
#define _WIN32_WINNT   0x0600
#define _WIN32_WINDOWS 0x0600
#define _WIN32_IE      0x0600

#include <SDKDDKVer.h>
#include <Windows.h>

#define SAFE_DELETE(__ptr) if(__ptr){delete __ptr;__ptr=NULL;}
#define SAFE_DELETE_ARRAY(__ptr) if(__ptr){delete[] __ptr;__ptr=NULL;}
#define MY_ARRAYSIZE(__a) (sizeof(__a)/sizeof(__a[0]))
#define STRINGIZE(__a) __STRINGIZE(__a)
#define __STRINGIZE(__a) #__a

#define CONCATHERE(_a,_b) CONCATHERE_(_a,_b)
#define CONCATHERE_(_a,_b) _a##_##_b
#define CONV_UNICODE(__a) __CONV_UNICODE(__a)
#define __CONV_UNICODE(__a) L ##__a

#define MAX_BUBBLECOLORS 6

#define M_PI       3.14159265358979323846
#ifndef _MSC_VER

#ifndef _ASSERT
#define _ASSERT
#endif
#define MAX std::max
#else
#define MAX max
#endif
