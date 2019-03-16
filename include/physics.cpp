#include "physics.h"

glm::vec2 triple_cross(glm::vec2 a, glm::vec2 b, glm::vec2 c) {
  glm::vec3 A(a.x, a.y, 0);
  glm::vec3 B(b.x, b.y, 0);
  glm::vec3 C(c.x, c.y, 0);

  A = glm::cross(A, B);
  A = glm::cross(A, C);

  return glm::vec2(A.x, A.y);
}

Edge find_closest_edge(unsigned int winding, std::vector<glm::vec2> simplex){
  int closest_index = 0;
  glm::vec2 closest_normal;
  float closest_dist = 65536.0f;
  glm::vec2 line_vec;
  for(int i = 0; i < (int)simplex.size(); i++) {
    int j = i + 1;
    if(j >= (int)simplex.size()) j = 0;
    line_vec = simplex[j] - simplex[i];
    glm::vec2 normal;
    if(winding == 0)
      normal = glm::vec2(line_vec.y, -line_vec.x);
    else
      normal = glm::vec2(-line_vec.y, line_vec.x);
    normal = glm::normalize(normal);

    float dist = glm::dot(normal, simplex[i]);
    if(dist < closest_dist) {
      closest_dist = dist;
      closest_normal = normal;
      closest_index = j;
    }
  }

  return Edge(closest_index, closest_dist, closest_normal);
}
