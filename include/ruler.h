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
    std::vector<glm::vec2> vertices_2d_box;
    std::vector<glm::vec2> normals_2d_box;
    std::vector<glm::vec3> vertices_2d_box_modelspace;
    glm::vec4 prev_move;
  public:
    Ruler(unsigned int id, glm::vec3 p, float r);
    glm::mat4 getModelMatrix();
    glm::vec3 getVelocity();
    void setVelocity(glm::vec3 _velocity);
    float getAngularVelocity();
    void setAngularVelocity(float _angular_velocity);
    float getMomentOfInertia();
    std::vector<glm::vec2> getVertices();
    std::vector<glm::vec2> getNormals();
    void move(glm::vec3 delta_pos, float delta_angle);
    void update();
    void updateVertices();
    void updateNormals();
    // for gjk collision detection
    glm::vec2 getCenter();
    glm::vec2 supportPoint(glm::vec2 direction);
};

bool initialize_ruler(std::vector<glm::vec3>& vertices, 
    std::vector<glm::vec2>& uv_vertices, std::vector<glm::vec3>& normal_vectices);

#endif
