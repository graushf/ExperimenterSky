#pragma once

#include <GL/glew.h>
#include <vector>
#include <SOIL/SOIL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utilities.h"
#include "Shader.h"
#include "Camera.h"

class Skybox 
{
private:
	Camera* camera;
	GLuint screenWidth;
	GLuint screenHeight;
	Utilities* utils;

public:
	
	GLuint skyboxTexture;
	GLuint skyboxVAO;
	Shader skyboxShader;

	Skybox(Camera* camera, GLuint screenWidth, GLuint screenHeight, Utilities* utils);
	void loadCubemap(std::vector<const GLchar*> faces);
	void createSkyboxGeom();
	void createSkyboxShader();
	void renderSkybox();
};