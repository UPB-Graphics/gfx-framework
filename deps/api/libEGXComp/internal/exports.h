#ifndef LIBEGXCOMP_EXPORTS_H
#define LIBEGXCOMP_EXPORTS_H

#if defined(_WIN32)
#	ifdef LIBEGXCOMP_EXPORTS
#		define LIBEGXCOMP_API __declspec(dllexport)
#	else
#		define LIBEGXCOMP_API __declspec(dllimport)
#	endif
#else
#	define LIBEGXCOMP_API
#endif

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable: 4251)
#	pragma warning(disable: 4273)
#endif

#endif
