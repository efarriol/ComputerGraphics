#include "Game.h"

/**
* Constructor
* Note: It uses an initialization list to set the parameters
* @param windowTitle is the window title
* @param screenWidth is the window width
* @param screenHeight is the window height
*/
Game::Game(std::string windowTitle, int screenWidth, int screenHeight, bool enableLimiterFPS, int maxFPS, bool printFPS) :
	_windowTitle(windowTitle), 
	_screenWidth(screenWidth), 
	_screenHeight(screenHeight),
	_gameState(GameState::INIT), 
	_fpsLimiter(enableLimiterFPS, maxFPS, printFPS) {
	_screenType = 1;
	srand(time(0));
	velocity = 0.01f;
	score = 0;
	cameraPosIncrement = 0.0f;
	up = false;
	cameraType = 0;
}

/**
* Destructor
*/
Game::~Game()
{
}


/*
* Game execution
*/
void Game::run() {
		//System initializations
	initSystems();
		//Start the game if all the elements are ready
	gameLoop();
}

/*
* Initializes all the game engine components
*/
void Game::initSystems() {
		//Create an Opengl window using SDL
	_window.create(_windowTitle, _screenWidth, _screenHeight, 0);		
		//Compile and Link shader
	initShaders();
		//Set up the openGL buffers
	_openGLBuffers.initializeBuffers(_colorProgram);
		//Load the current scenario
	_gameElements.loadBasic3DObjects();
	_gameElements.firstRender = true;

	_gameElements.loadGameElements("./resources/scene2D.txt");
	loadGameTextures();
		//Load the character AABB
	AABBOne = _gameElements.getAABB(0);

	start = time(0);
	previousTime = 0;
	_gameState = GameState::MENU;
}

void Game::loadGameTextures() {
	GameObject currentGameObject;

	for (int i = 0; i < _gameElements.getNumGameElements(); i++) {
		currentGameObject = _gameElements.getGameElement(i);
		if(_gameElements.getGameElement(i)._texturedObject) _gameElements.getGameElement(i)._textureID = _textureManager.getTextureID(currentGameObject._textureFile);
	}
}


/*
* Initialize the shaders:
* Compiles, sets the variables between C++ and the Shader program and links the shader program
*/
void Game::initShaders() {
		//Compile the shaders
	_colorProgram.addShader(GL_VERTEX_SHADER, "./resources/shaders/vertex-shader.txt");
	_colorProgram.addShader(GL_FRAGMENT_SHADER, "./resources/shaders/fragment-shader.txt");
	_colorProgram.compileShaders();
		//Attributes must be added before linking the code
	_colorProgram.addAttribute("vertexPositionGame");
	//_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.addAttribute("vertexNormal");
		//Link the compiled shaders
	_colorProgram.linkShaders();
		//Bind the uniform variables. You must enable shaders before gettting the uniforme variable location
	_colorProgram.use();
	//_lightTypeUniform = _colorProgram.getUniformLocation("light.type");
	modelMatrixUniform = _colorProgram.getUniformLocation("modelMatrix");
	viewMatrixUniform = _colorProgram.getUniformLocation("viewMatrix");
	projectionMatrixUniform = _colorProgram.getUniformLocation("projectionMatrix");
	_drawModeUniform = _colorProgram.getUniformLocation("drawMode");
	_viewerPositionUniform = _colorProgram.getUniformLocation("viewerPosition");
//	_newColorUniform = _colorProgram.getUniformLocation("objectColor");
	_textureDataLocation = _colorProgram.getUniformLocation("textureData");
	_textureScaleFactorLocation = _colorProgram.getUniformLocation("textureScaleFactor");
	modelNormalMatrixUniform = _colorProgram.getUniformLocation("modelNormalMatrix");
	_isALightSourceUniform = _colorProgram.getUniformLocation("isALightSource");
	_lightingEnabledUniform = _colorProgram.getUniformLocation("lightingEnabled");
	_materialAmbientUniform = _colorProgram.getUniformLocation("material.ambient");
	_materialDiffuseUniform = _colorProgram.getUniformLocation("material.diffuse");
	_materialSpecularUniform = _colorProgram.getUniformLocation("material.specular");
	_materialShininessUniform = _colorProgram.getUniformLocation("material.shininess");
	_lightAmbientUniform = _colorProgram.getUniformLocation("light.ambient");
	_lightDiffuseUniform = _colorProgram.getUniformLocation("light.diffuse");
	_lightSpecularUniform = _colorProgram.getUniformLocation("light.specular");
	_linearUniform = _colorProgram.getUniformLocation("light.linear");
	_constantUniform = _colorProgram.getUniformLocation("light.constant");
	_quadraticUniform = _colorProgram.getUniformLocation("light.quadratic");
	//_lightShininessUniform = _colorProgram.getUniformLocation("light.shininess");


	_lightPositionUniform = _colorProgram.getUniformLocation("lightPosition");
	_colorProgram.unuse();
	
}

