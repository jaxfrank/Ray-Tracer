#include "Camera.h"

#include <glm\glm.hpp>

Camera::Camera(float fov):
fov(fov)
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