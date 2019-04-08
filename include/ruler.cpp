#include "ruler.h"
#include <iostream>
#include <glm/ext.hpp>

extern GLFWwindow* window;
extern float delta_time;

bool initialize_ruler(std::vector<glm::vec3>& vertices, 
  std::vector<glm::vec2>& uv_vertices, std::vector<glm::vec3>& normal_vectices) {
  bool result = load_obj("model/ruler.obj", vertices, uv_vertices, normal_vectices);

  return result;
}

Ruler::Ruler(unsigned int id, glm::vec3 p, float r) : object_id(id), pos(p), radian(r) {
  // radian = 0.5f * 3.14f;
  velocity = glm::vec3(0.0f, 0.0f, 0.0f);
  angular_velocity = 0.0f;
  moment_of_inertia = 0.01f;
  // vertices coords of collision box
  vertices_2d_box_modelspace.push_back(glm::vec3( 12.5f, 0.0f, -1.5f));
  vertices_2d_box_modelspace.push_back(glm::vec3(-12.5f, 0.0f, -1.5f));
  vertices_2d_box_modelspace.push_back(glm::vec3(-12.5f, 0.0f,  1.5f));
  vertices_2d_box_modelspace.push_back(glm::vec3( 12.5f, 0.0f,  1.5f));
}

glm::vec3 Ruler::getVelocity() {
  return velocity;
}

void Ruler::setVelocity(glm::vec3 _velocity) {
  velocity = _velocity;
}
float Ruler::getAngularVelocity() {
  return angular_velocity;
}

void Ruler::setAngularVelocity(float _angular_velocity) {
  angular_velocity = _angular_velocity;
}

float Ruler::getMomentOfInertia() {
  return moment_of_inertia;
}

void Ruler::move(glm::vec3 delta_pos, float delta_radian) {
  pos += delta_pos;
  // std::cout << glm::to_string(delta_pos) << std::endl;
  radian += delta_radian;
}

void Ruler::update() {
  // if velocity is not zero
  if(!glm::all(glm::epsilonEqual(velocity, glm::vec3(0.0f, 0.0f, 0.0f), 0.0001f))) {
    move(velocity * delta_time, angular_velocity * delta_time);
    std::cout << object_id << ": " << " pos: " << glm::to_string(pos) << " , angle: " << radian << std::endl;
    // std::cout << glm::to_string(glm::epsilonEqual(velocity, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f)) << std::endl;
    // frictional force of the desk
    addForce(*this, 0.5f, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    // addForce((*ruler_list)[0], 10.0f, glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(1.5f, 0.0f, 12.5f));
    // collision detection
    std::vector<Ruler> *ruler_list = (std::vector<Ruler>*)glfwGetWindowUserPointer(window);
    CollisionResult result = collision_detection(*this, (*ruler_list)[1-object_id]);
    if(result.is_collided) {
      std::cout << "intersection: "<< glm::to_string(result.intersection) << std::endl;
      std::cout << "pos: "<< glm::to_string(pos) << std::endl;
      velocity = glm::vec3(0.0f, 0.0f, 0.0f);
      angular_velocity = 0.0f;
      addForce((*ruler_list)[1-object_id], 5.0f,
          glm::vec3(result.intersection.x, 0.0f, result.intersection.y), glm::vec3(12.5f, 0.0f, -1.5f));
    };
  }
}

glm::mat4 Ruler::getModelMatrix() {
  glm::mat4 model_matrix = glm::mat4(1.0f);
  model_matrix = glm::scale(model_matrix, glm::vec3(0.06f, 0.1f, 0.06f));
  model_matrix = glm::rotate(radian, glm::vec3(0.0f, 1.0f, 0.0f)) * model_matrix;
  return glm::translate(pos) * model_matrix;
}

glm::vec2 Ruler::get_center() {
  return glm::vec2(pos.x, pos.z);
}

glm::vec2 Ruler::support_function(glm::vec2 direction) {
  glm::vec4 vertex_world_space = getModelMatrix() * glm::vec4(vertices_2d_box_modelspace[0], 1.0f);
  glm::vec2 vertex_2d_world_space = glm::vec2(vertex_world_space.x, vertex_world_space.z);
  float max_distance = glm::dot(vertex_2d_world_space, direction);
  glm::vec2 max_vertex = vertex_2d_world_space;
  for(int i = 1; i < vertices_2d_box_modelspace.size(); i++) {
    vertex_world_space = getModelMatrix() * glm::vec4(vertices_2d_box_modelspace[i], 1.0f);
    vertex_2d_world_space = glm::vec2(vertex_world_space.x, vertex_world_space.z);
    float distance = glm::dot(vertex_2d_world_space, direction);
    if(distance > max_distance) {
      max_distance = distance;
      max_vertex = vertex_2d_world_space;
    }
  }
  // std::cout << "ruler " << object_id << "   max vertex: " << glm::to_string(max_vertex)
    // << "  direction: " << glm::to_string(direction) << std::endl;
  return max_vertex;
}
