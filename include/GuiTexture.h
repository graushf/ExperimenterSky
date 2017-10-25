#pragma once

#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"

struct GuiTextureData {
	GLuint VAO;
	GLuint EBO;
	GLuint texture;
	Shader shader;
	int number_vertices;
};

class GuiTexture {
	
public:
	GuiTexture(std::string texture, glm::vec2 position, glm::vec2 scale, Camera* camera);

	std::string getTexture();
	glm::vec2 getPosition();
	glm::vec2 getScale();
	void draw(int viewMode);

private:
	GuiTextureData guiTextureData;
	std::string texture;
	glm::vec2 position;
	glm::vec2 scale;
	Camera* camera;

	void loadToVAO();
	
};