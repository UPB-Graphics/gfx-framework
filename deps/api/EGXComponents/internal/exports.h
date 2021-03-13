#ifndef EGXC_EXPORTS_H
#define EGXC_EXPORTS_H

#if defined(_WIN32)
#    ifdef EGXC_EXPORTS
#        define EGXC_API __declspec(dllexport)
#    else
#        define EGXC_API __declspec(dllimport)
#    endif
#else
#    define EGXC_API
#endif

#if defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable: 4251)
#    pragma warning(disable: 4273)
#endif

#endif
