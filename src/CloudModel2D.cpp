#include "CloudModel2D.h"



CloudModel2D::CloudModel2D(Camera* camera, float screenWidth, float screenHeight)
{
	if (!setupCloudPlaneData()) {
		std::cout << "The 2d cloud model data could not be generated" << std::endl;
	}
	CloudModel2D::camera = camera;
	CloudModel2D::screenWidth = screenWidth;
	CloudModel2D::screenHeight = screenHeight;
}

CloudModel2D::~CloudModel2D()
{

}

bool CloudModel2D::setupCloudPlaneData()
{
	Shader shader = Shader("./shaders/clouds.vertex", "./shaders/clouds.frag");

	GLfloat vertices[] = {
		-0.5f, 0.0f, 0.5f,
		 0.5f, 0.0f, 0.5f,
		 0.5f,  0.0f, -0.5f,
		 -0.5f, 0.0f, -0.5f,
	};

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	GLfloat textureCoordinatesPlane[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	GLuint planeVAO, planeVBO, texCoordsPlaneVBO, EBO;

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glGenBuffers(1, &texCoordsPlaneVBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordsPlaneVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinatesPlane), textureCoordinatesPlane, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	cloudModelData.VAO = planeVAO;
	cloudModelData.shader = shader;
	cloudModelData.EBO = EBO;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); // Unbind VAO

	// Texture
	GLuint texturePlane;
	glGenTextures(1, &texturePlane);
	glBindTexture(GL_TEXTURE_2D, texturePlane);
		// Set our texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width = 0;
	int height = 0;
	unsigned char* image_plane = SOIL_load_image("./resources/test.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_plane);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image_plane);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	cloudModelData.textureBuffer = texturePlane;

	if ((planeVAO == -1) && (shader.Program == -1)) {
		return false;
	}
	else {
		return true;
	}
}

void CloudModel2D::draw(float Height, int viewMode)
{
	if (viewMode == 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} 
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	cloudModelData.shader.Use();
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cloudModelData.textureBuffer);
	glUniform1i(glGetUniformLocation(cloudModelData.shader.Program, "texture"), 0);

	// Create camera transfomation
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(camera->Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100000.0f);
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(cloudModelData.shader.Program, "model");
	GLint viewLoc = glGetUniformLocation(cloudModelData.shader.Program, "view");
	GLint projLoc = glGetUniformLocation(cloudModelData.shader.Program, "projection");
	glUniform2f((glGetUniformLocation(cloudModelData.shader.Program, "resolution")), screenWidth, screenHeight);
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0, Height, 0.0));
	//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0, 1.0, 0.0));
	float scale_factor = 50*Height;
	model = glm::scale(model, glm::vec3(scale_factor, scale_factor, scale_factor));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	
	glBindVertexArray(cloudModelData.VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//cloudModelData.shader.Use();
	//// Bind Textures using texture units
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, cloudModelData.textureBuffer);
	//glUniform1i(glGetUniformLocation(cloudModelData.shader.Program, "texture"), 0);

	//// Create camera transformation
	//glm::mat4 view;
	//view = camera->GetViewMatrix();
	//glm::mat4 projection;
	//projection = glm::perspective(camera->Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
	//
	//glm::mat4 model;
	//model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
	////model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0, 1.0, 0.0));
	//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

	//GLuint loc1 = glGetUniformLocation(cloudModelData.shader.Program, "model");
	//GLuint loc2 = glGetUniformLocation(cloudModelData.shader.Program, "view");
	//GLuint loc3 = glGetUniformLocation(cloudModelData.shader.Program, "projection");

	//// Pass the matrices to the shader
	//glUniformMatrix4fv(glGetUniformLocation(cloudModelData.shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//glUniformMatrix4fv(glGetUniformLocation(cloudModelData.shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	//glUniformMatrix4fv(glGetUniformLocation(cloudModelData.shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	//
	//glBindVertexArray(cloudModelData.VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void CloudModel2D::updateCameraObject(Camera* camera)
{
	CloudModel2D::camera = camera;
}