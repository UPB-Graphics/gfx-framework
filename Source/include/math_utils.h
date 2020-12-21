#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

inline float lerp(float v0, float v1, float t) {
	return v0 + (v1 - v0) * t;
}

#ifndef MAX
#define MAX(a, b) ((a > b) ? a : b)
#endif
#ifndef MIN
#define MIN(a, b) ((a < b) ? a : b)
#endif

#define UPPER_BOUND(a, b) ((a + b - 1) / b)

#define RADIANS(angle)		(angle * 0.0174532925194444f)	// a * M_PI / 180
#define TO_RADIANS			0.0174532925194444f				// M_PI / 180

#define DEGREES(radians)	(radians * 57.29577951308233f)
#define TO_DEGREES			57.29577951308233f				// 180 / M_PI
