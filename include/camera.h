#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
  private:
    float speed;
    glm::vec3 pos;
  public:
    Camera();
    void move(glm::vec3 variation);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
};

void getDeviceInput();

#endif
