#include "ruler.h"

Ruler::Ruler(glm::vec3 p) : pos(p) {}

bool Ruler::initialize(std::vector<glm::vec3>& vertices, 
  std::vector<glm::vec2>& uv_vertices, std::vector<glm::vec3>& normal_vectices) {
  bool result = load_obj("model/ruler2.obj", vertices, uv_vertices, normal_vectices);

  return result;
}

glm::mat4 Ruler::getModelMatrix() {
  glm::mat4 model_matrix = glm::mat4(1.0f);
  model_matrix = glm::scale(model_matrix, glm::vec3(0.06f, 0.1f, 0.06f));
  model_matrix = glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * model_matrix;
  return glm::translate(pos) * model_matrix;
}
