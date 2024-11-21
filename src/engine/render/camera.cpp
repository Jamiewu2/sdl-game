#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

//Public fn's

mat4 Camera::asViewProjectionMatrix() const {
    mat4 view = asViewMatrix();
    mat4 projection = asProjectionMatrix();
    
    return projection * view;
}

void Camera::translate(vec3 translation) {
    this->camera_data.position += translation;
}

//Private fn's

mat4 Camera::asViewMatrix() const {
    return lookAt(
        camera_data.position,
        camera_data.position + camera_data.direction,
        camera_data.up_vec
    );
}

mat4 Camera::asProjectionMatrix() const {
    return perspective(
        radians(camera_data.fov),
        camera_data.aspect,
        camera_data.near,
        camera_data.far
    );
}
