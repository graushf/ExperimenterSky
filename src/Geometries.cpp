#include "Geometries.h"

Geometries::Geometries(Camera* camera, GLuint screenWidth, GLuint screenHeight, Utilities* utilities)
{
	Geometries::camera = camera;
	Geometries::screenWidth = screenWidth;
	Geometries::screenHeight = screenHeight;
	utils = utilities;

	sunAltitude = -30;
	sunAzimuth = 0.0;

	moonAltitude = 90; 
	moonAzimuth = 180.0;

	skybox = new Skybox(camera, screenWidth, screenHeight, utilities);
	//Billboard billboard = Billboard(camera, screenWidth, screenHeight, "./resources/cloud.png", "./shaders/shaderBillboard.vertex", "./shaders/shaderBillboard.frag");
	//billboardList.push_back(billboard);
}

void Geometries::initBillboards() {
	//billboardListDay[i]->renderBillboardGeom(glm::vec3(2.0, 2.0, 2.0), sunPos, 5.0);
	Billboard* billboard = new Billboard(camera, screenWidth, screenHeight, sunPos, glm::vec3(5.0, 5.0, 5.0), 30.0, "./resources/sun_texture.png", "./shaders/shaderBillboard.vertex", "./shaders/shaderBillboard.frag");
	Billboard* billboard2 = new Billboard(camera, screenWidth, screenHeight, utils->generateRandomVector(), glm::vec3(2.0, 2.0, 2.0), utils->rand_FloatRange(1.0, 3.0), "./resources/cloud.png", "./shaders/shaderBillboard.vertex", "./shaders/shaderBillboard.frag");
	Billboard* billboard3 = new Billboard(camera, screenWidth, screenHeight, utils->generateRandomVector(), glm::vec3(2.0, 2.0, 2.0), utils->rand_FloatRange(1.0, 3.0), "./resources/cloud_2.png", "./shaders/shaderBillboard.vertex", "./shaders/shaderBillboard.frag");
	Billboard* billboard4 = new Billboard(camera, screenWidth, screenHeight, utils->generateRandomVector(), glm::vec3(0.5, 0.5, 0.5), utils->rand_FloatRange(3.0, 6.0), "./resources/cloud_3.png", "./shaders/shaderBillboard.vertex", "./shaders/shaderBillboard.frag");
	
	billboardListDay.push_back(billboard);
	billboardListDay.push_back(billboard2);
	billboardListDay.push_back(billboard3);
	billboardListDay.push_back(billboard4);

	Billboard* billboard5 = new Billboard(camera, screenWidth, screenHeight, moonPos, glm::vec3(3.0, 3.0, 3.0), 20.0, "./resources/moon.png", "./shaders/shaderBillboard.vertex", "./shaders/shaderBillboard.frag");
	float _aux = utils->rand_FloatRange(0.5, 1.5);
	Billboard* billboard6 = new Billboard(camera, screenWidth, screenHeight, utils->generateRandomVector(), glm::vec3(_aux, _aux, _aux), utils->rand_FloatRange(3.0, 9.0), "./resources/star2.png", "./shaders/shaderBillboard.vertex", "./shaders/shaderBillboard.frag");

	billboardListNight.push_back(billboard5);
	billboardListNight.push_back(billboard6);

	utils->initRandomDataBillboards(200);
}

void Geometries::setPlaneGeom()
{
	GLfloat verticesPlane[] = {
		5.0f, 0.0f, 5.0f,
		5.0f, 0.0f, -5.0f,
		-5.0f, 0.0f, 5.0f,

		5.0f, 0.0f, -5.0f,
		-5.0f, 0.0f, -5.0f,
		-5.0f, 0.0f, 5.0f
	};

	GLfloat textureCoordinatesPlane[] = {
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glGenBuffers(1, &texCoordsPlaneVBO);

	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPlane), verticesPlane, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordsPlaneVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinatesPlane), textureCoordinatesPlane, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);	// Unbind VAO

	// Texture
	glGenTextures(1, &texturePlane);
	glBindTexture(GL_TEXTURE_2D, texturePlane);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width = 0;
	int height = 0;
	unsigned char* image_plane = SOIL_load_image("./resources/plane_texture.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_plane);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image_plane);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Geometries::renderPlaneGeom() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	planeShader.Use();
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturePlane);
	glUniform1i(glGetUniformLocation(planeShader.Program, "ourTexture"), 0);

	// Create camera transformation
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(camera->Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(planeShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(planeShader.Program, "view");
	GLint projLoc = glGetUniformLocation(planeShader.Program, "projection");
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0, -1.0, 0.0));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	//GLfloat angle = 20.0f * (9 + 1);
	//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Geometries::createPlaneShader() {
	planeShader = Shader("./shaders/shaderPlane.vertex", "./shaders/shaderPlane.frag");
}

