#include "primitives.h"

#include <iostream>
#include <fmt/core.h>

using namespace glm;

// glGenBuffers(1, &g_uiVBO); // find 1 unused id to create a buffer, g_uiVBO_id = glGenBuffers(1)
// glBindBuffer(GL_ARRAY_BUFFER, g_uiVBO); //actually create the buffer, of type GL_ARRAY_BUFFER
// glBufferData(GL_ARRAY_BUFFER, sizeof(fVertexData), fVertexData, GL_STATIC_DRAW); // add the data

//VBO = Vertex Buffer Object, data of where the vertices are
//IBO = Index Buffer Object, data of where the triangles are (connecting the vertices in groups of 3)
GLsizei Primitives::glCreateCube(SceneData& scene_data)
{
    CustomVertex VertexData[] = {
        { vec3(0.5f,  0.5f, -0.5f) },
        { vec3(0.5f, -0.5f, -0.5f) },
        { vec3(-0.5f, -0.5f, -0.5f) },
        { vec3(-0.5f,  0.5f, -0.5f) },
        { vec3(-0.5f, -0.5f,  0.5f) },
        { vec3(-0.5f,  0.5f,  0.5f) },
        { vec3(0.5f, -0.5f,  0.5f) },
        { vec3(0.5f,  0.5f,  0.5f) }
    };

    GLuint uiIndexData[] = {
        0, 1, 3, 3, 1, 2,  // Create back face
        3, 2, 5, 5, 2, 4,  // Create left face
        1, 6, 2, 2, 6, 4,  // Create bottom face
        5, 4, 7, 7, 4, 6,  // Create front face
        7, 6, 0, 0, 6, 1,  // Create right face
        7, 0, 5, 5, 0, 3   // Create top face
    };

    GLuint ui_VAO;
    GLuint ui_VBO;
    GLuint ui_IBO;
    

    glGenVertexArrays(1, &ui_VAO);
    glGenBuffers(1, &ui_VBO);
    glGenBuffers(1, &ui_IBO);

    //Write to this VAO
    glBindVertexArray(ui_VAO);

    // Fill Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, ui_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);

    // Fill Index Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uiIndexData), uiIndexData, GL_STATIC_DRAW);

    // Specify location of data within buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CustomVertex), (const GLvoid *)0);
    glEnableVertexAttribArray(0);

    int num_indices = sizeof(uiIndexData) / sizeof(GLuint);
    scene_data.mp_Meshes.emplace_back(ui_VAO, ui_VBO, ui_IBO, num_indices);
    return num_indices;
}