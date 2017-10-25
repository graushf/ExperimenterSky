#pragma once
#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"

struct CloudData {
	GLuint VAO;
	Shader shader;
};

class SkyClouds {
	
public:
	SkyClouds(Camera* camera, int screenWidth, int screenHeight);
	~SkyClouds();
	void draw(int drawingMode);

private:
	CloudData cloudData;
	Camera* camera;
	int screenWidth;
	int screenHeight;

	bool setupSkyCloudsGeom();
};