void Geometries::updateCameraObject(Camera* camera) {
	Geometries::camera = camera;
}

void Geometries::createIcosahedronGeom() {
	// create 12 vertices of Icosahedron

	float t = (1.0 + sqrt(5.0)) / 2.0;

	GLfloat verticesIcosahedron[] = {
		-1.0f,	t,  0.0f,
		 1.0f,  t,  0.0f,
		-1.0f, -t,  0.0f,
		 1.0f, -t,  0.0f,

		 0.0f, -1.0f,  t,
		 0.0f,  1.0f,  t,
		 0.0f, -1.0f, -t,
		 0.0f,  1.0f, -t,

		 t,     0.0f, -1.0f,
		 t,     0.0f,  1.0f,
		-t,     0.0f, -1.0f,
		-t,     0.0f,  1.0f
	};

	GLuint indicesIcosahedron[] = {
		// 5 faces around point 0
		0, 11, 5,	
		0, 5, 1,
		0, 1, 7,
		0, 7, 10,
		0, 10, 11,

		// 5 adjacent faces
		1, 5, 9,
		5, 11, 4,
		11, 10, 2,
		10, 7, 6,
		7, 1, 8,

		// 5 faces around point 3
		3, 9, 4,
		3, 4, 2,
		3, 2, 6,
		3, 6, 8,
		3, 8, 9,

		// 5 adjacent faces
		4, 9, 5,
		2, 4, 11,
		6, 2, 10,
		8, 6, 7,
		9, 8, 1
	};

	glGenVertexArrays(1, &icosahedronVAO);
	glGenBuffers(1, &icosahedronVBO);
	glGenBuffers(1, &icosahedronEBO);
	
	glBindVertexArray(icosahedronVAO);

	glBindBuffer(GL_ARRAY_BUFFER, icosahedronVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesIcosahedron), verticesIcosahedron, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, icosahedronEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesIcosahedron), indicesIcosahedron, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Geometries::renderIcosahedronGeom(glm::vec3 position, float distance, glm::vec3 color)
{
	icosahedronShader.Use();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	 // Create camera transformation
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(camera->Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(icosahedronShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(icosahedronShader.Program, "view");
	GLint projLoc = glGetUniformLocation(icosahedronShader.Program, "projection");
	GLint colorLoc = glGetUniformLocation(icosahedronShader.Program, "ucolor");
	
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3f(colorLoc, color.x, color.y, color.z);
	
	glm::vec3 _aux1 = glm::vec3(distance * position.x, distance * position.y, distance * position.z);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(distance * position.x, distance * position.y, distance * position.z));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(icosahedronVAO);
	glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Geometries::createIcosahedronShader()
{
	icosahedronShader = Shader("./shaders/shaderIcosahedron.vertex", "./shaders/shaderIcosahedron.frag");
}

void Geometries::createScreenFillingRectangle()
{
	
	GLfloat vertices[] = {
		 1.0f,   1.0f,  -1.0f,	// Top Right
		 1.0f,  -1.0f,  -1.0f,  // Bottom Right
	    -1.0f,  -1.0f,  -1.0f, // Bottom Left
		-1.0f,   1.0f,  -1.0f   // Top Left
	};

	GLfloat textureCoordinatesNight[] = {
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};
	
	
	/*
	GLfloat vertices[] = {
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f
	};
	*/

	GLuint indices[] = { // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3	 // Second Triangle
	};

	glGenVertexArrays(1, &screenFillingVAO);
	glGenBuffers(1, &screenFillingVBO);
	glGenBuffers(1, &texCoordsScreenFillTextureVBO);
	glGenBuffers(1, &screenFillingEBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(screenFillingVAO);

	glBindBuffer(GL_ARRAY_BUFFER, screenFillingVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordsScreenFillTextureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinatesNight), textureCoordinatesNight, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenFillingEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer regissters VBO as the currently bound vertex buffer object so afterwards we can safely unbind.

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
	
	// Texture
	glGenTextures(1, &textureScreenFill);
	glBindTexture(GL_TEXTURE_2D, textureScreenFill);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Star texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Load, create texture and generate mipmaps
	int width = 0;
	int height = 0;
	unsigned char* image_plane = SOIL_load_image("./resources/night_sky.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_plane);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image_plane);
	glBindTexture(GL_TEXTURE_2D, 0);

	skybox->createSkyboxShader();
	skybox->createSkyboxGeom();
}

float degToRad(float degree)
{
	return (degree * M_PI) / 180;
}

void Geometries::renderSkyOnScreen()
{
	//std::cout << "DELTA TIME: " << utils->deltaTime << std::endl;
	glDisable(GL_DEPTH_TEST);
	skyShader.Use();

	//Bind Texture using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureScreenFill);
	glUniform1i(glGetUniformLocation(skyShader.Program, "textureNight"), 0);

	// Create camera transformation
	glm::mat4 view;
	view = camera->GetViewMatrix();
	// Get the uniform locations
	GLint viewLoc = glGetUniformLocation(skyShader.Program, "view");
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLint uSunDirLoc = glGetUniformLocation(skyShader.Program, "uSunDir");
	GLint gammaMixLoc = glGetUniformLocation(skyShader.Program, "gammaMix");
	GLint isNightTransitionLoc = glGetUniformLocation(skyShader.Program, "isNightTransition");
	GLint gamma1Loc = glGetUniformLocation(skyShader.Program, "gamma1");
	GLint isNightSkyLoc = glGetUniformLocation(skyShader.Program, "isNightSky");
	GLint gamma2Loc = glGetUniformLocation(skyShader.Program, "gamma2");

	if (isNightTransition == true) {
		//updateGamma1();
		glUniform1i(isNightTransitionLoc, 1);
		glUniform1f(gamma1Loc, gamma1);
	}
	else {
		glUniform1i(isNightTransitionLoc, 0);
	}

	if (isNightSky == true) {
		updateMoonPosition();
		updateGamma2();
		//glUniform1i(isNightSkyLoc, 1);
		//skybox->renderSkybox();
		glUniform1f(gamma2Loc, gamma2);
	}
	else {
		glUniform1i(isNightSkyLoc, 0);
	}
	
	glUniform1f(gammaMixLoc, gamma);
	
	if ((isNightTransition == false) && (isNightSky == false)) {
		updateSunPosition();
	}

	

	glUniform3f(uSunDirLoc, sunPos.x, sunPos.y, sunPos.z);
	//glUniform3f(uSunDirLoc, 0, 0, 1);
	
	if (!isNightSky) {
		glBindVertexArray(screenFillingVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
	
	//renderBillboardsSky();

	if ((isNightTransition == false) && (isNightSky == false))
	{
		renderIcosahedronGeom(sunPos, 30.0, glm::vec3(1.0, 1.0, 0.0));
	}
	if (isNightSky)
	{
		renderIcosahedronGeom(moonPos, 20.0, glm::vec3(0.0, 0.0, 1.0));
	}
	
	
	glEnable(GL_DEPTH_TEST);
}



void Geometries::renderBillboardsSky() 
{
	// DAY
	if ((isNightTransition == false) && (isNightSky == false)) 
	{
		billboardListDay[0]->updatePositionBillboard(sunPos);

		billboardListDay[0]->renderBillboardGeom();
		billboardListDay[1]->renderBillboardGeom();
		billboardListDay[2]->renderBillboardGeom();
		billboardListDay[3]->renderBillboardGeom();
	}

	// NIGHT
	
	if (isNightSky == true)
	{
		//std::cout << "MoonPos: " << moonPos.x << " , " << moonPos.y << " , " << moonPos.z << std::endl;
		billboardListNight[0]->updatePositionBillboard(moonPos);
		billboardListNight[0]->renderBillboardGeom();
		//billboardListNight[1]->renderBillboardGeom();
		for (int i = 0; i < utils->billbRandomDataList.size(); i++)
		{
			billboardListNight[1]->renderBillboardGeom(utils->billbRandomDataList[i]._scale, utils->billbRandomDataList[i]._position, utils->billbRandomDataList[i]._distance);
		}
	}
	
}



void Geometries::updateGamma1() {
	float _x = utils->deltaTime;
	gamma1 = 1.1 * pow(_x, (1 / M_PI));
	if (gamma1 > 1.0) {
		isNightTransition = false;
		isNightSky = true;
		utils->resetTime();
		gamma1 = 0.0;
	}
	//std::cout << "GAMMA 1: " << gamma1 << std::endl;
}
void Geometries::updateGamma2() {
	float _x = utils->deltaTime;
	gamma2 = 1.0 * pow(_x, (1 / M_PI));
	//std::cout << "GAMMA 2: " << gamma2 << std::endl;
}



void Geometries::updateSunPosition() {

	// For timelapsing 
	sunAltitude += 0.05;
	if (sunAltitude > 85.0) {
		//isNight = true;
	}
	if (sunAltitude > 90.0) {
		isNightTransition = true;
		utils->resetTime();

		sunAltitude = -90;
	}

	sunPos.z = sin(degToRad(sunAltitude));
	float hyp = cos(degToRad(sunAltitude));
	sunPos.y = hyp * cos(degToRad(sunAzimuth));
	sunPos.x = hyp * sin(degToRad(sunAzimuth));
	//std::cout << "sunAltitude: " << sunAltitude << std::endl;
	if (sunAltitude > 90.0)
	{
		float time = glfwGetTime();
		//std::cout << "Time GLFW: " << time << std::endl;
	}
}

void Geometries::updateMoonPosition() {
	moonAltitude += 0.05;
	
	moonPos.z = sin(degToRad(moonAltitude));
	float hyp = cos(degToRad(moonAltitude));
	moonPos.y = hyp * cos(degToRad(moonAzimuth));
	moonPos.x = hyp * sin(degToRad(moonAzimuth));
}

void Geometries::updateGamma() {
	float time = glfwGetTime();
	//std::cout << "Time GLFW: " << time << std::endl;
	gamma += (time / 4000);
	if (gamma > 1.0) {
		gamma = 1.0;
	}
	//std::cout << "Gamma: " << gamma << std::endl;
	//gamma = (sin(time) / 2) + 0.5;
	if (gamma >= 1.0) {
		//gamma = 0.0;
	}
}

void Geometries::createSkyShader()
{
	skyShader = Shader("./shaders/shaderSky.vertex", "./shaders/shaderSky.frag");
}


void Geometries::createSkyShaderAdv()
{
	skyShaderAdv = Shader("./shaders/shaderSkyAdv.vertex", "./shaders/shaderSkyAdv.frag");
}

void Geometries::renderSkyOnScreenAdv(int drawingMode) {
	//std::cout << "DELTA TIME: " << utils->deltaTime << std::endl;
	if (drawingMode == 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glDisable(GL_DEPTH_TEST);
	skyShaderAdv.Use();

	// Create camera transformations
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(camera->Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
	// Get the uniform locations
	GLint viewLoc = glGetUniformLocation(skyShaderAdv.Program, "viewMatrix");
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(skyShaderAdv.Program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform2f((glGetUniformLocation(skyShaderAdv.Program, "resolution")), screenWidth, screenHeight);

	glm::mat4 model;
	glUniform1f(glGetUniformLocation(skyShaderAdv.Program, "luminance"), 1.0);
	glUniform1f(glGetUniformLocation(skyShaderAdv.Program, "turbidity"), 10);
	glUniform1f(glGetUniformLocation(skyShaderAdv.Program, "rayleigh"), 2);
	glUniform1f(glGetUniformLocation(skyShaderAdv.Program, "mieCoefficient"), 0.005);
	glUniform1f(glGetUniformLocation(skyShaderAdv.Program, "mieDirectionalG"), 0.8);
	glUniform3f(glGetUniformLocation(skyShaderAdv.Program, "sunPosition"), sunPos.x, sunPos.y, sunPos.z);

	//glUniform3f(uSunDirLoc, 0, 0, 1);

	//if (!isNightSky) {
	glBindVertexArray(screenFillingVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	glEnable(GL_DEPTH_TEST);
}