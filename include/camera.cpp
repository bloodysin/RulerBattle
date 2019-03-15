#include "camera.h"
// #include <iostream>
// #include <glm/ext.hpp>
// extern GLFWwindow* window;
// extern Camera camera;

Camera::Camera() : 
  pos(glm::vec3(0.0f, 3.0f, 2.0f)) {
  speed = 3.0f;
  level = 0;
}

void Camera::move(glm::vec3 variation) {
  pos += variation * speed;
}

glm::mat4 Camera::getViewMatrix() {
  // std::cout << glm::to_string(pos) << std::endl;
  return glm::lookAt(pos, pos + glm::vec3(0.0f, -3.0f, -2.0f), glm::vec3(0.0f, 0.0f, -1.0f));
  
}

glm::mat4 Camera::getProjectionMatrix() {
  return glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
}