/*
* Game execution: Gets input events, processes game logic and draws sprites on the screen
*/
void Game::gameLoop() {	
	while (_gameState != GameState::EXIT) {	
			//Start synchronization between refresh rate and frame rate
		_fpsLimiter.startSynchronization();
			//Process the input information (keyboard and mouse)
		processInput();
			//Execute the GAMEer actions (keyboard and mouse)
		executeGAMECommands();
			//Update the game status
		if(_gameState == GameState::GAME)doPhysics();
			//Draw the objects on the screen
		//if (_gameState == GameState::MENU && !menuRendered) renderMenu();
		renderGame();	
			//Force synchronization
		_fpsLimiter.forceSynchronization();
	}
}

/*
* Processes input with SDL
*/
void Game::processInput() {
	_inputManager.update();
	//Review https://wiki.libsdl.org/SDL_Event to see the different kind of events
	//Moreover, table show the property affected for each event type
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		default:
			break;
		}
	}

}


/**
* Executes the actions sent by the user by means of the keyboard and mouse
*/
void Game::executeGAMECommands() {
	Camera camera(WIDTH, HEIGHT, _screenType);
	if (_gameState == GameState::GAME) {
		if (_inputManager.isKeyDown(SDLK_w) || _inputManager.isKeyDown(SDLK_UP)) {
			(_gameElements.getGameElement(0))._translate = (_gameElements.getGameElement(0))._translate - glm::vec3(0, 0.05, 0);
			AABBOne._centre = (_gameElements.getGameElement(0))._translate;

		}
		if (_inputManager.isKeyDown(SDLK_s) || _inputManager.isKeyDown(SDLK_DOWN)) {
			(_gameElements.getGameElement(0))._translate = (_gameElements.getGameElement(0))._translate - glm::vec3(0, -0.05, 0);
			AABBOne._centre = (_gameElements.getGameElement(0))._translate;
		}
		if (_inputManager.isKeyPressed(SDLK_p)) {
			if (cameraType == 0) {
				_screenType = PERSP_CAM;
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				cameraType = 1;
			}
			else if (cameraType == 1) {
				_screenType = ORTHO_CAM;
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				cameraType = 2;
			}
			else {
				_screenType = AUTO_CAM;
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				cameraType = 0;
			}
		}
	}
	else if(_gameState == GameState::MENU) {
		if (_inputManager.isKeyPressed(SDLK_1)) {
			_gameState = GameState::GAME;
		}
		if (_inputManager.isKeyPressed(SDLK_2)) {
			_gameState = GameState::EXIT;
		}
	}
	else if (_gameState == GameState::LOSE){
		if (_inputManager.isKeyPressed(SDLK_r)) {
			_gameElements.firstRender = false;
			_gameElements.loadGameElements("./resources/scene2D.txt");
			loadGameTextures();
			AABBOne = _gameElements.getAABB(0);
			start = time(0);
			previousTime = 0;
			velocity = 0.01f;
			score = 0;
			_gameState = GameState::GAME;
		}
		if (_inputManager.isKeyPressed(SDLK_e)) {
			_gameState = GameState::EXIT;
		}
	}
}

