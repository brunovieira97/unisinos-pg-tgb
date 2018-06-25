#pragma once

#include "Shader.h"
#include <GLM/glm.hpp>
#include <GLM/GTC/matrix_transform.hpp>
#include <GLM/GTC/type_ptr.hpp>

class SceneManager {
public:
	SceneManager();

	~SceneManager();

	void initialize(GLuint width, GLuint height);
	void initializeGraphics();

	void addShader(string vFilename, string fFilename);

	// GLFW Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void resize(GLFWwindow* window, int width, int height);

	void doMovement();
	void render();
	void run();
	void finish();

	// Scene
	void setupScene();
	void setupCamera2D();
	void setupTexture();

private:
	GLFWwindow *window;

	// Shader program
	Shader *shader;

	// Scene attributes
	GLuint VAO;

	// Transformations - Model Matrix
	glm::mat4 modelMatrix;

	// 2D Camera - Projection matrix
	glm::mat4 projectionMatrix;

	//Texture index
	unsigned int texture;
};
