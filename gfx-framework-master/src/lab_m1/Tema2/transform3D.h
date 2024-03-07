#pragma once

#include "utils/glm_utils.h"


namespace transform3D
{
    // Translate matrix
    inline glm::mat4 Translate(float tx, float ty, float tz)
    {
        // TODO(student): Implement the translation matrix
        return glm::transpose(
            glm::mat4(1, 0, 0, tx,
                0, 1, 0, ty,
                0, 0, 1, tz,
                0, 0, 0, 1)
        );

    }

    // Scale matrix
    inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
    {
        // TODO(student): Implement the scaling matrix
        return glm::transpose(
            glm::mat4(scaleX, 0, 0, 0,
                0, scaleY, 0, 0,
                0, 0, scaleZ, 0,
                0, 0, 0, 1)
        );

    }

    // Rotate matrix relative to the OZ axis
    inline glm::mat4 RotateOZ(float radians)
    {
        // TODO(student): Implement the rotation matrix
        float degrees = glm::degrees(radians);
        return glm::transpose(
            glm::mat4(glm::cos(degrees), -glm::sin(degrees), 0, 0,
                glm::sin(degrees), glm::cos(degrees), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1)
        );

    }

    // Rotate matrix relative to the OY axis
    inline glm::mat4 RotateOY(float radians)
    {
        // TODO(student): Implement the rotation matrix
        float degrees = glm::degrees(radians);
        return glm::transpose(
            glm::mat4(glm::cos(degrees), 0, glm::sin(degrees), 0,
                0, 1, 0, 0,
                -glm::sin(degrees), 0, glm::cos(degrees), 0,
                0, 0, 0, 1)
        );

    }

    // Rotate matrix relative to the OX axis
    inline glm::mat4 RotateOX(float radians)
    {
        // TODO(student): Implement the rotation matrix
        float degrees = glm::degrees(radians);
        return glm::transpose(
            glm::mat4(1, 0, 0, 0,
                0, glm::cos(degrees), -glm::sin(degrees), 0,
                0, glm::sin(degrees), glm::cos(degrees), 0,
                0, 0, 0, 1)
        );
    }

}   // namespace transform3D
