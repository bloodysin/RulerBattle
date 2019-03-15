#ifndef RULER_H
#define RULER_H

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "model_loader.h"


class Ruler {
  private:
    glm::vec3 pos;
    std::vector<glm::vec3> vertices;
  public:
    Ruler(glm::vec3 p);
    bool initialize(std::vector<glm::vec3>& vertices, 
        std::vector<glm::vec2>& uv_vertices, std::vector<glm::vec3>& normal_vectices);
    glm::mat4 getModelMatrix();
    void move();
};

#endif
