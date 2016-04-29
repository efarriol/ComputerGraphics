#include "GLSLProgram.h"
#include <fstream>
#include <vector>
#include "ErrorManagement.h"


/* Constructor: Initialize all the ID
*/
GLSLProgram::GLSLProgram() :_programID(0), _numAttributes(0){		
}

/* Destructor
*/
GLSLProgram::~GLSLProgram()
{
	
}

/*
* Add a shader into the queu for being compiled
*/
void GLSLProgram::addShader(GLenum shaderType, const std::string& shaderPathFileName) {
	_listOfShaders[shaderType] = shaderPathFileName;
}

//Adds an attribute to our shader. Should be called between compiling and linking.
void GLSLProgram::addAttribute(const string & attributeName) {
	//Bind the variables used in the GLSL source code with our program data
	//Parameters: 
	//1st parameter: _programID is the program we are compiling and linking
	//2nd parameter: attributeID is the attribute of the GLSL source code.
	//3rd parameter: name of the attribute as a char *				
	glBindAttribLocation(_programID, _numAttributes, attributeName.c_str());
	_numAttributes++;
}

//enable the shader, and all its attributes
void GLSLProgram::use() {
	glUseProgram(_programID);
	for (int i = 0; i < _numAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}

//disable the shader
void GLSLProgram::unuse() {
	glUseProgram(0);
	for (int i = 0; i < _numAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}

//Compiles the shaders into a form that your GPU can understand
void GLSLProgram::compileShaders() {	
	GLenum shaderType;
	string shaderPathFileName;
	GLint shaderID;

		//Get a program object.
	_programID = glCreateProgram();

		//Compile each shader that you intend to use
	for (auto it = _listOfShaders.begin(); it != _listOfShaders.end(); it++) {			
			//Get the first shader
		shaderType = it->first;
		shaderPathFileName=it->second;

			//Create the shader object, and store its ID
		shaderID = glCreateShader(shaderType);
		if (shaderID == 0) {
			ErrorManagement::errorRunTime(shaderPathFileName + " failed to be created!");
		}
			//Compile each shader
		compileShader(shaderID, shaderPathFileName);
		_listOfShaderID.push_back(shaderID);
	}					
}

//Compiles a single shader file
void GLSLProgram::compileShader(GLuint id, const string & filePath) {
	//Open, read, send to openGl and compile the GLSL source code for the vertex shader
	//Open the file
	ifstream shaderFile(filePath);
	if (shaderFile.fail())	{
		//Get additional information related to the error
		perror(filePath.c_str());
		ErrorManagement::errorRunTime("Failed to open " + filePath);
	}

	//File contents stores all the text in the file
	string fileContents = "";
	//line is used to grab each line of the file
	string line;
	//Get all the lines in the file and add it to the contents
	while (getline(shaderFile, line)) {
		fileContents += line + "\n";
	}
	//Close the file
	shaderFile.close();

	//Send to code to OpenGL.
	//Parameters:
	//Object that will store the shader: id
	//Number of strings that contain the code:1 
	//Array of lines that contains the code. We only have one string with everything so we pass an array with one c_str
	//The length​ can be either NULL or an array of count​ integers. These are the lengths of the corresponding strings in the string​ array. This allows you to use non-NULL-terminated strings. If you pass NULL, then OpenGL will assume all of the strings are NULL-terminated and will therefore compute the length in the usual way.
	//get a pointer to our file contents c string;
	const char *contentsPtr = fileContents.c_str();
	//tell opengl that we want to use fileContents as the contents of the shader file
	glShaderSource(id, 1, &contentsPtr, nullptr);
	//Compile the shader
	glCompileShader(id);

	//Check for errors
	GLint isCompiled = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE) 	{
		//Get the error message. First we get the length and next we read the content
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		// Don't leak the shader.
		glDeleteShader(id);

		// Exit with failure.		
		printf("%s\n", &(errorLog[0]));
		ErrorManagement::errorRunTime("Shader " + filePath + " failed to compile");
	}
	// Shader compilation is successful.
}

void GLSLProgram::linkShaders() {
	//Shaders are successfully compiled.
	//Now time to link them together into a program.

		//Attach our shaders to our program
	for (unsigned int i = 0; i < _listOfShaderID.size(); i++) {
		glAttachShader(_programID, _listOfShaderID[i]);
	}
			

	//Link our program
	glLinkProgram(_programID);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(_programID);
		//Don't leak shaders either.
		for (unsigned int i = 0; i < _listOfShaderID.size(); i++) {
			//Don't leak shaders either.
			glDeleteShader(_listOfShaderID[i]);
		}
			
		// Exit with failure.		
		printf("%s\n", &(errorLog[0]));
		ErrorManagement::errorRunTime("Shaders failed to link");
	}

		
	for (unsigned int i = 0; i < _listOfShaderID.size(); i++) {
			//Always detach shaders after a successful link.
		glDetachShader(_programID, _listOfShaderID[i]);
			//Don't leak shaders either.
		glDeleteShader(_listOfShaderID[i]);
	}
}

//Get the value of a uniform value from the GLSL program
GLint GLSLProgram::getUniformLocation(const string & uniformName) {
	GLint location = glGetUniformLocation(_programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX) {
		ErrorManagement::errorRunTime("Uniform " + uniformName + " not found in shader!");
	}
	return location;
}

//Get the position of a variable from the GLSL program
GLint GLSLProgram::getAttribLocation(const string & attribName) {
	GLint attrib = glGetAttribLocation(_programID, attribName.c_str());
	if (attrib == GL_INVALID_INDEX)
		ErrorManagement::errorRunTime("Attribute " + attribName + " not found in shader!");
			
	return attrib;
}

