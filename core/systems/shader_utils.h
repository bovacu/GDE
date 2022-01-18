// Created by borja on 18/1/22.


#ifndef ENGINE2_0_SHADER_UTILS_H
#define ENGINE2_0_SHADER_UTILS_H


#ifndef _CREATE_SHADER_H
#define _CREATE_SHADER_H
#include <GL/glew.h>
char* file_read(const char* filename);
void print_log(GLuint object);
GLuint create_shader(const char* filename, GLenum type);
GLuint create_program(const char* vertexfile, const char *fragmentfile);
GLuint create_gs_program(const char* vertexfile, const char *geometryfile, const char *fragmentfile, GLint input, GLint output, GLint vertices);
GLint get_attrib(GLuint program, const char *name);
GLint get_uniform(GLuint program, const char *name);
#endif


#endif //ENGINE2_0_SHADER_UTILS_H
