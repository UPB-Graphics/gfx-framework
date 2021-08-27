#ifndef GFXC_EXPORTS_H
#define GFXC_EXPORTS_H

#if defined(_WIN32)
#    ifdef GFXC_EXPORTS
#        define GFXC_API __declspec(dllexport)
#    else
#        define GFXC_API __declspec(dllimport)
#    endif
#else
#    define GFXC_API
#endif

#if defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable: 4251)
#    pragma warning(disable: 4273)
#endif

#endif
