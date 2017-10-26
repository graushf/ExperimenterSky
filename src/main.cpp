// Std. Includes
#include <string>

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
#include "Geometries.h"
#include "Utilities.h"

#include "Skybox.h"

#include "Geodesicsphere.h"

#include "CloudModel2D.h"

#include "SkyClouds.h"

#include "GuiTexture.h"
#include "GuiRenderer.h"

#include "LensFlare.h"
#include "LensFlareRenderer.h"
#include "LensFlareManager.h"

// Properties
GLuint screenWidth = 800, screenHeight = 600;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), screenWidth, screenHeight);
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

int toggleModeDraw = 1;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

Utilities* utilities = new Utilities();
Geometries* geometries = new Geometries(camera, screenWidth, screenHeight, utilities);
Skybox* skybox = new Skybox(camera, screenWidth, screenHeight, utilities);

// The MAIN function, from here we start out application and run our Game loop
int main()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwDefaultWindowHints();

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Cl3ver_DynamicSky", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	utilities->initTimeCounting();

	// Setup and compile our shaders
	Shader ourShader("./shaders/shader.vertex", "./shaders/shader.frag");

	// Set up our vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f
	};
	glm::vec3 cubePositions[] = {
		glm::vec3( 0.0f,  0.0f,   0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f,  -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,  -7.5f),
		glm::vec3( 1.3f, -2.0f,  -2.5f),
		glm::vec3( 1.5f,  2.0f,  -2.5f),
		glm::vec3( 1.5f,  0.2f,  -1.5f),
		glm::vec3(-1.3f,  1.0f,  -1.5f)
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind our Vertex Array Object first, then bind and set our buffers and pointers
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);	// Unbind VAO

	// Load and create a texture
	GLuint texture1;
	GLuint texture2;
	// --== TEXTURE 1 == --
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);	// All upcoming GL_TEXTURE_2D operations now have effect on our texture object
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image("./resources/container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);	// Unbind texture when done, so we won't accidentily mess up our texture.
	// --== TEXTURE 2 == --
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	image = SOIL_load_image("./resources/awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	geometries->createPlaneShader();
	geometries->setPlaneGeom();

	//geometries->initBillboards();

	geometries->createIcosahedronShader();
	geometries->createIcosahedronGeom();

	geometries->createSkyShader();
	geometries->createSkyShaderAdv();
	geometries->createScreenFillingRectangle();

	skybox->createSkyboxShader();
	skybox->createSkyboxGeom();

	Geodesicsphere* geodesicSphere = new Geodesicsphere(camera, screenWidth, screenHeight);
	geodesicSphere->createSkyShader();
	geodesicSphere->Create(4);
	geodesicSphere->createAdvancedGeodesicsphereGeom();
	CloudModel2D* cloudModel2D = new CloudModel2D(camera, screenWidth, screenHeight);
	SkyClouds* skyClouds = new SkyClouds(camera, screenWidth, screenHeight);
	GuiTexture* guiTexture = new GuiTexture("./resources/lensFlare/tex8.png", glm::vec2(0.0f, 0.0f), glm::vec2(0.2f, 0.3f), camera);
	//GuiTexture* guiTexture2 = new GuiTexture("./resources/container.jpg", glm::vec2(-0.5f, 0.5f), glm::vec2(0.3f, 0.3f), camera);
	//GuiTexture* guiTexture3 = new GuiTexture("./resources/moon.png", glm::vec2(-0.5f, -0.5f), glm::vec2(0.4f, 0.4f), camera);
	//GuiTexture* guiTexture4 = new GuiTexture("./resources/sun_texture.png", glm::vec2(0.5f, -0.5f), glm::vec2(0.5f, 0.5f), camera);
	std::vector<GuiTexture*> guiTextureList;
	guiTextureList.push_back(guiTexture);
	//guiTextureList.push_back(guiTexture2);
	//guiTextureList.push_back(guiTexture3);
	//guiTextureList.push_back(guiTexture4);
	GuiRenderer* guiRenderer = new GuiRenderer();

	/*LensFlare* lensFlare = new LensFlare("./resources/lensFlare/tex2.png", glm::vec2(0.0f, 0.0f), glm::vec2(0.1f, 0.1f), camera);
	LensFlare* lensFlare2 = new LensFlare("./resources/lensFlare/tex3.png", glm::vec2(0.0f, 0.0f), glm::vec2(0.2f, 0.2f), camera);
	LensFlare* lensFlare3 = new LensFlare("./resources/lensFlare/tex4.png", glm::vec2(0.0f, 0.0f), glm::vec2(0.2f, 0.2f), camera);
	LensFlare* lensFlare4 = new LensFlare("./resources/lensFlare/tex5.png", glm::vec2(0.0f, 0.0f), glm::vec2(0.3f, 0.3f), camera);
	LensFlare* lensFlare5 = new LensFlare("./resources/lensFlare/tex6.png", glm::vec2(0.0f, 0.0f), glm::vec2(0.4f, 0.4f), camera);
	LensFlare* lensFlare6 = new LensFlare("./resources/lensFlare/tex7.png", glm::vec2(0.0f, 0.0f), glm::vec2(0.5f, 0.5f), camera);
	LensFlare* lensFlare7 = new LensFlare("./resources/lensFlare/tex8.png", glm::vec2(0.0f, 0.0f), glm::vec2(0.6f, 0.6f), camera);*/
	LensFlare* lensFlare = new LensFlare("./resources/awesomeface.png", glm::vec2(0.0f, 0.0f), glm::vec2(0.1f, 0.1f), camera);
	LensFlare* lensFlare2 = new LensFlare("./resources/awesomeface.png", glm::vec2(0.0f, 0.0f), glm::vec2(0.2f, 0.2f), camera);
	LensFlare* lensFlare3 = new LensFlare("./resources/awesomeface.png", glm::vec2(0.0f, 0.0f), glm::vec2(0.2f, 0.2f), camera);
	LensFlare* lensFlare4 = new LensFlare("./resources/awesomeface.png", glm::vec2(0.0f, 0.0f), glm::vec2(0.3f, 0.3f), camera);
	LensFlare* lensFlare5 = new LensFlare("./resources/awesomeface.png", glm::vec2(0.0f, 0.0f), glm::vec2(0.4f, 0.4f), camera);
	LensFlare* lensFlare6 = new LensFlare("./resources/awesomeface.png", glm::vec2(0.0f, 0.0f), glm::vec2(0.5f, 0.5f), camera);
	LensFlare* lensFlare7 = new LensFlare("./resources/awesomeface.png", glm::vec2(0.0f, 0.0f), glm::vec2(0.6f, 0.6f), camera);

	std::vector<LensFlare*> lensFlareList;
	lensFlareList.push_back(lensFlare);
	lensFlareList.push_back(lensFlare2);
	lensFlareList.push_back(lensFlare3);
	lensFlareList.push_back(lensFlare4);
	lensFlareList.push_back(lensFlare5);
	lensFlareList.push_back(lensFlare6);
	lensFlareList.push_back(lensFlare7);

	//LensFlareRenderer* lensFlareRenderer = new LensFlareRenderer();

	LensFlareManager* lensFlareManager = new LensFlareManager(0.4f, lensFlareList);


	// Game Loop
	while (!glfwWindowShouldClose(window))
	{
		utilities->measureTime();
		// Set frame rate
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();

		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update camera for geometries
		geometries->updateCameraObject(camera);
		geometries->updateSunPosition();
		geometries->renderSkyOnScreenAdv(toggleModeDraw);
		// Draw plane
		geometries->renderPlaneGeom();
		geometries->renderIcosahedronGeom(geometries->sunPos, 10.0, glm::vec3(1.0, 0.0, 0.0));
		
		//cloudModel2D->updateCameraObject(camera);
		//cloudModel2D->draw(10000, toggleModeDraw);

		//skyClouds->draw(toggleModeDraw);

		lensFlareManager->render(camera, geometries->sunPos);

		//guiRenderer->render(guiTextureList, toggleModeDraw);
		//lensFlareRenderer->render(lensFlareList, toggleModeDraw);
		//guiTexture->draw(toggleModeDraw);
	
		// Swap the buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they-ve outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}


// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera Controls
	if (keys[GLFW_KEY_W]) {
		camera->ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S]) {
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A]) {
		camera->ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D]) {
		camera->ProcessKeyboard(RIGHT, deltaTime);
	}
}


// Is called whenever a key is pressed/released bia GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) 
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		toggleModeDraw = !toggleModeDraw;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;		// Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera->ProcessMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->ProcessMouseScroll(yoffset);
}