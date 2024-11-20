#version 460 core

layout(location = 0) in vec3 v3VertexPos3D;

void main()
{
    gl_Position = vec4(v3VertexPos3D, 1.0f);
}