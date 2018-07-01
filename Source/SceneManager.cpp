#include <Classes/SceneManager.h>

// Static controllers for mouse and keyboard
static bool keys[1024], resized;
static GLuint width, height;

SceneManager::SceneManager() {
}

SceneManager::~SceneManager() {
}

void SceneManager::initialize(GLuint width, GLuint height) {
	::width = width;
	::height = height;
	
	// TO DO: Config file
	initializeGraphics();

	ifstream file("Settings/Tilemap.txt");
	if (!file) {
		cout << "-";
	} else {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				file >> mapLayout[i][j];
				cout << mapLayout[i][j];

				if (mapLayout[i][j] == 2) {
					mapCanWalk[i][j] = 0;
				} else {
					mapCanWalk[i][j] = 1;
				}
			}
			cout << endl;
		}
	}

}

void SceneManager::initializeGraphics() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "Trabalho do GB - Processamento Gráfico", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, keyCallback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress))
		std::cout << "Failed to initialize GLAD." << std::endl;

	// Build and compile our shader program
	addShader();

	setupScene();

	resized = true;
}

void SceneManager::addShader() {
	shader = new Shader ("Shaders/transformations.vs", "Shaders/transformations.frag");
	shaderCharacter = new Shader("Shaders/char.vs", "Shaders/char.frag");
}

void SceneManager::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void SceneManager::resize(GLFWwindow * window, int width, int height) {
	::width = width;
	::height = height;
	resized = true;

	glViewport(0, 0, width, height);
}


void SceneManager::doMovement() {
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void SceneManager::renderMap() {
	// Clear the colorbuffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render scene
	shader->Use();

	float xi = 0.0, yi = 0.0, xo = 500;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			xi = ((i - j) * 64.0f / 2.0) + xo;
			yi = (i + j) * 32.0f / 2.0;

			int currentTile = mapLayout[i][j];

			GLint offsetLoc = glGetUniformLocation(shader->Program, "offsetUV");
			glm::vec2 offset(currentTile/6.0, 0.0);
			glUniform2f(offsetLoc, offset.x,offset.y);

			// Create transformations
			modelMatrix = glm::mat4();
			modelMatrix = glm::translate(modelMatrix, glm::vec3(xi,yi,0.0));

			// Get their uniform location
			GLint modelLoc = glGetUniformLocation(shader->Program, "model");

			// Pass them to the shaders
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

			if (resized) {
				setupCamera2D();
				resized = false;
			}

			// Bind Textures using texture units
			glBindTexture(GL_TEXTURE_2D, texture);
			glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

			// render container
			glBindVertexArray(mapVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		}
	}
}

void SceneManager::renderCharacter() {
	shaderCharacter -> Use();

	// Create transformations 
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0, 0, 1));
	glUniform1f(glGetUniformLocation(shader -> Program, "offsetx"), 0);
	glUniform1f(glGetUniformLocation(shader -> Program, "offsety"), 0);

	GLint modelLoc = glGetUniformLocation(shader->Program, "model");

	// Passes transformations to Shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	if (resized) {
		setupCamera2D();
		resized = false;
	}

	glBindTexture(GL_TEXTURE_2D, characterTexture);
	glUniform1i(glGetUniformLocation(shader -> Program, "texture"), 0);

	// Render container
	glBindVertexArray(charVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void SceneManager::render() {
	renderMap();
	renderCharacter();
}

void SceneManager::run() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		doMovement();

		render();

		glfwSwapBuffers(window);
	}
}

void SceneManager::finish() {
	glfwTerminate();
}

void SceneManager::setupMap() {
	float u = 1.0 / 6.0, v = 1.0;

	float mapWidth = 64.0, mapHeight = 32.0;

	float vertices[] = {
		// Positions							// Colors					// Texture Coords
		mapWidth/2,	mapHeight,		0.0f,		1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// Top 
		mapWidth,	mapHeight/2,	0.0f,		0.0f,	1.0f,	0.0f,		u,		0.0f,		// Right
		0.0f,		mapHeight/2,	0.0f,		0.0f,	0.0f,	1.0f,		0.0f,	v,			// Left
		mapWidth/2,	0.0f,			0.0f,		1.0f,	1.0f,	0.0f,		u,		v			// Bottom
	};
	
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		0, 2, 3  // second triangle
	};

	unsigned int mapVBO, mapEBO;
	glGenVertexArrays(1, &mapVAO);
	glGenBuffers(1, &mapVBO);
	glGenBuffers(1, &mapEBO);

	glBindVertexArray(mapVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mapVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mapEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void SceneManager::setupCharacter() {
	float character[] = {
		 0.125f,	 0.239f, 1.0f,	1.0f, 0.0f, 0.0f,	1.0/4.0,	1.0/2.0,
		 0.125f,	-0.011f, 1.0f,	0.0f, 1.0f, 0.0f,	1.0/4.0,	0.0f,
		-0.125f,	-0.011f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f,		0.0f,
		-0.125f,	 0.239f, 1.0f,	1.0f, 1.0f, 0.0f,	0.0f,		1.0/2.0  
	};
	
	// First line = first triangle and so on
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3 
	};

	unsigned int charVBO, charEBO;

	glGenVertexArrays(1, &charVAO);
	glGenBuffers(1, &charVBO);
	glGenBuffers(1, &charEBO);

	glBindVertexArray(charVAO);

	glBindBuffer(GL_ARRAY_BUFFER, charVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(character), character, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, charEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	setupCharacterTexture();

	glBindVertexArray(0);

}

void SceneManager::setupCharacterTexture() {
	glGenTextures(1, &characterTexture);
	glBindTexture(GL_TEXTURE_2D, characterTexture); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Loads image, creates texture and generates mipmaps
	int charWidth, charHeight, charNrChannels;
	unsigned char *charData = stbi_load("Resources/Character.png", &charWidth, &charHeight, &charNrChannels, 0);

	if (charData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, charWidth, charHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, charData);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load main character texture" << std::endl;
	}
	stbi_image_free(charData);

	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void SceneManager::setupScene() {
	setupMap();
	setupMapTexture();

	setupCharacter();
	//setupCharacterTexture();

}

void SceneManager::setupCamera2D() {
	// Corrects Aspect Ratio
	float aspectRatio;
	float xMin = 0.0, xMax = 750, yMin = 600, yMax = 0.0, zNear = -1.0, zFar = 1.0;
	if (width >= height) {
		aspectRatio = width / (float) height;
		projectionMatrix = glm::ortho(xMin * aspectRatio, xMax * aspectRatio, yMin,	yMax, zNear, zFar);
	} else {
		aspectRatio = height / (float) width;
		projectionMatrix = glm::ortho(xMin, xMax, yMin * aspectRatio, yMax * aspectRatio, zNear, zFar);
	}

	GLint projLoc = glGetUniformLocation(shader->Program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void SceneManager::setupMapTexture() {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); 
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int textureWidth, textureHeight, channelCount;
	unsigned char *data = stbi_load("Resources/Terrain_Tileset.png", &textureWidth, &textureHeight, &channelCount, 0);
	
	cout << "MAP TEXTURE INFO";
	cout << "Channel count: " << channelCount << endl;
	cout << "Width x Height: " << textureWidth << " x " << textureHeight << endl;

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture." << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}