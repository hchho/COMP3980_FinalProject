/**
 * Platform.h
 * (c) 2004 - 2006 SkyeTek, Inc. All Rights Reserved.
 *
 * Platform definitions.
 */
#ifndef SKYETEK_PLATFORM_H
#define SKYETEK_PLATFORM_H

/* Define which devices to use */
#define STAPI_DEMO 1
#define STAPI_SPI 1
#define STAPI_SERIAL 1
#define STAPI_USB 1


#if defined(WIN32) || defined(WINCE)

#if _MSC_VER > 1000
#pragma once
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#define SKYETEK_DEVICE_FILE HANDLE
#define vsnprintf _vsnprintf

#else

#include <unistd.h>
#include <stdint.h>

#define SKYETEK_DEVICE_FILE int
#define SKYETEK_API
#define SKYETEK_CPP_API

#endif

#if defined(WIN32) || defined(WINCE)
#ifdef SKYETEK_EXPORTS
#define SKYETEK_API __declspec(dllexport)
#else
#define SKYETEK_API __declspec(dllimport)
#endif
#ifdef SKYETEK_CPP_EXPORTS
#define SKYETEK_CPP_API __declspec(dllexport)
#else
#define SKYETEK_CPP_API __declspec(dllimport)
#endif
#endif

#if defined(WIN32) || defined(WINCE)
	#define MUTEX(m) CRITICAL_SECTION m
	#define MUTEX_CREATE(m) InitializeCriticalSection(m)
	#define MUTEX_DESTROY(m) DeleteCriticalSection(m)
	#define MUTEX_LOCK(m) EnterCriticalSection(m)
	#define MUTEX_UNLOCK(m) LeaveCriticalSection(m)
#elif defined(HAVE_PTHREAD)
	#define MUTEX(m) pthread_mutex_t m
	#define MUTEX_CREATE(m) pthread_mutex_init(m, NULL)
	#define MUTEX_DESTROY(m) pthread_mutex_destroy(m)
	#define MUTEX_LOCK(m) pthread_mutex_lock(m)
	#define MUTEX_UNLOCK(m) pthread_mutex_unlock(m)
#else
	#define MUTEX(m)
	#define MUTEX_CREATE(m)
	#define MUTEX_DESTROY(m)
	#define MUTEX_LOCK(m)
	#define MUTEX_UNLOCK(m)
#endif

#if defined(WIN32) || defined(WINCE)
typedef unsigned char  UINT8; 
typedef unsigned short UINT16; 
typedef short INT16;

#else

#include <inttypes.h>

#ifndef UINT32
typedef uint32_t UINT32;
#endif
#ifndef UINT16
typedef uint16_t UINT16;
#endif
#ifndef UINT8
typedef uint8_t UINT8;
#endif
#ifndef UINT64
#ifndef WIN32 
typedef uint64_t UINT64;
#else
typedef unsigned __int64 UINT64;
#endif
#endif
#endif
  
union _16Bits 
{ 
	UINT16  w; 
	UINT8   b[2]; 
}; 
  
union _32Bits 
{ 
	UINT32  l; 
	UINT16  w[2]; 
	UINT8   b[4]; 
};

union _64Bits
{
	UINT64	ll;
	UINT32	l[2];
	UINT16	w[4];
	UINT8	b[8];
};


#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#define SKYETEK_Sleep(x) 	Sleep(x)
#else
#define SKYETEK_Sleep(x)	usleep(x*1000)
#endif


#ifndef WIN32
#include <string.h>

#define max(a,b) (a > b) ? a : b
#define _T(s) s

#define _tcscpy strcpy
#define _tcscmp strcmp
#define _tcslen strlen
#define _tcsstr strstr
#define _stprintf sprintf
#define _fgetts fgets
#define _tcstok strtok
#define _ttoi atoi
#define _tfopen fopen
#define _tcstoul strtoul
#define _vsntprintf snprintf
#define _tcsncpy strncpy

typedef char TCHAR;
typedef TCHAR* LPTSTR;
#endif


#ifdef __cplusplus
}
#endif



#endif 
