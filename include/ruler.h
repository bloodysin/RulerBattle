#ifndef RULER_H
#define RULER_H

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Ruler {
  private:
    glm::vec3 pos;
    GLfloat vertices[];
  public:
    Ruler(glm::vec3 p);
    void move();
    glm::mat4 getModelMatrix();
};

#endif
