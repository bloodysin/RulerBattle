#ifndef CONTROL_H
#define CONTROL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "camera.h"

void getDeviceInput();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void pick_ruler(glm::mat4 model_matrix);

#endif
