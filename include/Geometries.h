#ifndef GEOMETRIES_H
#define GEOMETRIES_H

#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <vector>

#include <cmath>
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#include "Shader.h"
#include "Camera.h"
#include "Utilities.h"
#include "Billboard.h"
#include "Skybox.h"

class Geometries
{
public:
	Camera* camera;
	
	GLuint screenWidth;
	GLuint screenHeight;

	GLuint planeVAO;
	GLuint planeVBO;
	GLuint texCoordsPlaneVBO;
	GLuint texturePlane;
	Shader planeShader;

	/*
	GLuint billboardVAO;
	GLuint billboardVBO;
	GLuint texCoordsBillboardVBO;
	GLuint billboardEBO;
	Shader billboardShader;
	GLuint textureBillboard;
	*/

	GLuint icosahedronVAO;
	GLuint icosahedronVBO;
	GLuint icosahedronEBO;
	Shader icosahedronShader;

	GLuint screenFillingVAO;
	GLuint screenFillingVBO;
	GLuint texCoordsScreenFillTextureVBO;
	GLuint screenFillingEBO;
	Shader skyShader;
	Shader skyShaderAdv;
	GLuint textureScreenFill;

	Utilities* utils;

	std::vector<Billboard*> billboardListDay;
	std::vector<Billboard*> billboardListNight;


	float distanceBillboard = 7;


	

	// SKY PARAMETERS
	bool isNightSky = false;
	bool isNightTransition = false;

	float gamma = 0.0;

	float gamma1 = 0.0;
	float gamma2 = 0.0;

	float sunAltitude;
	float sunAzimuth;
	glm::vec3 sunPos;

	float moonAltitude;
	float moonAzimuth;
	glm::vec3 moonPos;

	Skybox* skybox;
	//


	Geometries(Camera* camera, GLuint screenWidth, GLuint screenHeight, Utilities* utilities);
	void updateCameraObject(Camera* camera);

	void setPlaneGeom();
	void renderPlaneGeom();
	void createPlaneShader();

	void createIcosahedronGeom();
	void renderIcosahedronGeom(glm::vec3 position, float distance, glm::vec3 color);
	void createIcosahedronShader();

	//void createBillboardGeom(char* texturebillboardpath);
	//void renderBillboardGeom(int option, glm::vec3 position, float distanceBillboard);
	//void createBillboardShader();

	void createScreenFillingRectangle();
	void renderSkyOnScreen();
	void createSkyShader();
	void createSkyShaderAdv();
	void renderSkyOnScreenAdv(int drawingMode);

	void initBillboards();

	// SKY
	void updateSunPosition();
	void updateMoonPosition();

	void updateGamma();

	void updateGamma1();
	void updateGamma2();

private:
	void renderBillboardsSky();

};



#endif /* GEOMETRIES_H */