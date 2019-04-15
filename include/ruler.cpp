#include "ruler.h"
#include <iostream>
#include <glm/ext.hpp>

extern GLFWwindow* window;
extern float delta_time;
extern std::vector<Ruler> ruler_list;

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
  prev_move = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
  // vertices coords and normals for collision box
  vertices_2d_box = std::vector<glm::vec2>(4, glm::vec2(0, 0));
  normals_2d_box = std::vector<glm::vec2>(4, glm::vec2(0, 0));
  // the order is top-left, top-right, bottom-right, bottom-left
  vertices_2d_box_modelspace.push_back(glm::vec3(-12.5f, 0.0f, -1.5f));
  vertices_2d_box_modelspace.push_back(glm::vec3( 12.5f, 0.0f, -1.5f));
  vertices_2d_box_modelspace.push_back(glm::vec3( 12.5f, 0.0f,  1.5f));
  vertices_2d_box_modelspace.push_back(glm::vec3(-12.5f, 0.0f,  1.5f));
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

std::vector<glm::vec2> Ruler::getVertices() {
  return vertices_2d_box;
}

std::vector<glm::vec2> Ruler::getNormals() {
  return normals_2d_box;
}

void Ruler::move(glm::vec3 delta_pos, float delta_radian) {
  CollisionResult result = collision_detection_SAT(*this, ruler_list[1-object_id]);
  if(!result.is_collided) {
    pos += delta_pos;
    radian += delta_radian;
    prev_move = glm::vec4(delta_pos, delta_radian);
  }
  else {
    pos -= glm::vec3(prev_move.x, prev_move.y, prev_move.z);
    radian -= prev_move.w;
  }
}

void Ruler::update() {
  updateVertices();
  updateNormals();
  // if velocity is not zero
  // if(!glm::all(glm::epsilonEqual(velocity, glm::vec3(0.0f, 0.0f, 0.0f), 0.0001f))) {
    // move(velocity * delta_time, angular_velocity * delta_time);
    // frictional force of the desk
    // addForce(*this, 0.5f, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    // addForce((*ruler_list)[0], 10.0f, glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(1.5f, 0.0f, 12.5f));
    // collision detection
    // std::vector<Ruler> *ruler_list = (std::vector<Ruler>*)glfwGetWindowUserPointer(window);
    // CollisionResult result = collision_detection_SAT(*this, (*ruler_list)[1-object_id]);
    // if(result.is_collided) {
      // std::cout << "intersection: "<< glm::to_string(result.intersection) << std::endl;
      // velocity = glm::vec3(0.0f, 0.0f, 0.0f);
      // angular_velocity = 0.0f;
      // addForce((*ruler_list)[1-object_id], 1.0f,
          // glm::vec3(result.intersection.x, 0.0f, result.intersection.y), glm::vec3(12.5f, 0.0f, -1.5f));
    // };
  // }
}

void Ruler::updateVertices() {
  for(int i = 0; i < 4; i++) {
    glm::vec4 vertex_world_space = getModelMatrix() * glm::vec4(vertices_2d_box_modelspace[i], 1.0f);
    vertices_2d_box[i] = glm::vec2(vertex_world_space.x, vertex_world_space.z);
  }
}

void Ruler::updateNormals() {
  glm::vec2 prev = vertices_2d_box[3];
  for(int i = 0; i < 4; i++) {
    glm::vec2 norm = prev - vertices_2d_box[i];
    normals_2d_box[i] = glm::vec2(-norm.y, norm.x);
    prev = vertices_2d_box[i];
  }
}

glm::mat4 Ruler::getModelMatrix() {
  glm::mat4 model_matrix = glm::mat4(1.0f);
  model_matrix = glm::scale(model_matrix, glm::vec3(0.06f, 0.1f, 0.06f));
  model_matrix = glm::rotate(radian, glm::vec3(0.0f, 1.0f, 0.0f)) * model_matrix;
  return glm::translate(pos) * model_matrix;
}

glm::vec2 Ruler::getCenter() {
  return glm::vec2(pos.x, pos.z);
}

glm::vec2 Ruler::supportPoint(glm::vec2 direction) {
  float max_distance = glm::dot(vertices_2d_box[0], direction);
  glm::vec2 max_vertex = vertices_2d_box[0];
  for(int i = 1; i < vertices_2d_box.size(); i++) {
    float distance = glm::dot(vertices_2d_box[i], direction);
    if(distance > max_distance) {
      max_distance = distance;
      max_vertex = vertices_2d_box[i];
    }
  }
  // std::cout << "ruler " << object_id << "   max vertex: " << glm::to_string(max_vertex)
    // << "  direction: " << glm::to_string(direction) << std::endl;
  return max_vertex;
}
