#include "control.h"
#include <iostream>
#include <glm/ext.hpp>

extern GLFWwindow* window;
extern Camera camera;
extern float delta_time;
extern glm::vec3 cursor_click_point;

void getDeviceInput() {
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
  // last_time = current_time;
}

// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
// }

void cursor_ray_screen2world(int screen_x, int screen_y, int screen_width, int screen_height,
    glm::mat4 view_matrix, glm::mat4 projection_matrix, glm::vec3 &out_ray_origin, glm::vec3 &out_ray_direction) {
  glm::vec4 cursor_ray_x(
      ((float)screen_x/(float)screen_width - 0.5f) * 2.0f,
      ((float)screen_y/(float)screen_height - 0.5f) * 2.0f,
      -1.0f,
      1.0f);
  glm::vec4 cursor_ray_y(
      ((float)screen_x/(float)screen_width - 0.5f) * 2.0f,
      ((float)screen_y/(float)screen_height - 0.5f) * 2.0f,
      0.0f,
      1.0f);

  glm::mat4 inverse_view_matrix = glm::inverse(view_matrix);
  glm::mat4 inverse_projection_matrix = glm::inverse(projection_matrix);

  glm::vec4 ray_x_camera = inverse_projection_matrix * cursor_ray_x;
  ray_x_camera /= ray_x_camera.w;
  glm::vec4 ray_x_world = inverse_view_matrix * ray_x_camera;
  ray_x_world /= ray_x_world.w;
  glm::vec4 ray_y_camera = inverse_projection_matrix * cursor_ray_y;
  ray_y_camera /= ray_y_camera.w;
  glm::vec4 ray_y_world = inverse_view_matrix * ray_y_camera;
  ray_y_world /= ray_y_world.w;

  out_ray_origin = glm::vec3(ray_x_world);
  out_ray_direction = glm::normalize(glm::vec3(ray_y_world - ray_x_world));
}

// Obb algorithm to detect mouse click 
// bool ray_obb_intersection(glm::vec3 ray_origin, glm::vec3 ray_direction,
    // glm::vec3 coord_min, glm::vec3 coord_max, glm::mat4 model_matrix, float &distance) {
// }

// For this game, only need to detect cursor ray and the top plane of rulers
glm::vec3 ray_plane_intersection(glm::vec3 ray_origin, glm::vec3 ray_direction) {
  glm::vec3 plane_normal(0.0f, 1.0f, 0.0f);
  glm::vec3 plane_point(0.0f, 0.35f * 0.1f, 0.0f);
  glm::vec3 intersection_point_world;
  // know plane and line, calculate the intersection of them
  if(glm::dot(ray_direction, plane_normal) != 0) {
    float d = glm::dot((plane_point - ray_origin), plane_normal);
    d = d / glm::dot(ray_direction, plane_normal);
    intersection_point_world = d * ray_direction + ray_origin;
  }
  else {
    intersection_point_world = glm::vec3(0, 0, 0);
  }

  return intersection_point_world;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  // mouse left click to decide the point on ruler
  if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    std::cout << "x: " << xpos << " , y: " << ypos << std::endl;
    int width, height;
    glm::vec3 ray_origin, ray_direction;
    glfwGetWindowSize(window, &width, &height);
    cursor_ray_screen2world((float)xpos, (float)ypos, width, height,
        camera.getViewMatrix(), camera.getProjectionMatrix(), ray_origin, ray_direction);
    std::cout << glm::to_string(ray_origin) << " , " << glm::to_string(ray_direction) << std::endl;
    // glm::vec3 intersection_point_world = ray_plane_intersection(ray_origin, ray_direction);
    cursor_click_point = ray_plane_intersection(ray_origin, ray_direction);
    std::cout << "intersection: " << glm::to_string(cursor_click_point) << std::endl;

    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::scale(model_matrix, glm::vec3(0.06f, 0.1f, 0.06f));
    model_matrix = glm::rotate(glm::radians(180.0f - 45.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * model_matrix;
    model_matrix = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)) * model_matrix;
    pick_ruler(model_matrix);
  }
}

void pick_ruler(glm::mat4 model_matrix) {
  glm::mat4 inverse_model_matrix = glm::inverse(model_matrix);
  glm::vec3 intersection_point_model = inverse_model_matrix * glm::vec4(cursor_click_point, 1.0f);
  std::cout << "intersection in model: " << glm::to_string(intersection_point_model) << std::endl;
  if(intersection_point_model.x < 12.5f && intersection_point_model.x > -12.5f
      && intersection_point_model.z < 1.5f && intersection_point_model.z > -1.5f) {
    std::cout << "click in the ruler!" << std::endl;
  }
}
