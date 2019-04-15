#ifndef PHYSICS_H
#define PHYSICS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <cstdlib>
#include <vector>
#include <iostream>
#include <glm/ext.hpp>

extern float delta_time;

// Use cross product to get normal of a line
glm::vec2 triple_cross(glm::vec2 a, glm::vec2 b, glm::vec2 c);

// In this game, the only object type is Ruler
template<class T>
void addForce(T &object, float force, glm::vec3 direction, glm::vec3 r) {
  direction = glm::normalize(direction);
  glm::vec3 force_vector = force * direction;
  glm::vec3 velocity = object.getVelocity() + force_vector;
  object.setVelocity(velocity);
  // since ruler is moved on fixed y-axis, torque is computed in 2d demension
  float torque = r.x * force_vector.z + r.z * force_vector.x;
  float angular_acceleration = torque / object.getMomentOfInertia();
  float angular_velocity = object.getAngularVelocity() + angular_acceleration * delta_time;
  object.setAngularVelocity(angular_velocity);
}

// The following functions are all for collision detection
// In this game, only 2d collision detection is needed. 
struct CollisionResult {
  bool is_collided;
  glm::vec2 intersection;
};

struct Edge {
  int index;
  float distance;
  glm::vec2 normal;

  Edge(int _index, float _distance, glm::vec2 _normal) :
    index(_index), distance(_distance), normal(_normal) {};
};

Edge find_closest_edge(unsigned int winding, std::vector<glm::vec2> simplex);

// Collision detection by SAT(seperating axis theorm)
template<class T>
CollisionResult collision_detection_SAT(T &object_a, T &object_b) {
  CollisionResult result;
  result.is_collided = true;
  //TODO do circle collision check first to reduce cost

  //  calculate the axis of minimum penetration
  std::vector<glm::vec2> vertices = object_a.getVertices();
  std::vector<glm::vec2> norms = object_a.getNormals();
  float max_projection_a = -FLT_MAX;
  glm::vec2 min_support_a;
  for(int i = 0; i < norms.size(); i++) {
    glm::vec2 support_point = object_b.supportPoint(norms[i] * glm::vec2(-1, -1));
    float projection = glm::dot(support_point - vertices[i], norms[i]);
    if(projection > max_projection_a) {
      max_projection_a = projection;
      min_support_a = support_point;
    }
  }
  if(max_projection_a > 0) {
    result.is_collided = false;
    return result;
  }
  vertices = object_b.getVertices();
  norms = object_b.getNormals();
  float max_projection_b = -FLT_MAX;
  glm::vec2 min_support_b;
  for(int i = 0; i < norms.size(); i++) {
    glm::vec2 support_point = object_a.supportPoint(norms[i] * glm::vec2(-1, -1));
    float projection = glm::dot(support_point - vertices[i], norms[i]);
    if(projection > max_projection_b) {
      max_projection_b = projection;
      min_support_b = support_point;
    }
  }
  if(max_projection_b > 0) {
    result.is_collided = false;
    return result;
  }
  result.intersection = (max_projection_a > max_projection_b) ? min_support_a : min_support_b;
  return result;
}

// TODO
// Collision detection by GJK algorithm, uncompleted
// penetration vector is calculated by EPA method(expanding polytype algorithm)
template<class T>
CollisionResult collision_detection_GJK(T &object_a, T &object_b) {
  CollisionResult result;
  std::vector<glm::vec2> simplex;
  // detection status: 0 - no intersection, 1 - found intersection, 2 - still finding
  int status = 2;
  glm::vec2 direction;
  // gjk algorithm
  while (status == 2) {
    switch(simplex.size()) {
      case 0:
        // point a: choose the direction that connect two objects' center
        direction = object_b.getCenter() - object_a.getCenter();
        break;
      case 1:
        // point b: choose the reverse direction of first simplex
        direction = direction * -1;
        break;
      case 2: {
        // point c: choose the direction which perpendicular the line ba
        glm::vec2 ab = simplex[1] - simplex[0];
        glm::vec2 a0 = simplex[0] * -1;
        direction = triple_cross(ab, a0, ab);
        break;
      }
      case 3: {
        // caculate if origin is inside the triangle abc
        glm::vec2 c0 = simplex[2] * -1;
        glm::vec2 cb = simplex[1] - simplex[2];
        glm::vec2 ca = simplex[0] - simplex[2];

        glm::vec2 cb_normal = triple_cross(ca, cb, cb);
        glm::vec2 ca_normal = triple_cross(cb, ca, ca);

        // caculate if the normal and c0 is the same direction
        if(glm::dot(cb_normal, c0) > 0) {
          simplex.pop_back();
          direction = cb_normal;
        }
        else if(glm::dot(ca_normal, c0) > 0) {
          simplex.pop_back();
          direction = ca_normal;
        }
        else {
          status = 1;
        }
        break;
      }
    }
    if(status != 1) {
      // add support vertex
      glm::vec2 minkowski_diff = object_a.supportPoint(direction) 
        - object_b.supportPoint(-1 * direction);
      // std::cout << "direction: " << glm::to_string(direction) << std::endl;
      // std::cout << "diff: " << glm::to_string(minkowski_diff) << std::endl;
      simplex.push_back(minkowski_diff);
      // std::cout << "num: " << simplex.size() << std::endl;
      // for(int i = 0; i < simplex.size(); i++) {
        // std::cout << "no: " << i  << " : " <<  glm::to_string(simplex[i]) << std::endl;
      // }
      if(glm::dot(direction, minkowski_diff) > 0) {
        status = 2;
      }
      else {
        status = 0;
      }
    }
  }
  result.is_collided = (status == 1);

  // EPA
  if(result.is_collided == true) {
    glm::vec2 intersection;
    // calculate winding number of polygon
    float e0 = (simplex[1].x - simplex[0].x) * (simplex[1].y + simplex[0].y);
    float e1 = (simplex[2].x - simplex[1].x) * (simplex[2].y + simplex[1].y);
    float e2 = (simplex[0].x - simplex[2].x) * (simplex[0].y + simplex[2].y);
    // 0 - clockwise, 1 - counter clockwise
    unsigned int winding = 0;
    if(e0 + e1 + e2 < 0) winding = 1;
    for(int i = 0; i < 32; i++) {
      Edge edge = find_closest_edge(winding, simplex);
      glm::vec2 minkowski_diff = object_a.supportPoint(edge.normal) 
        - object_b.supportPoint(-1 * edge.normal);
      float distance = glm::dot(minkowski_diff, edge.normal);

      intersection = edge.normal;
      intersection = distance * intersection;
      if(abs(distance - edge.distance) <= 0.000001) {
        result.intersection = intersection;
        return result;
      }
      else {
        std::vector<glm::vec2>::iterator iterator = simplex.begin();
        simplex.insert(iterator + edge.index, minkowski_diff);
      }
    }
    result.intersection = intersection;
  }

  return result;
}

#endif
