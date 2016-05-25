#pragma once
//Third-party libraries
#include <GL/glew.h>			//The OpenGL Extension Wrangler
#include <glm/glm.hpp>			//OpenGL Mathematics 

struct Position {
	GLfloat  x, y, z; 
};

struct ColorRGBA8 {
	ColorRGBA8() :r(0), g(0), b(0), a(0) {	}
	ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) : r(R), g(G), b(B), a(A)  {}
	GLubyte r, g, b, a;
};
struct UV {
	GLfloat u, v;
};

struct NormalVector {
	GLfloat nx, ny, nz;
};

struct Vertex {
	Position position;
	ColorRGBA8 color;
	UV uv;
	NormalVector normal;
	void setPosition(GLfloat  x, GLfloat  y, GLfloat  z) {
		position.x = x;
		position.y = y;
		position.z = z;
	}
	glm::vec3 getPosition() {
		return glm::vec3 (position.x, position.y, position.z); 
	}
	
	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}

	void setUV(GLfloat  u, GLfloat  v) {
		uv.u = u;
		uv.v = v;
	}

	void setNormalVector(GLfloat  x, GLfloat  y, GLfloat  z) {
		normal.nx = x;
		normal.ny = y;
		normal.nz = z;
	}

};