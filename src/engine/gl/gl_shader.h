#pragma once

#include <GL/glew.h>

bool GL_LoadShader(GLuint & uiShader, GLenum ShaderType, const GLchar * p_cShader);
bool GL_LoadShaders(GLuint & uiShader, GLuint uiVertexShader, GLuint uiFragmentShader);
bool GL_LoadShaderFile(GLuint & uiShader, GLenum ShaderType, const char * p_cFileName, int iFileID);
