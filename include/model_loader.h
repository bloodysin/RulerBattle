#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <stdio.h>
#include <string.h>
#include <vector>

#include <iostream>
#include <glm/ext.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "lodepng.h"

typedef std::vector<glm::vec3> VecArray;

bool load_obj(const char* path, VecArray& vertices, std::vector<glm::vec2>& uv_vertices, 
    VecArray& normal_vectices);

GLuint load_texture_png(const char* path);

#endif
