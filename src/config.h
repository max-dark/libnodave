#ifndef CONFIG_H
#define CONFIG_H

#define NO_PRINT_CODE // disable logger
//#define LINENUMBERS // enable line numbers in logs
#define DUMPLIMIT 11132

#define DONT_USE_GETHOSTBYNAME
#define DAVE_HAVE_MEMCPY  // normally we have

//#define DEBUG_CALLS	// Define this and recompile to get parameters and results
// of each function call printed. I could have made this an
// option bit in daveDebug, but most applications will never,never
// need it. Still they would have to do the jumps...
// This option is just useful when developing bindings to new
// programming languages.

#if defined(_WIN32) || defined(__WIN32__)
#define OS_WINDOWS

#ifndef DONT_USE_GETHOSTBYNAME
#define DONT_USE_GETHOSTBYNAME
#endif // !DONT_USE_GETHOSTBYNAME

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif // _MSC_VER

//#define DO_EXPORT // for nodave.dll build
//#define DO_IMPORT // for link libnodave.dll

//#define HAVE_S7ONLINE // for s7onlinx.dll support

#if defined(DO_EXPORT)
#define EXPORTSPEC __declspec (dllexport)
#elif defined(DO_IMPORT)
#define EXPORTSPEC __declspec (dllimport)
#else
// static link(default)
#define EXPORTSPEC
#endif

#define CALL_SPEC __stdcall

#else

#if defined(HAVE_S7ONLINE)
#warning "HAVE_S7ONLINE for windows only. Disabled"
#undef HAVE_S7ONLINE
#endif // defined(HAVE_S7ONLINE)

#endif // Windows

#if defined(LINUX) || defined(__linux__) || defined(__linux) || defined(linux)
#define OS_LINUX
#define EXPORTSPEC
#define CALL_SPEC

#define HAVE_UNISTD
#define HAVE_SELECT
#endif // Linux

#if defined(DOS)
#define OS_DOS

#define CALL_SPEC __cedcl
#define EXPORTSPEC

#endif // DOS

#if defined(AVR)
#define CALL_SPEC
#define EXPORTSPEC

#define HAVE_UNISTD

#endif // AVR

#if defined(OS_WINDOWS) || defined(OS_LINUX) || defined(OS_DOS) || defined(AVR)
// get rid of nested ifdefs.
#define API_KNOWN
#else
#error "Unknow OS/compiler"
#endif

#endif // !CONFIG_H

/*
Changes:
	2018-02-14 * API detection moved into this file.
	2018-02-16 + new define HAVE_S7ONLINE - enable/disable s7onlinx.dll support(windows only)
				default == disabled
*/
