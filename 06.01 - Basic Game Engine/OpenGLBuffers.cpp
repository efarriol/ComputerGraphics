#include "OpenGLBuffers.h"


/*
* Constructor
*/
OpenGLBuffers::OpenGLBuffers() :gVAO(0), gVBO(0)
{
	
}

/*
* Destructor
*/
OpenGLBuffers::~OpenGLBuffers()
{
	//Always remember to delete your buffers when you are done!
	if (gVAO != 0) {
		glDeleteVertexArrays(1, &gVAO);
	}

	if (gVBO != 0) {
		glDeleteBuffers(1, &gVBO);
	}
}

/*
* Prepare the buffers based on the information that has to be rendered
*/
void OpenGLBuffers::initializeBuffers(GLSLProgram & _colorProgram) {
		//Create VAO and VBO
	allocateVAOVBO();
		//Initialize the VAO content
	initializeVertexArrayObject(_colorProgram);
}


/*
* Create VAO and VBO. This step must be done outside of the constructor because the OpenGL environment must be previously initialized
*/
void OpenGLBuffers::allocateVAOVBO() {
	//Generate the VAO if it isn't already generated
	//This is for preventing a memory leak if someone calls twice the initializeVertexArrayObject method
	if (gVAO == 0) {
		glGenVertexArrays(1, &gVAO);
	}

	//Generate the VBO if it isn't already generated
	//This is for preventing a memory leak if someone calls twice the init method
	if (gVBO == 0) {
		glGenBuffers(1, &gVBO);
	}
}

/*
* Create the Vertex Array Object
* @param _coloProgram is the object that manages the data related to the shaders
*/
void OpenGLBuffers::initializeVertexArrayObject(GLSLProgram & _colorProgram) {	

		// Bind the VAO. All subsequent opengl calls will modify it's state.
	glBindVertexArray(gVAO);

		//Bind the buffer object. YOU MUST BIND the buffer vertex object before binding attributes
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	
		//Connect the xyz to the "vertexPosition" attribute of the vertex shader
	glEnableVertexAttribArray(_colorProgram.getAttribLocation("vertexPosition"));
		//Connect the rgba to the "vertexColor" attribute of the vertex shader
	//glEnableVertexAttribArray(_colorProgram.getAttribLocation("vertexColor"));

	//Connect theuv to the "vertexUV" attribute out of the vertex shader
	glEnableVertexAttribArray(_colorProgram.getAttribLocation("vertexUV"));
	//Connect theuv to the "vertexNormal" attribute out of the vertex shader
	glEnableVertexAttribArray(_colorProgram.getAttribLocation("vertexNormal"));


	//Point Opengl to the data in our VBO
	/* The vertexPosition attribute refers to the 3D position
	The first argument is the shader variable that the data should be sent to
	The second argument, 3, says that each vertex has three numbers
	The third argument, GL_FLOAT, says that the three numbers are GLfloats
	The fourth argument, GL_FALSE, says that we do not want the floats to be "normalized." If they were normalized, they would be restricted to having a minimum of zero, and a maximum of one. We don't want that restriction on our points, which is why this argument is false.
	The fifth argument, sizeof(Vertex), says that the information in the buffer vertex object will be composed by elements of the type Vertex
	The sixth argument, (void*)offsetof(Vertex, position), says where starts this kind of information in the vertex buffer object
	*/
	glVertexAttribPointer(_colorProgram.getAttribLocation("vertexPosition"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex, position));
	

	/* The vertexColor attribute refers to the color in RGBA components
	The first argument is the shader variable that the data should be sent to
	The second argument, 4, says that the color contains 4 numbers
	The third argument, GL_UNSIGNED_BYTE, says that the  numbers are GLubyte
	The fourth argument, GL_TRUE, allows to normalize values between 0 and 1
	The fifth argument, sizeof(Vertex), says that the information in the buffer vertex object will be composed by elements of the type Vertex
	The sixth argument, (void*)offsetof(Vertex, color), says where starts this kind of information in the vertex buffer object
	*/
	//glVertexAttribPointer(_colorProgram.getAttribLocation("vertexColor"), 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	//The vertexUV attribute refers to the uv positions of the texture
	glVertexAttribPointer(_colorProgram.getAttribLocation("vertexUV"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glVertexAttribPointer(_colorProgram.getAttribLocation("vertexNormal"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// unbind the VAO and VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);

}




/*
* Send the information to the GPU
* @param data is an array that contains a set of vertices
* @param numVertices is the number of vertices to render
*/
void OpenGLBuffers::sendDataToGPU(Vertex * data, int numVertices) {
	//Bind the vertex array object
	glBindVertexArray(gVAO);

	//Bind the vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	// Put all the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), data, GL_DYNAMIC_DRAW);

	//Draw a set of elements(numVertices) from the VBO as GL_TRIANGLES. The first vertex is in the 0th position

	if(numVertices == 4 ) glDrawArrays(GL_QUADS, 0, numVertices);
	else glDrawArrays(GL_TRIANGLES, 0, numVertices);

	//Unbind the VBO and VAO
	glBindVertexArray(0);	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}
