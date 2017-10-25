#pragma once


#include <iostream>
#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"

struct CloudModel2Ddata {
	GLuint VAO;
	GLuint EBO;
	GLuint textureBuffer;
	Shader shader;
};

class CloudModel2D {

public:
	CloudModel2D(Camera* camera, float screenWidth, float screenHeight);
	~CloudModel2D();

	void draw(float Height, int viewMode);
	void updateCameraObject(Camera* camera);
private:
	CloudModel2Ddata cloudModelData;
	Camera* camera;
	float screenWidth;
	float screenHeight;

	bool setupCloudPlaneData();
};