#include "GuiTexture.h"

GuiTexture::GuiTexture(std::string texture, glm::vec2 position, glm::vec2 scale, Camera* camera)
{
	GuiTexture::texture = texture;
	GuiTexture::position = position;
	GuiTexture::scale = scale;
	this->camera = camera;

	loadToVAO();
}

std::string GuiTexture::getTexture()
{
	return texture;
}

glm::vec2 GuiTexture::getPosition()
{
	return position;
}

glm::vec2 GuiTexture::getScale()
{
	return scale;
}

void GuiTexture::loadToVAO()
{
	Shader shader = Shader("./shaders/guiTexture.vertex", "./shaders/guiTexture.frag");

	GLfloat vertices[] = {
		-1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, -1.0f,
		-1.0f, -1.0f
	};

	GLuint indices[] = {
		0, 3, 1,
		3, 2, 1
	};

	GLfloat textureCoordinatesPlane[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	GLuint quadVAO, quadVBO, texCoordsQuadVBO, quadEBO;

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glGenBuffers(1, &texCoordsQuadVBO);
	glGenBuffers(1, &quadEBO);

	glBindVertexArray(quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordsQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinatesPlane), textureCoordinatesPlane, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	guiTextureData.VAO = quadVAO;
	guiTextureData.shader = shader;
	guiTextureData.EBO = quadEBO;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); // Unbind VAO

	GLuint textureQuad;
	glGenTextures(1, &textureQuad);
	glBindTexture(GL_TEXTURE_2D, textureQuad);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width = 0;
	int height = 0;
	unsigned char* image_plane = SOIL_load_image(texture.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_plane);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image_plane);
	glBindTexture(GL_TEXTURE_2D, 0);

	guiTextureData.texture = textureQuad;
}

void GuiTexture::draw(int viewMode)
{
	if (viewMode == 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	guiTextureData.shader.Use();
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, guiTextureData.texture);
	glUniform1i(glGetUniformLocation(guiTextureData.shader.Program, "textureSampler"), 0);

	// Create camera transformations
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = camera->GetPerspectiveMatrix();
	// Get the uniform locations
	
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(position.x, position.y, 0.0));
	transform = glm::scale(transform, glm::vec3(scale.x, scale.y, 1.0));
	GLuint loc = glGetUniformLocation(guiTextureData.shader.Program, "transformationMatrix");
	glUniformMatrix4fv(glGetUniformLocation(guiTextureData.shader.Program, "transformationMatrix"), 1, GL_FALSE, glm::value_ptr(transform));
	glBindVertexArray(guiTextureData.VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}