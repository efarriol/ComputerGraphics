#pragma once
//Third-party libraries
#include <GL/glew.h>			//The OpenGL Extension Wrangler
#include <glm/glm.hpp>			//OpenGL Mathematics 
#include <iostream>
#include "GLSLProgram.h"
#include <cstddef>
#include "Vertex.h"

class OpenGLBuffers {
	GLuint gVAO;			//Vertex Array Object
	GLuint gVBO;			//Vertex Buffer Object


public:
	OpenGLBuffers();
	~OpenGLBuffers();
	void initializeBuffers(GLSLProgram & _colorProgram);
	void allocateVAOVBO();
	void initializeVertexArrayObject(GLSLProgram & _colorProgram);
	void sendDataToGPU(Vertex * data, int numVertex);
};

