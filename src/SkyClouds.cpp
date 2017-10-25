#include "SkyClouds.h"

SkyClouds::SkyClouds(Camera* camera, int screenWidth, int screenHeight) {
	if (!setupSkyCloudsGeom()) {
		std::cout << "The cloud shader data couldn't be initialized" << std::endl;
	}
	SkyClouds::camera = camera;
	SkyClouds::screenWidth = screenWidth;
	SkyClouds::screenHeight = screenHeight;
}

bool SkyClouds::setupSkyCloudsGeom()
{
	Shader shader = Shader("./shaders/clouds.vertex", "./shaders/clouds.frag");

	GLfloat vertices[] = {
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f
	};

	GLuint indices[] = { // Note that we start from 0!
		1, 0, 2, // First triangle
		2, 0, 3
	};

	GLuint VAO, VBOvertices, EBOindices;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBOvertices);
	glGenBuffers(1, &EBOindices);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBOvertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOindices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	cloudData.VAO = VAO;
	cloudData.shader = shader;

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	if (VAO != -1) return true;
	return false;
}

void SkyClouds::draw(int drawingMode)
{
	if (drawingMode == 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glDisable(GL_DEPTH_TEST);
	cloudData.shader.Use();

	// Create camera transformations
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(camera->Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
	// Get the uniform locations
	// Pass the matrices to the shader
	glUniformMatrix4fv(glGetUniformLocation(cloudData.shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(cloudData.shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform2f((glGetUniformLocation(cloudData.shader.Program, "resolution")), screenWidth, screenHeight);
	glm::mat4 model;

	glBindVertexArray(cloudData.VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);
}

