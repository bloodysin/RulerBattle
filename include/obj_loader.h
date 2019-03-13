#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <stdio.h>
#include <string.h>
#include <vector>

#include <iostream>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

typedef std::vector<glm::vec3> VecArray;

bool load_obj(const char* path, VecArray& vertices, VecArray& uv_vertices, VecArray& normal_vectices) {
  printf("start loading model %s.\n", path);

  FILE *file = fopen(path, "r");
  if(file == NULL) {
    printf("Fail to open obj file: %s\n", path);
    getchar();
    return false;
  }

  VecArray vertex_indices, uv_indices, normal_indices;

  // load vertex info from .obj file
  while(1) {
    char header[128];

    if(fscanf(file, "%s", header) == EOF) {
      break;
    }

    if(strcmp(header, "v") == 0) {
      // vertex index
      glm::vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      // std::cout << glm::to_string(vertex) << std::endl;
      vertex_indices.push_back(vertex);
    }
    else if(strcmp(header, "vn") == 0) {
      // normal vertex index
      glm::vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      normal_vectices.push_back(vertex);
    }
    else if(strcmp(header, "v") == 0) {
      // texture uv vertex index
      glm::vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      uv_indices.push_back(vertex);
    }
    else if(strcmp(header, "f") == 0) {
      int temp_indices[9];
      // for the case of three vertices on one face
      fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
          &temp_indices[0], &temp_indices[1], &temp_indices[2],
          &temp_indices[3], &temp_indices[4], &temp_indices[5],
          &temp_indices[6], &temp_indices[7], &temp_indices[8]
          );

      vertices.push_back(vertex_indices[temp_indices[0] - 1]);
      vertices.push_back(vertex_indices[temp_indices[3] - 1]);
      vertices.push_back(vertex_indices[temp_indices[6] - 1]);
      uv_vertices.push_back(vertex_indices[temp_indices[1] - 1]);
      uv_vertices.push_back(vertex_indices[temp_indices[4] - 1]);
      uv_vertices.push_back(vertex_indices[temp_indices[7] - 1]);
      normal_vectices.push_back(vertex_indices[temp_indices[2] - 1]);
      normal_vectices.push_back(vertex_indices[temp_indices[5] - 1]);
      normal_vectices.push_back(vertex_indices[temp_indices[8] - 1]);
    }
    else {
      // other header, ignore and go to next line
      char buffer[128];
      fgets(buffer, 128, file);
    }
  }
  // printf("vertices size: %d\n", vertices.size());
  // for(unsigned int i = 0; i < vertices.size(); i++) {
    // std::cout << glm::to_string(vertices[i]) << std::endl;
  // }

  fclose(file);
  printf("Finish loading model %s.\n", path);
  return true;
}

#endif
