#pragma once


//Third-party libraries
#include <GL/glew.h>			//The OpenGL Extension Wrangler
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>			//OpenGL Mathematics 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Window.h"
#include "GLSLProgram.h"
#include "FPSLimiter.h"
#include "OpenGLBuffers.h"
#include "Vertex.h"
#include "Geometry.h"
#include "InputManager.h"
#include "Constants.h"
#include "Camera.h"
#include "ComputeCollision.h"
#include <time.h>
#include <random> 
#include "TextureManager.h"


//Game has four possible states: INIT (Preparing environment), PLAY (Playing), EXIT (Exit from the game) or MENU (Game menu)
enum class GameState{INIT, PLAY, EXIT, MENU};

//This class manages the game execution
class Game {
	public:						
		Game(std::string windowTitle, int screenWidth, int screenHeight, bool enableLimiterFPS, int maxFPS, bool printFPS);	//Constructor
		~Game();					//Destructor
		void run();					//Game execution
		time_t start;
		double seconds_since_start;
		double previousTime;
		float velocity;
		int score;
		float cameraPosIncrement;
		int cameraType;
			
	private:
			//Attributes	

		std::string _windowTitle;		//Window Title
		int _screenWidth;				//Screen width in pixels				
		int _screenHeight;				//Screen height in pixels	
		int _screenType;				//Screen type Persp/Ortho
		GameState _gameState;			//It describes the game state				
		Window _window;					//Manage the OpenGL context
		GLSLProgram _colorProgram;		//Manage the shader programs
		FPSLimiter _fpsLimiter;			//Manage the synchronization between frame rate and refresh rate
		OpenGLBuffers _openGLBuffers;	//Manage the openGL buffers
		TextureManager _textureManager;
		Geometry _gameElements;			//Manage the game elements
		InputManager _inputManager;		//Manage the input devices
		AABB AABBOne;
		material currentMaterial;
		MaterialManager _materialManager;
		GLuint modelMatrixUniform;
		GLuint viewMatrixUniform;
		GLuint projectionMatrixUniform;
		GLuint modelNormalMatrixUniform;

		GLuint _linearUniform;
		GLuint _constantUniform;
		GLuint _quadraticUniform;
		GLuint _viewerPositionUniform;
		GLuint _lightPositionUniform;
		GLuint _materialAmbientUniform;
		GLuint _materialDiffuseUniform;
		GLuint _materialSpecularUniform;
		GLuint _materialShininessUniform;
		GLuint _lightAmbientUniform;
		GLuint _lightDiffuseUniform;
		GLuint _lightSpecularUniform;
		GLuint _lightTypeUniform;
		//GLuint _lightShininessUniform;
		GLuint _isALightSourceUniform;
		GLuint _lightingEnabledUniform;
		GLuint _drawModeUniform;
		GLuint _newColorUniform;
		GLint _textureDataLocation;
		GLint _textureScaleFactorLocation;
			//Internal methods
		void initSystems();
		void loadGameTextures();
		void initShaders();		
		void gameLoop();
		void processInput();
		void doPhysics();
		void executePlayerCommands();
		void renderGame();	
		bool up;
		
};

