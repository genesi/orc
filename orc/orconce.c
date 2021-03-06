

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <orc/orconce.h>
#include <orc/orcdebug.h>

#if defined(HAVE_THREAD_PTHREAD)

#include <pthread.h>

static pthread_mutex_t once_mutex = PTHREAD_MUTEX_INITIALIZER;

void
_orc_once_init (void)
{
  
}

void
orc_once_mutex_lock (void)
{
  pthread_mutex_lock (&once_mutex);
}

void
orc_once_mutex_unlock (void)
{
  pthread_mutex_unlock (&once_mutex);
}

#elif defined(HAVE_THREAD_WIN32)

#include <windows.h>

static CRITICAL_SECTION once_mutex;

void
_orc_once_init (void)
{
}

void
orc_once_mutex_lock (void)
{
  EnterCriticalSection (&once_mutex);
}

void
orc_once_mutex_unlock (void)
{
  LeaveCriticalSection (&once_mutex);
}

#ifdef _MSC_VER

#pragma section(".CRT$XCU",read)

static void __cdecl
orc_once_cs_init (void)
{
  InitializeCriticalSection (&once_mutex);
}

__declspec(allocate(".CRT$XCU"))
void (__cdecl * orc_once_cs_init_constructor)(void) = orc_once_cs_init;

#elif defined(__GNUC__)

static void orc_once_cs_init (void) __attribute__((constructor));

static void
orc_once_cs_init (void) 
{
  InitializeCriticalSection (&once_mutex);
}

#else
#error Expecting GCC or MSVC on Windows
#endif

#if 0
BOOL WINAPI
DllMain (HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
  if (dwReason == DLL_PROCESS_ATTACH) {
    InitializeCriticalSection (&once_mutex);
  }
  return TRUE;
}
#endif

#else

void
_orc_once_init (void)
{
}

void
orc_once_mutex_lock (void)
{
}

void
orc_once_mutex_unlock (void)
{
}

#endif