/*
* Update the game objects based on the physics
*/
void Game::doPhysics() {
	static int counter = 0;
	float distance = 1;
	AABB AABBTwo;
	ComputeCollision computeCollision;
	
	if (counter == 1) {
		seconds_since_start = difftime(time(0), start);
		if (seconds_since_start - previousTime >= 1) {
			velocity += 0.001f;
			cameraPosIncrement += 0.45f;
			score++;
			previousTime = seconds_since_start;
		}

		for (int i = 0; i < _gameElements.getNumGameElements(); i++) {
			if (_gameElements.getGameElement(i)._objectType == 3) {
				_gameElements.getGameElement(i)._translate.x += velocity;
				_gameElements.getAABB(i)._centre.x += velocity;
			}
		}
		counter = 0;
	}
	counter++;
	
	for (int j = 1; j < _gameElements.getNumGameElements(); j++) {
		if (_gameElements.getGameElement(j)._collisionType == 1 || _gameElements.getGameElement(j)._collisionType == 2) {
			int collisionID = 0;
			for (int a = 1; a <= j; a++) {
				if (_gameElements.getGameElement(a)._collisionType == 1 || _gameElements.getGameElement(a)._collisionType == 2) collisionID++;
			}
			AABBTwo = _gameElements.getAABB(collisionID);
			if (glm::length(AABBTwo._centre - AABBOne._centre) < distance) {

				if (computeCollision.computeCollisionAABB(AABBOne, AABBTwo)) { 
					cout << endl << "YOU LOSE"<< endl << "SCORE:" << score << endl;
					_gameState = GameState::LOSE;
					 }
			}
		}
	}
	for (int i = 1; i <= _gameElements.getGameElement(0)._maxCars; i++) {
		if (_gameElements.getGameElement(i)._translate.x >= 4) {
			std::random_device rd;
			std::mt19937 eng(rd());
			std::uniform_real_distribution<> dis(-0.7f, 0.7f);
			int collisionID = 0;
			for (int a = 1; a <= i; a++) {
				if (_gameElements.getGameElement(a)._collisionType == 1 || _gameElements.getGameElement(a)._collisionType == 2) collisionID++;
			}
			_gameElements.getGameElement(i)._translate.x = -4.0f;
			_gameElements.getAABB(collisionID)._centre.x = _gameElements.getGameElement(i)._translate.x;
			_gameElements.getGameElement(i)._translate.y = dis(eng);
			_gameElements.getAABB(collisionID)._centre.y = _gameElements.getGameElement(i)._translate.y;
		}
	}
	if (_gameElements.getGameElement(0)._translate.y >= 1.05f) {
		_gameElements.getGameElement(0)._translate.y = 1.049f;
	}
	if (_gameElements.getGameElement(0)._translate.y <= -0.8f) {
		_gameElements.getGameElement(0)._translate.y = -0.79f;
	}
}

