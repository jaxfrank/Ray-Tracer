#include "Camera.h"

#include <glm\glm.hpp>

Camera::Camera(float fov):
fov(fov),
position(glm::vec3(0.0f, 0.0f, 0.0f)),
forward(glm::vec3(0.0f, 0.0f, -1.0f))
{
    setScale(fov);
}

Camera::~Camera() {}

void Camera::setFOV(float fov) {
    this->fov = fov;
    setScale(fov);
}

void Camera::setScale(float fov) {
    scale = glm::tan(glm::radians(fov / 2));
}