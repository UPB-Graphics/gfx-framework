#ifndef LIBEGC_EXPORTS_H
#define LIBEGC_EXPORTS_H

#if defined(_WIN32)
#	ifdef LIBEGC_EXPORTS
#		define LIBEGC_API __declspec(dllexport)
#	else
#		define LIBEGC_API __declspec(dllimport)
#	endif
#else
#	define LIBEGC_API
#endif

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable: 4251)
#	pragma warning(disable: 4273)
#endif

#endif