/**
* Draw the sprites on the screen
*/
void Game::renderGame() {
	//Temporal variable
	GameObject currentRenderedGameElement;
	Camera _camera(WIDTH, HEIGHT, _screenType);
	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Bind the GLSL program. Only one code GLSL can be used at the same time
	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);

	//For each one of the elements: Each object MUST BE RENDERED based on its position, rotation and scale data
	//_camera.setPosition(glm::vec3(_camera.getPosition().x - cameraPosIncrement, _camera.getPosition().y, _camera.getPosition().z));
	if (_screenType == AUTO_CAM) _camera.setFront(glm::vec3(_gameElements.getGameElement(0)._translate.x, _gameElements.getGameElement(0)._translate.y, _gameElements.getGameElement(0)._translate.z));
		
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, glm::value_ptr(_camera.viewMatrix()));
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, glm::value_ptr(_camera.projectionMatrix()));
	
	for (int i = 0; i < _gameElements.getNumGameElements(); i++) {	
		currentRenderedGameElement = _gameElements.getGameElement(i);	
		glm::mat4 modelMatrix;
		glm::mat3 modelNormalMatrix;
		modelMatrix = glm::translate(modelMatrix, currentRenderedGameElement._translate);
		if (currentRenderedGameElement._angle != 0) {
			modelMatrix = glm::rotate(modelMatrix, glm::radians(currentRenderedGameElement._angle), currentRenderedGameElement._rotation);
		}
		modelMatrix = glm::scale(modelMatrix, currentRenderedGameElement._scale);
		
		modelNormalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));

			//Texture
		if  (currentRenderedGameElement._texturedObject) {
			glBindTexture(GL_TEXTURE_2D, currentRenderedGameElement._textureID);
			glUniform1i(_drawModeUniform, 1);	
		}
		else glUniform1i(_drawModeUniform, 0);
		
		//Material
		if (currentRenderedGameElement._materialType == "LIGHT") {
			glUniform1i(_isALightSourceUniform, 1);
			if (currentRenderedGameElement._lightEnable) glUniform1i(_lightingEnabledUniform, 1);
			else glUniform1i(_lightingEnabledUniform, 0);
			currentMaterial.type = 0;
			glUniform3fv(_lightPositionUniform, 1, glm::value_ptr(currentRenderedGameElement._translate));
			currentMaterial.ambient = glm::vec3 (5.0f);
			glUniform3fv(_lightAmbientUniform, 1, glm::value_ptr(currentMaterial.ambient));
			glUniform3fv(_lightDiffuseUniform, 1, glm::value_ptr(currentMaterial.diffuse));
			glUniform3fv(_lightSpecularUniform, 1, glm::value_ptr(currentMaterial.specular));
			glUniform1f(_linearUniform, 1.0f);
			glUniform1f(_constantUniform, 0.0014f);
			glUniform1f(_quadraticUniform, 0.000007f);
		//	glUniform1i(_lightTypeUniform, currentMaterial.type);

			//glUniform1f(_lightShininessUniform, currentMaterial.shininess);
		}
		else glUniform1i(_isALightSourceUniform, 0);
		
		currentMaterial = _materialManager.getMaterialComponents(_materialManager.getMaterialID(currentRenderedGameElement._materialType));
		glUniform3fv(_materialAmbientUniform, 1, glm::value_ptr(currentMaterial.ambient));
		glUniform3fv(_materialDiffuseUniform, 1, glm::value_ptr(currentMaterial.diffuse));
		glUniform3fv(_materialSpecularUniform, 1, glm::value_ptr(currentMaterial.specular));
		glUniform1f(_materialShininessUniform, currentMaterial.shininess);
		glUniform3fv(_viewerPositionUniform, 1, glm::value_ptr(_camera.getPosition()));

		//glUniform4fv(_newColorUniform, 1, glm::value_ptr(currentRenderedGameElement._color));
		glUniform1i(_textureDataLocation, 0);		//This line is not needed if we use only 1 texture, it is sending the GL_TEXTURE0		
		if (currentRenderedGameElement._textureRepetion) {
			glUniform2f(_textureScaleFactorLocation, currentRenderedGameElement._scale.x, currentRenderedGameElement._scale.y);
		}
		else {
			glUniform2f(_textureScaleFactorLocation, 1.0f, 1.0f);
		}
		
		glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(modelNormalMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelNormalMatrix));
				//Send data to GPU
		if(_gameState==GameState::MENU && currentRenderedGameElement._renderType == "MENU")
			_openGLBuffers.sendDataToGPU(_gameElements.getData(currentRenderedGameElement._objectType), _gameElements.getNumVertices(currentRenderedGameElement._objectType));	
		else if (_gameState == GameState::GAME && currentRenderedGameElement._renderType == "GAME")
			_openGLBuffers.sendDataToGPU(_gameElements.getData(currentRenderedGameElement._objectType), _gameElements.getNumVertices(currentRenderedGameElement._objectType));
		else if (_gameState == GameState::LOSE && (currentRenderedGameElement._renderType == "GAME" || currentRenderedGameElement._renderType == "LOSE"))
			_openGLBuffers.sendDataToGPU(_gameElements.getData(currentRenderedGameElement._objectType), _gameElements.getNumVertices(currentRenderedGameElement._objectType));
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	//Unbind the program
	_colorProgram.unuse();

	//Swap the disGAME buffers (disGAMEs what was just drawn)
	_window.swapBuffer();
}