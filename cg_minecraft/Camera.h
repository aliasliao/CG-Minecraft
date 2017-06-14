#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "define.h"

class Camera
{
	glm::vec3 position;
	glm::vec3 frontVec;
	glm::vec3 upVec;
	glm::vec3 rightVec;
	glm::vec3 worldUpVec;
	float yaw;
	float pitch;
	float moveSpeed;
	float mouseSensitivity;
	float zoom;

public:
	Camera(const glm::vec3 &position = glm::vec3(0, 0, 0))
	{
		this->position = position;
		this->frontVec = glm::vec3(0, 0, -1);
		this->upVec = glm::vec3(0, 1, 0);
		this->rightVec = glm::vec3(1, 0, 0);
		this->worldUpVec = glm::vec3(0, 1, 0);

		this->yaw = -90.0f;
		this->pitch = 0.0f;
		this->moveSpeed = 5.0f;
		this->mouseSensitivity = 0.1f;
		this->zoom = 45.0f;
	}

	glm::mat4 getViewMat()
	{
		return glm::lookAt(this->position, this->position + this->frontVec, this->upVec);
	}

	glm::vec3 getPosVec()
	{
		return this->position;
	}

	float getZoom()
	{
		return this->zoom;
	}

	void processKeyboard(cam direction, float deltaTime)  // deltaTime is interval between frames
	{
		float velocity = this->moveSpeed * deltaTime;
		switch (direction)
		{
			case cam::front:
				this->position += this->frontVec * velocity;
				break;
			case cam::back:
				this->position -= this->frontVec * velocity;
				break;
			case cam::left:
				this->position -= this->rightVec * velocity;
				break;
			case cam::right:
				this->position += this->rightVec * velocity;
				break;
			default:
				break;
		}
	}

	void processMouseMove(float xoffset, float yoffset)
	{
		this->yaw += xoffset * this->mouseSensitivity;
		this->pitch += yoffset * this->mouseSensitivity;

		if (this->pitch > 89.0f) {
			this->pitch = 89.0f;
		}
		if (this->pitch < -89.0f) {
			this->pitch = -89.0f;
		}

		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		this->frontVec = glm::normalize(front);
		this->rightVec = glm::normalize(glm::cross(this->frontVec, this->worldUpVec));
		this->upVec = glm::normalize(glm::cross(this->rightVec, this->frontVec));
	}
};