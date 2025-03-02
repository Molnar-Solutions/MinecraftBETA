#pragma once

#include <GL/glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();
	Camera(
		glm::vec3 startPosition,
		glm::vec3 startUp,
		GLfloat startYaw,
		GLfloat startPitch,
		GLfloat startMovementSpeed,
		GLfloat startTurnSpeed
	);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::mat4 calculateViewMatrix()
	{
		return glm::lookAt(position, position + front, up);
	}

	glm::vec3 getPosition() { return position; }
	glm::vec3 getDirection() { return glm::normalize(front); }


	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw, pitch,
		movementSpeed, turnSpeed;

	void update();
};

