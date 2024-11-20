#version 460 core

layout(binding = 1) uniform CameraData {
    mat4 m4ViewProjection;
};

layout(location = 0) in vec3 v3VertexPos3D;

void main()
{
    gl_Position = m4ViewProjection * vec4(v3VertexPos3D, 1.0f);
}