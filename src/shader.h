#ifndef __SHADER_H__
#define __SHADER_H__

unsigned int createShader(const char* vertexFile, const char* fragmentFile);

void detachShader();

void destroyShader(unsigned int shaderID);

#endif