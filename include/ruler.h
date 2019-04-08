#ifndef RULER_H
#define RULER_H

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/epsilon.hpp>

#include "model_loader.h"
#include "physics.h"


class Ruler {
  private:
    unsigned int object_id;
    glm::vec3 pos;
    glm::vec3 velocity;
    float angular_velocity;
    float radian;
    float moment_of_inertia;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> vertices_2d_box_modelspace;
  public:
    Ruler(unsigned int id, glm::vec3 p, float r);
    glm::mat4 getModelMatrix();
    glm::vec3 getVelocity();
    void setVelocity(glm::vec3 _velocity);
    float getAngularVelocity();
    void setAngularVelocity(float _angular_velocity);
    float getMomentOfInertia();
    void move(glm::vec3 delta_pos, float delta_angle);
    void update();
    // for gjk collision detection
    glm::vec2 get_center();
    glm::vec2 support_function(glm::vec2 direction);
};

bool initialize_ruler(std::vector<glm::vec3>& vertices, 
    std::vector<glm::vec2>& uv_vertices, std::vector<glm::vec3>& normal_vectices);

#endif
