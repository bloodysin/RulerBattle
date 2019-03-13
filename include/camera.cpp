#include "camera.h"
#include <iostream>
#include <glm/ext.hpp>
extern GLFWwindow* window;
extern Camera camera;

Camera::Camera() : 
  pos(glm::vec3(0.0f, 3.0f, 2.0f)) {
  speed = 3.0f;
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

void getDeviceInput() {
  static double last_time = glfwGetTime();
  double current_time = glfwGetTime();
  float delta_time = float(current_time - last_time);
  // get keyboard input
  if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    camera.move(glm::vec3(0.0f, 0.0f, -1.0f) * delta_time);
  }
  if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    camera.move(glm::vec3(0.0f, 0.0f, 1.0f) * delta_time);
  }
  if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    camera.move(glm::vec3(-1.0f, 0.0f, 0.0f) * delta_time);
  }
  if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    camera.move(glm::vec3(1.0f, 0.0f, 0.0f) * delta_time);
  }
  last_time = current_time;
}
