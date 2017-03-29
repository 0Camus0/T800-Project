/*********************************************************
* Copyright (C) 2017 Daniel Enriquez (camus_mm@hotmail.com)
* All Rights Reserved
*
* You may use, distribute and modify this code under the
* following terms:
* ** Do not claim that you wrote this software
* ** A mention would be appreciated but not needed
* ** I do not and will not provide support, this software is "as is"
* ** Enjoy, learn and share.
*********************************************************/

#include <utils\Utils.h>
#include <stdio.h>    
#include <stdlib.h>

#ifdef USING_OPENGL_ES
void checkcompilederrors(GLuint shader, GLenum type) {
	GLint bShaderCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &bShaderCompiled);
	if (!bShaderCompiled) {
		int i32InfoLogLength, i32CharsWritten;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);

		char* pszInfoLog = new char[i32InfoLogLength];
		glGetShaderInfoLog(shader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);
		char* pszMsg = new char[i32InfoLogLength + 256];
		if (type == GL_FRAGMENT_SHADER) {
			sprintf(pszMsg, "Failed to compile pixel shader: %s", pszInfoLog);
		}
		else if (type == GL_VERTEX_SHADER) {
			sprintf(pszMsg, "Failed to compile vertex shader: %s", pszInfoLog);
		}
		else {
			sprintf(pszMsg, "Failed to compile wtf shader: %s", pszInfoLog);
		}
		printf("%s", pszMsg);
		delete[] pszMsg;
		delete[] pszInfoLog;
	}
}

GLuint createShader(GLenum type, char* pSource) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char**)&pSource, NULL);
	glCompileShader(shader);
	checkcompilederrors(shader, type);
	return shader;
}
#endif

char *file2string(const char *path) {
	FILE *fd;
	long len, r;
	char *str;
	if (!(fd = fopen(path, "r"))) {
		fprintf(stderr, "Can't open file '%s'\n", path);
		return NULL;
	}
	fseek(fd, 0, SEEK_END);
	len = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	str = (char*)malloc( 1 + len * sizeof(char));
	r = fread(str, sizeof(char), len, fd);
	str[r] = '\0';
	fclose(fd);
	return str;
}