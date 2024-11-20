#pragma once

#include <glm/glm.hpp>

struct CameraData
{
    // float m_fAngleX;
    // float m_fAngleY;
    // float m_fMoveZ;
    // float m_fMoveX;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up_vec; // which direction is up on the camera
    float fov; //in degrees, 45 degrees usually
    float aspect;
    float near;
    float far;
};

class Camera {
    CameraData camera_data;

    public:
        Camera(CameraData camera_data) : camera_data(camera_data) {};

        glm::mat4 asViewProjectionMatrix() const;

    private:
        glm::mat4 asViewMatrix() const;
        glm::mat4 asProjectionMatrix() const;

};

