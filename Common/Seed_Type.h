#ifndef _SEED_TYPE_H_
#define _SEED_TYPE_H_

#include <iostream>

namespace Seed {

	typedef int8_t		int8;
	typedef uint8_t		uint8;
	typedef int16_t		int16;
	typedef uint16_t	uint16;
	typedef int32_t		int32;
	typedef uint32_t	uint32;
	typedef int64_t		int64;
	typedef uint64_t	uint64;
	typedef float		float32;
	typedef double		float64;

#define SEED_SUCCESS	0
#define SEED_FAILURE	1

    #if defined(_MSC_VER)
        typedef wchar_t             wchar;
        typedef __int64             int64;
        typedef unsigned __int64    uint64;
        typedef __time32_t          time_t32;
        typedef __time64_t          time_t64;
    #else
        typedef signed long long    int64;
        typedef unsigned long long  uint64;
        typedef unsigned short      wchar;
        typedef int                 BOOL;
        typedef int                 SOCKET;
        typedef char                CHAR;
        typedef unsigned char       BYTE;
        typedef short               SHORT;
        typedef unsigned short      WORD;
        typedef int                 INT;
        typedef unsigned int        UINT;
        typedef unsigned int        UINT_PTR;
        typedef unsigned long       DWORD;
        typedef unsigned long       DWORD_PTR;
        typedef int64               INT64;
        typedef uint64              UINT64;
        typedef int64               LONGLONG;
        typedef uint64              ULONGLONG;
        typedef long                LONG;
        typedef long                LONG_PTR;
        typedef unsigned long       ULONG;
        typedef float               FLOAT;
        typedef double              DOUBLE;
        typedef unsigned char       UCHAR;
        typedef unsigned short      USHORT;
        typedef const char*		    LPCSTR;
        typedef const char*		    PCSTR;
        typedef const char*		    LPCTSTR;
        typedef void                VOID;
        typedef void*			    LPVOID;
        typedef void*			    PVOID;
        typedef void*			    HANDLE;
        typedef PVOID               HANDLE;
        typedef HANDLE              HWND;
        typedef DWORD               COLORREF;
        typedef char                TCHAR;
        typedef UINT_PTR            WPARAM;
        typedef LONG_PTR            LPARAM;
        typedef LONG_PTR            LRESULT;
        typedef int32               time_t32;
        typedef int64               time_t64;

        typedef struct sockaddr_in              SOCKADDR_IN;
        typedef struct sockaddr*                LPSOCKADDR;
    #endif

    #if !defined(TRUE)
        #define TRUE              1
    #endif
    
    #if !defined(FALSE)
        #define FALSE             0
    #endif
    
    #if !defined(NULL)
        #define NULL              0
    #endif
    
    #if !defined(DONE)
        #define DONE              0
    #endif

    #if defined(__linux__)
        #define CONST            const
        #define MAX_PATH         256
        #define WINAPI      
        #define E_INVALIDARG
        #define _In_	
        #define _Out_
        #define _Inout_	
        #define _In_z_

        #define _atoi64(s)          atoll(s)
        #define _tcslen(s)          strlen(s)
        #define _tcsncpy(d,s,n)     strncpy(d,s,n)
        #define _tcscpy(d,s)        strcpy(d,s)
        #define _tcscmp(s1,s2)      strcmp(s1,s2)
        #define _tcsncmp(s1,s2,n)   strncmp(s1,s2,n)
        #define _tcstol(n,e,b)      strtol(n,e,b)
        #define _tcstod(n,e)        strtod(n,e)
        #define _tcstok(s,d)        strtok(s,d)
        #define _itot(v,s,r)        sprintf(s,"%d",v)
        #define _ltot(v,s,r)        sprintf(s,"%ld",v)
        #define _T(s)               s
        #define _istalnum(c)        isalnum(c)
        #define _stprintf           sprintf
        #define _ttoi               atoi
        #define _stscanf            sscanf
        #define _tcscspn            strcspn
        #define _tstof              atof
        #define _tcstok_s(s,d,p)    strtok(s,d)
        #define _tmain              main
        #define _tfopen             fopen
        #define _tfopen_s           fopen_s

        #define RGB(r,g,b)                              ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
        #define ASSERT(f)	
        #define ZeroMemory(Destination,Length)          memset((Destination),0,(Length))
        #define CopyMemory(Destination,Source,Length)   memcpy((Destination),(Source),(Length))
        #define ATLENSURE(expr)	
        #define ATLASSUME(expr)	
        #define AtlThrow(x)
        #define AfxMessageBox(x)

        #define MAKEWORD(a, b)   ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
        #define MAKELONG(a, b)   ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
        #define LOWORD(l)        ((WORD)(((DWORD_PTR)(l)) & 0xffff))
        #define HIWORD(l)        ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
        #define LOBYTE(w)        ((BYTE)(((DWORD_PTR)(w)) & 0xff))
        #define HIBYTE(w)        ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))

        #define INVALID_SOCKET                (SOCKET)(~0)
        #define SD_BOTH                       (2)
        #define closesocket(x)                (shutdown(x, SD_BOTH), close(x))
        #define ioctlsocket                   ioctl
        #define SOCKET_ERROR                  (-1)

        #define CRITICAL_SECTION              pthread_mutex_t
        #define EnterCriticalSection(x)       pthread_mutex_lock(x)
        #define LeaveCriticalSection(x)       pthread_mutex_unlock(x)
        #define InitializeCriticalSection(x)  pthread_mutex_init(x,NULL)
        #define DeleteCriticalSection(x)      pthread_mutex_destroy(x)

        #define VIX_DEPRECATED  __attribute__((__deprecated__))
    #else
        #if defined(_NOT_USE_MFC)
        #define SD_BOTH                       (2)
        #endif
        #define VIX_DEPRECATED  __declspec(deprecated)
    #endif

	typedef enum doc_type_e {
		DOC_TYPE_JSON = 1,
		DOC_TYPE_INI,
		DOC_TYPE_BINARY,
		DOC_TYPE_MAX,
	} DOC_TYPE_E;
}

#endif