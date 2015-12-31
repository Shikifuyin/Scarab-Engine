/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/Forwards/ForwardWin32.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Forward : <windows.h>
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_THIRDPARTY_FORWARDS_FORWARDWIN32_H
#define SCARAB_THIRDPARTY_FORWARDS_FORWARDWIN32_H

/////////////////////////////////////////////////////////////////////////////////
// Includes

/////////////////////////////////////////////////////////////////////////////////
// Compiler directives
#pragma warning(disable:4201) // Non-Standard extension : unnammed struct/union

/////////////////////////////////////////////////////////////////////////////////
#ifndef _WINDOWS_
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Basic declarations
typedef void * HANDLE;
#define DECLARE_HANDLE(name) \
    struct name##__ { int unused; }; typedef struct name##__ * name

/////////////////////////////////////////////////////////////////////////////////
// Timing definitions
typedef struct _SYSTEMTIME {
    unsigned short wYear;
    unsigned short wMonth;
    unsigned short wDayOfWeek;
    unsigned short wDay;
    unsigned short wHour;
    unsigned short wMinute;
    unsigned short wSecond;
    unsigned short wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

typedef struct _FILETIME {
    unsigned long dwLowDateTime;
    unsigned long dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

typedef struct _TIME_ZONE_INFORMATION {
    long Bias;
    wchar_t StandardName[32];
    SYSTEMTIME StandardDate;
    long StandardBias;
    wchar_t DaylightName[32];
    SYSTEMTIME DaylightDate;
    long DaylightBias;
} TIME_ZONE_INFORMATION, *PTIME_ZONE_INFORMATION, *LPTIME_ZONE_INFORMATION;

/////////////////////////////////////////////////////////////////////////////////
// Threading definitions
typedef unsigned long (__stdcall * PTHREAD_START_ROUTINE)( void * );
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;

struct _RTL_CRITICAL_SECTION_DEBUG;
struct _RTL_CRITICAL_SECTION;

#ifndef _WINNT_
typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY * Flink;
   struct _LIST_ENTRY * Blink;
} LIST_ENTRY, *PLIST_ENTRY;
#endif

typedef struct _RTL_CRITICAL_SECTION_DEBUG {
    unsigned short Type;
    unsigned short CreatorBackTraceIndex;
    struct _RTL_CRITICAL_SECTION * CriticalSection;
    LIST_ENTRY ProcessLocksList;
    unsigned long EntryCount;
    unsigned long ContentionCount;
    unsigned long Flags;
    unsigned short CreatorBackTraceIndexHigh;
    unsigned short SpareWORD;
} RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG;

typedef struct _RTL_CRITICAL_SECTION
{
    struct _RTL_CRITICAL_SECTION_DEBUG * DebugInfo;
    long LockCount;
    long RecursionCount;
    HANDLE OwningThread;
    HANDLE LockSemaphore;
    __w64 unsigned long SpinCount;
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION LPCRITICAL_SECTION;

/////////////////////////////////////////////////////////////////////////////////
// GDI+ definitions
typedef __w64 unsigned int WPARAM;
typedef __w64 long LPARAM;
typedef __w64 long LRESULT;

#define CALLBACK __stdcall

DECLARE_HANDLE(HINSTANCE);
DECLARE_HANDLE(HMONITOR);
DECLARE_HANDLE(HWND);
DECLARE_HANDLE(HMENU);
DECLARE_HANDLE(HICON);
typedef HICON HCURSOR;

typedef struct tagPOINT
{
    long x;
    long y;
} POINT, *PPOINT;
typedef struct tagRECT
{
    long left;
    long top;
    long right;
    long bottom;
} RECT, *PRECT;

typedef struct tagWINDOWPLACEMENT {
    unsigned int  length;
    unsigned int  flags;
    unsigned int  showCmd;
    POINT ptMinPosition;
    POINT ptMaxPosition;
    RECT  rcNormalPosition;
} WINDOWPLACEMENT, *PWINDOWPLACEMENT;
typedef PWINDOWPLACEMENT LPWINDOWPLACEMENT;

/////////////////////////////////////////////////////////////////////////////////
#endif // _WINDOWS_
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#ifndef _WINSOCK2API_
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Networking definitions
typedef __w64 unsigned int SOCKET;
#define SOMAXCONN       0x7fffffff

typedef struct sockaddr {
    unsigned short sa_family;
    char sa_data[14];
} SOCKADDR;

typedef struct addrinfo
{
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    unsigned int ai_addrlen;
    char * ai_canonname;
    struct sockaddr * ai_addr;
    struct addrinfo * ai_next;
} ADDRINFOA, *PADDRINFOA;
typedef struct addrinfoW
{
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    unsigned int ai_addrlen;
    wchar_t * ai_canonname;
    struct sockaddr * ai_addr;
    struct addrinfoW * ai_next;
} ADDRINFOW, *PADDRINFOW;

#if ( defined(UNICODE) || defined(_UNICODE) )
    typedef ADDRINFOW ADDRINFOT, *PADDRINFOT;
#else
    typedef ADDRINFOA ADDRINFOT, *PADDRINFOT;
#endif

/////////////////////////////////////////////////////////////////////////////////
#endif // _WINSOCK2API_
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ForwardWin32.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_FORWARDS_FORWARDWIN32_H

