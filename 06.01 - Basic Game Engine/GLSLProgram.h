#pragma once
#include <string>
using namespace std;
#include <unordered_map>
#include <GL/glew.h>

//This class handles the compilation, linking, and usage of a GLSL shader program.
//Reference: http://www.opengl.org/wiki/Shader_Compilation

	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void addShader(GLenum shaderType, const std::string& shaderPathFileName);
		void compileShaders();
		void addAttribute(const string & attributeName);
		void linkShaders();		
		void use();
		void unuse();
		GLint getUniformLocation(const string & uniformName);
		GLint getAttribLocation(const string & attribName);
	private:
		std::unordered_map<GLenum, string> _listOfShaders;
		std::vector<GLuint> _listOfShaderID;
		GLuint _programID;
		int _numAttributes;
		void compileShader(GLenum shaderType, const std::string& shaderFileName);

	};

