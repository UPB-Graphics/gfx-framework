#pragma once

#include <ostream>

#include "utils/math_utils.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"


inline glm::vec3 NormalizedRGB(unsigned char r, unsigned char g, unsigned char b) {
    return glm::vec3(r / 255.f, g / 255.f, b / 255.f);
}


/**
 *  overload ostream operator << for easy printing glm classes
 */
inline std::ostream &operator<< (std::ostream &out, const glm::ivec2 &vec) {
    out << "[" << vec.x << " " << vec.y << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::vec2 &vec) {
    out << "[" << vec.x << " " << vec.y << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::ivec3 &vec) {
    out << "[" << vec.x << " " << vec.y << " " << vec.z << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
    out << "[" << vec.x << " " << vec.y << " " << vec.z << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::ivec4 &vec) {
    out << "[" << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec) {
    out << "[" << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::quat &rot) {
    out << "[" << rot.x << " " << rot.y << " " << rot.z << " " << rot.w << "]";
    return out;
}


namespace glm
{
    using namespace std;

    const glm::vec3 vec3_up = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 vec3_down = glm::vec3(0.0f, -1.0f, 0.0f);
    const glm::vec3 vec3_left = glm::vec3(-1.0f, 0.0f, 0.0f);
    const glm::vec3 vec3_right = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 vec3_forward = glm::vec3(0.0f, 0.0f, 1.0f);
    const glm::vec3 vec3_backward = glm::vec3(0.0f, 0.0f, -1.0f);

    inline glm::quat AxisAngle(const float xx, const float yy, const float zz, const float angle360)
    {
        float t = float(RADIANS(angle360) / 2.0);

        // Precompute the sin(theta/2) once for optimization
        float sinT = sin(t);

        // Calculate the x, y and z of the quaternion
        float x = xx * sinT;
        float y = yy * sinT;
        float z = zz * sinT;
        float w = cos(t);        // W value by cos(theta/2)

        return glm::quat(w, x, y, z);
    }

    // Convert a quaternion to axis angle
    inline glm::vec4 GetAxisAngle(glm::quat &rotation, int precision = 0)
    {
        float angle = acos(rotation.w);

        if (angle == 0)
        {
            return glm::vec4(1, 0, 0, 0);
        }

        auto t = sqrt(1 - rotation.w * rotation.w);

        if (precision)
        {
            auto x = round(rotation.x / t * precision) / precision;
            auto y = round(rotation.y / t * precision) / precision;
            auto z = round(rotation.z / t * precision) / precision;
            return glm::vec4(x, y, z, round(DEGREES(angle)));
        }

        return glm::vec4(rotation.x / t, rotation.y / t, rotation.z / t, round(DEGREES(angle)));
    }
}
