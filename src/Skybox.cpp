#include "Skybox.h"


Skybox::Skybox(Camera* camera, GLuint screenWidth, GLuint screenHeight, Utilities* utils)
{
	Skybox::camera = camera;
	Skybox::screenWidth = screenWidth;
	Skybox::screenHeight = screenHeight;	
	Skybox::utils = utils;

	skyboxTexture = GL_NONE;
	skyboxVAO = GL_NONE;	
}

void Skybox::loadCubemap(std::vector<const GLchar*> faces)
{
	glGenTextures(1, &skyboxTexture);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::createSkyboxGeom()
{
	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	glGenBuffers(1, &skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	std::vector<const GLchar*> faces;
	faces.push_back("resources/skybox/right.png");
	faces.push_back("resources/skybox/left.png");
	faces.push_back("resources/skybox/top.png");
	faces.push_back("resources/skybox/bottom.png");
	faces.push_back("resources/skybox/back.png");
	faces.push_back("resources/skybox/front.png");
	loadCubemap(faces);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
}

void Skybox::createSkyboxShader()
{
	skyboxShader = Shader("./shaders/shaderSkybox.vertex", "./shaders/shaderSkybox.frag");
}

void Skybox::renderSkybox()
{
	glDepthMask(GL_FALSE);
	skyboxShader.Use();

	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(camera->Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(skyboxShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(skyboxShader.Program, "view");
	GLint projLoc = glGetUniformLocation(skyboxShader.Program, "projection");
	GLint uSkyboxUniform = glGetUniformLocation(skyboxShader.Program, "skybox");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	
	glm::mat4 model;
	model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uSkyboxUniform, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

	glBindVertexArray(skyboxVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);		
}





