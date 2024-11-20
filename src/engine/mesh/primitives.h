#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../gl/gl_scene.h"
// #include <math.h>

class Primitives {
    private:
    
        struct CustomVertex
        {
            glm::vec3 v3Position;
        };

    public:
        enum PrimitiveType {
            CUBE
        };

        //create and write to scene_data
        static GLsizei glCreateCube(SceneData& data);
};
