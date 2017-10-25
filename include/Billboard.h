#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL/SOIL.h>

#include "Shader.h"
#include "Camera.h"

class Billboard {

public:

Camera* camera;
GLuint screenWidth;
GLuint screenHeight;

GLuint billboardVAO;
GLuint billboardVBO;
GLuint texCoordsBillboardVBO;
GLuint billboardEBO;
Shader billboardShader;
GLuint textureBillboard;

glm::vec3 _position;
glm::vec3 _scale;
float _distance;

Billboard(Camera* camera, float screenWidth, float screenHeight, glm::vec3 position, glm::vec3 scale, float distance, char* texturebillboardpath, char* vertexShaderSrc, char* fragmentShaderSrc);

void createBillboardGeom(char* texturebillboardpath);
void renderBillboardGeom();
void renderBillboardGeom(glm::vec3 scale, glm::vec3 position, float distance);
void createBillboardShader(char* vertexShaderSrc, char* fragmentShaderSrc);

void updatePositionBillboard(glm::vec3 position);
//void updateScaleBillboard(glm::vec3 scale);
//void updateDistanceBillboard(glm::vec3 distance);

};

