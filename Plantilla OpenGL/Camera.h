#include <iostream>
#include <openGL/glut.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class Camera
{
public:
	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 frt = glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3 u = glm::vec3(0.0f, 1.0f, 0.0f))
		: position(pos), front(frt), up(u) {}
	
	glm::mat4 getViewMatrix() {
		return glm::lookAt(position, position + front, up);
	}

	void moveForward(float delta) {
		position += front * delta;
	}

	void moveBackward(float delta) {
		position -= front * delta;
	}

	void moveLeft(float delta) {
		position -= glm::normalize(glm::cross(front, up)) * delta;
	}

	void moveRight(float delta) {
		position += glm::normalize(glm::cross(front, up)) * delta;
	}

public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
};