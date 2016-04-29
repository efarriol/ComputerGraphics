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
	up = false;
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
	_gameElements.loadGameElements("./resources/scene2D.txt");
		//Load the character AABB
	AABBOne = _gameElements.getAABB(0);

	start = time(0);
	previousTime = 0;
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
	_colorProgram.addAttribute("vertexColor");
		//Link the compiled shaders
	_colorProgram.linkShaders();
		//Bind the uniform variables. You must enable shaders before gettting the uniforme variable location
	_colorProgram.use();
	modelMatrixUniform = _colorProgram.getUniformLocation("modelMatrix");
	_colorProgram.unuse();
}

/*
* Game execution: Gets input events, processes game logic and draws sprites on the screen
*/
void Game::gameLoop() {	
	_gameState = GameState::PLAY;
	while (_gameState != GameState::EXIT) {	
			//Start synchronization between refresh rate and frame rate
		_fpsLimiter.startSynchronization();
			//Process the input information (keyboard and mouse)
		processInput();
			//Execute the player actions (keyboard and mouse)
		executePlayerCommands();
			//Update the game status
		doPhysics();
			//Draw the objects on the screen
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
void Game::executePlayerCommands() {
	Camera camera(WIDTH, HEIGHT, _screenType);
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)){
		glm::ivec2 mouseCoords = _inputManager.getMouseCoords();
		cout << mouseCoords.x << ", " << mouseCoords.y << endl;
	}

	if (_inputManager.isKeyDown(SDLK_w)) {
		(_gameElements.getGameElement(0))._translate = (_gameElements.getGameElement(0))._translate - glm::vec3(0, 0.05, 0);
		AABBOne._centre = (_gameElements.getGameElement(0))._translate;
		
	}

	/*if (_inputManager.isKeyDown(SDLK_a)) {
		(_gameElements.getGameElement(0))._translate = (_gameElements.getGameElement(0))._translate - glm::vec3(-0.01, 0, 0);
		AABBOne._centre = (_gameElements.getGameElement(0))._translate;
	}*/
	if (_inputManager.isKeyDown(SDLK_s)) {
		(_gameElements.getGameElement(0))._translate = (_gameElements.getGameElement(0))._translate - glm::vec3(0, -0.05, 0);
		AABBOne._centre = (_gameElements.getGameElement(0))._translate;
	}
	if (_inputManager.isKeyDown(SDLK_UP)) {
		up = true;
	}

	/*if (_inputManager.isKeyDown(SDLK_d)) {
		(_gameElements.getGameElement(0))._translate = (_gameElements.getGameElement(0))._translate - glm::vec3(0.01, 0, 0);
		AABBOne._centre = (_gameElements.getGameElement(0))._translate;
	}*/
	if (_inputManager.isKeyDown(SDLK_p)) {
		_screenType = PERSP_CAM;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (_inputManager.isKeyDown(SDLK_o)) {
		_screenType = ORTHO_CAM;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (_inputManager.isKeyDown(SDLK_i)) {
		_screenType = AUTO_CAM;
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
		/*(_gameElements.getGameElement(1))._angle = (_gameElements.getGameElement(1))._angle + 5;
		(_gameElements.getGameElement(1))._rotation.z = 1.0f;
		(_gameElements.getGameElement(4))._angle = (_gameElements.getGameElement(4))._angle + 1;
		(_gameElements.getGameElement(4))._rotation.x = 1.0f;
		(_gameElements.getGameElement(4))._rotation.y = 1.0f;*/
		seconds_since_start = difftime(time(0), start);
		if (seconds_since_start - previousTime >= 1) {
			velocity += 0.001;
			previousTime = seconds_since_start;
			score++;
		}
		for (int i = 1; i <= _gameElements.getGameElement(0)._maxCars; i++) {
			_gameElements.getGameElement(i)._translate.x += velocity;
			_gameElements.getAABB(i)._centre.x += velocity;
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
					cout << j << endl;
					cout << endl << "************************" << endl;
					cout << "GAME OVER" << endl;
					cout << "FINAL SCORE: " << score << endl;
					cout << "************************" << endl;
					system("Pause");
					_gameState = GameState::EXIT;
					 }

			}
		}
	}
	for (int i = 1; i <= _gameElements.getGameElement(0)._maxCars; i++) {
		if (_gameElements.getGameElement(i)._translate.x >= 4) {
			std::random_device rd;
			std::mt19937 eng(rd());
			std::uniform_real_distribution<> dis(-0.7, 0.7);
			int collisionID = 0;
			for (int a = 1; a <= i; a++) {
				if (_gameElements.getGameElement(a)._collisionType == 1 || _gameElements.getGameElement(a)._collisionType == 2) collisionID++;
			}
			_gameElements.getGameElement(i)._translate.x = -4;
			_gameElements.getAABB(collisionID)._centre.x = _gameElements.getGameElement(i)._translate.x;
			_gameElements.getGameElement(i)._translate.y = dis(eng);
			_gameElements.getAABB(collisionID)._centre.y = _gameElements.getGameElement(i)._translate.y;
		}
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
	//For each one of the elements: Each object MUST BE RENDERED based on its position, rotation and scale data
	for (int i = 0; i < _gameElements.getNumGameElements(); i++) {	
		currentRenderedGameElement = _gameElements.getGameElement(i);	
		glm::mat4 modelMatrix;
	
		modelMatrix = glm::translate(modelMatrix, currentRenderedGameElement._translate);
		if (currentRenderedGameElement._angle != 0) {
			modelMatrix = glm::rotate(modelMatrix, glm::radians(currentRenderedGameElement._angle), currentRenderedGameElement._rotation);
		}
		modelMatrix = glm::scale(modelMatrix, currentRenderedGameElement._scale);
		
		//glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
			//Send data to GPU
		
		if(up)_camera.setPosition(glm::vec3(_camera.getPosition().x + 0.1f, _camera.getPosition().y, _camera.getPosition().z));

		GLuint modelMatrixUnifrom = _colorProgram.getUniformLocation("modelMatrix");
		glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		GLuint viewMatrixUniform = _colorProgram.getUniformLocation("viewMatrix");
		glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, glm::value_ptr(_camera.viewMatrix()));
		GLuint projectionMatrixUniform = _colorProgram.getUniformLocation("projectionMatrix");
		glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, glm::value_ptr(_camera.projectionMatrix()));
		_openGLBuffers.sendDataToGPU(_gameElements.getData(currentRenderedGameElement._objectType), _gameElements.getNumVertices(currentRenderedGameElement._objectType));

	}

	//Unbind the program
	_colorProgram.unuse();

	//Swap the display buffers (displays what was just drawn)
	_window.swapBuffer();
} 