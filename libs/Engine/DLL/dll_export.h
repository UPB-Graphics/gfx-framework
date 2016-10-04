#pragma once

#ifndef _WIN32
	#define DLLExport
#endif

#ifdef _WIN32
	#ifdef ENGINE_DLL_EXPORTS
		#define DLLExport __declspec(dllexport)
	#else
		#define DLLExport __declspec(dllimport)
	#endif

	// Disable the warning C4251 which is triggered by stl classes in
	// Ceres' public interface. To quote MSDN: "C4251 can be ignored "
	// "if you are deriving from a type in the Standard C++ Library"
	#pragma warning( push )
	#pragma warning( disable : 4251 )

	// Disable C4250 - inheritance via dominance
	#pragma warning( disable : 4250 )


	#pragma warning( disable : 4099 )

#endif
