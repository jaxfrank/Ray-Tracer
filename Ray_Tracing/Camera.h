#pragma once

class Camera {
public:
    Camera(float fov);
    ~Camera();

    void setFOV(float fov);
    float getFOV() const { return fov; }

    float getScale() const { return scale; }

    glm::vec3 getPosition() const { return position; }
    void setPosition(glm::vec3 position) { this->position = position; }

    glm::vec3 getForward() const { return forward; }
    void setForward(glm::vec3 forward) { this->forward = forward; }

private:
    glm::vec3 position;
    glm::vec3 forward;

    float fov;
    float scale;

    void setScale(float fov);
};

