#include "Camera.h"



// Default camera values
const GLfloat YAW					= -90.0f;
const GLfloat PITCH					= 0.0f;
const GLfloat SPEED					= 9.0f;
const GLfloat SENSITIVITY			= 0.25f;
const GLfloat ZOOM					= 45.0f;
const GLfloat SCROLLSENSITIVITY		= 0.05f;



Camera::Camera(int screenWidth, int screenHeight) {
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	GLfloat yaw = YAW;
	GLfloat pitch = PITCH;
	Camera::Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Camera::MovementSpeed = SPEED;
	Camera::MouseSensitivity = SENSITIVITY;
	Camera::Zoom = ZOOM;
	this->Position = position;
	this->WorldUp = up;
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
}

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, int screenWidth, int screenHeight){
	Camera::Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Camera::MovementSpeed = SPEED;
	Camera::MouseSensitivity = SENSITIVITY;
	Camera::Zoom = ZOOM;
	this->Position = position;
	this->WorldUp = up;
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}


Camera::Camera(glm::vec3 position, int screenWidth, int screenHeight) {
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	GLfloat yaw = YAW;
	GLfloat pitch = PITCH;
	Camera::Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Camera::MovementSpeed = SPEED;
	Camera::MouseSensitivity = SENSITIVITY;
	Camera::Zoom = ZOOM;
	this->Position = position;
	this->WorldUp = up;
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}


Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) {
	Camera::Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Camera::MovementSpeed = SPEED;
	Camera::MouseSensitivity = SENSITIVITY;
	Camera::Zoom = ZOOM;
	this->Position = glm::vec3(posX, posY, posZ);
	this->WorldUp = glm::vec3(upX, upY, upZ);
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}


void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
	GLfloat velocity = this->MovementSpeed * deltaTime;
	if (direction == FORWARD) {
		this->Position += this->Front * velocity;
	}
	if (direction == BACKWARD) {
		this->Position -= this->Front * velocity;
	}
	if (direction == LEFT) {
		this->Position -= this->Right * velocity;
	}
	if (direction == RIGHT) {
		this->Position += this->Right * velocity;
	}
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch) {
	constrainPitch = true;
	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	this->Yaw += xoffset;
	this->Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen downs't get flipped
	if (constrainPitch) {
		if (this->Pitch > 89.0f) {
			this->Pitch = 89.0f;
		}
		if (this->Pitch < -89.0f) {
			this->Pitch = -89.0f;
		}
	}

	// Update Front, Right and Up Vectors using the updater Euler Angles
	this->updateCameraVectors();
}



void Camera::ProcessMouseScroll(GLfloat yoffset) {
	if (this->Zoom >= 1.0f && this->Zoom <= 45.0f) {
		this->Zoom -= yoffset * SCROLLSENSITIVITY;
	}
	if (this->Zoom <= 1.0f) {
		this->Zoom = 1.0f;
	}
	if (this->Zoom >= 45.0f) {
		this->Zoom = 45.0f;
	}
}

void Camera::updateCameraVectors() {
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	front.y = sin(glm::radians(this->Pitch));
	front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	this->Front = glm::normalize(front);
	// Also re-calculate the Right and Up Vectors
	this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp)); // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}

glm::mat4 Camera::GetPerspectiveMatrix() {
	return glm::perspective(this->Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100000.0f);
}