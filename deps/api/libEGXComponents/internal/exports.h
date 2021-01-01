#ifndef LIBEGXC_EXPORTS_H
#define LIBEGXC_EXPORTS_H

#if defined(_WIN32)
#	ifdef LIBEGXC_EXPORTS
#		define LIBEGXC_API __declspec(dllexport)
#	else
#		define LIBEGXC_API __declspec(dllimport)
#	endif
#else
#	define LIBEGXC_API
#endif

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable: 4251)
#	pragma warning(disable: 4273)
#endif

#endif
