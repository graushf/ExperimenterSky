#include "Billboard.h"



Billboard::Billboard(Camera* camera, float screenWidth, float screenHeight, glm::vec3 position, glm::vec3 scale, float distance, char* texturebillboardpath, char* vertexShaderSrc, char* fragmentShaderSrc)
{
	Billboard::camera = camera;
	Billboard::screenWidth = screenWidth;
	Billboard::screenHeight = screenHeight;

	Billboard::_position = position;
	Billboard::_scale = scale;
	Billboard::_distance = distance;

	createBillboardGeom(texturebillboardpath);
	createBillboardShader(vertexShaderSrc, fragmentShaderSrc);
}


void Billboard::createBillboardGeom(char* texturebillboardpath)
{
	glewInit();
	GLfloat vertices[] = {
		0.5f, 0.5f, 0.0f,	// Top Right
		0.5f, -0.5f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f, // Bottom Left
		-0.5f, 0.5f, 0.0f   // Top Left
	};

	GLuint indices[] = { // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3	 // Second Triangle
	};

	GLfloat textureCoordinatesBillboard[] = {
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	glGenVertexArrays(1, &billboardVAO);
	glGenBuffers(1, &billboardVBO);
	glGenBuffers(1, &texCoordsBillboardVBO);
	glGenBuffers(1, &billboardEBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(billboardVAO);

	glBindBuffer(GL_ARRAY_BUFFER, billboardVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordsBillboardVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinatesBillboard), textureCoordinatesBillboard, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, billboardEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer regissters VBO as the currently bound vertex buffer object so afterwards we can safely unbind.

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	// Texture 
	glGenTextures(1, &textureBillboard);
	glBindTexture(GL_TEXTURE_2D, textureBillboard);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width = 0;
	int height = 0;
	//"./resources/sun_texture.png"
	unsigned char* image_plane = SOIL_load_image(texturebillboardpath, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_plane);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image_plane);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Billboard::renderBillboardGeom()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	billboardShader.Use();

	//Bind Texture using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBillboard);
	GLint aux = glGetUniformLocation(billboardShader.Program, "textureBillboard");
	glUniform1i(glGetUniformLocation(billboardShader.Program, "textureBillboard"), 0);

	// Create camera transformation
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(camera->Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(billboardShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(billboardShader.Program, "view");
	GLint projLoc = glGetUniformLocation(billboardShader.Program, "projection");
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glm::mat4 model;

	glm::vec3 posBillboard;

	posBillboard.x = _distance * _position.x;
	posBillboard.y = _distance * _position.y;
	posBillboard.z = _distance * _position.z;

	//model = glm::translate(model, glm::vec3(posBillboard.x, posBillboard.y, posBillboard.z));
	model = glm::scale(model, _scale);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint pointPosLoc = glGetUniformLocation(billboardShader.Program, "point_pos");
	GLint cameraPosLoc = glGetUniformLocation(billboardShader.Program, "camera_pos");
	GLint cameraUpLoc = glGetUniformLocation(billboardShader.Program, "camera_up");
	GLint scaleLoc = glGetUniformLocation(billboardShader.Program, "scaleBillboard");

	glUniform3f(pointPosLoc, posBillboard.x, posBillboard.y, posBillboard.z);
	glUniform3f(cameraPosLoc, camera->Position.x, camera->Position.y, camera->Position.z);
	glUniform3f(cameraUpLoc, camera->Up.x, camera->Up.y, camera->Up.z);
	glUniform2f(scaleLoc, _scale.x, _scale.y);

	glBindVertexArray(billboardVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Billboard::renderBillboardGeom(glm::vec3 scale, glm::vec3 position, float distance)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	billboardShader.Use();

	//Bind Texture using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBillboard);
	GLint aux = glGetUniformLocation(billboardShader.Program, "textureBillboard");
	glUniform1i(glGetUniformLocation(billboardShader.Program, "textureBillboard"), 0);

	// Create camera transformation
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(camera->Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(billboardShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(billboardShader.Program, "view");
	GLint projLoc = glGetUniformLocation(billboardShader.Program, "projection");
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glm::mat4 model;

	glm::vec3 posBillboard;

	posBillboard.x = distance * position.x;
	posBillboard.y = distance * position.y;
	posBillboard.z = distance * position.z;

	model = glm::translate(model, glm::vec3(posBillboard.x, posBillboard.y, posBillboard.z));
	model = glm::scale(model, scale);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint pointPosLoc = glGetUniformLocation(billboardShader.Program, "point_pos");
	GLint cameraPosLoc = glGetUniformLocation(billboardShader.Program, "camera_pos");
	GLint cameraUpLoc = glGetUniformLocation(billboardShader.Program, "camera_up");
	GLint scaleLoc = glGetUniformLocation(billboardShader.Program, "scaleBillboard");

	glUniform3f(pointPosLoc, posBillboard.x, posBillboard.y, posBillboard.z);
	glUniform3f(cameraPosLoc, camera->Position.x, camera->Position.y, camera->Position.z);
	glUniform3f(cameraUpLoc, camera->Up.x, camera->Up.y, camera->Up.z);
	glUniform2f(scaleLoc, scale.x, scale.y);


	glBindVertexArray(billboardVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Billboard::updatePositionBillboard(glm::vec3 position) {
	_position = position;
}

void Billboard::createBillboardShader(char* vertexShaderSrc, char* fragmentShaderSrc)
{
	billboardShader = Shader(vertexShaderSrc, fragmentShaderSrc);
}

