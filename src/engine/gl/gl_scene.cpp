#include "gl_scene.h"

GLuint glCreateCamera(const Camera& camera) {
    GLuint ui_UBO;

    glGenBuffers(1, &ui_UBO);

    return ui_UBO;
}

void glBindCamera(const Camera& camera, GLuint ui_UBO) {
    glBindBuffer(GL_UNIFORM_BUFFER, ui_UBO);
    mat4 view_projection = camera.asViewProjectionMatrix();
    glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4), &view_projection, GL_DYNAMIC_DRAW);
}
