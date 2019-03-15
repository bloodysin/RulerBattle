#include "model_loader.h"

bool load_obj(const char* path, VecArray& vertices, std::vector<glm::vec2>& uv_vertices, 
    VecArray& normal_vectices) {
  printf("start loading obj file: %s.\n", path);

  FILE *file = fopen(path, "r");
  if(file == NULL) {
    printf("Fail to open obj file: %s\n", path);
    getchar();
    return false;
  }

  VecArray vertex_indices, normal_indices;
  std::vector<glm::vec2> uv_indices;

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
    else if(strcmp(header, "vt") == 0) {
      // texture uv vertex index
      glm::vec2 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y);
      // inverse y corrdinate since png texture
      vertex.y = -vertex.y;
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
      uv_vertices.push_back(uv_indices[temp_indices[1] - 1]);
      uv_vertices.push_back(uv_indices[temp_indices[4] - 1]);
      uv_vertices.push_back(uv_indices[temp_indices[7] - 1]);
      normal_vectices.push_back(normal_vectices[temp_indices[2] - 1]);
      normal_vectices.push_back(normal_vectices[temp_indices[5] - 1]);
      normal_vectices.push_back(normal_vectices[temp_indices[8] - 1]);
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
  // printf("uv vertices size: %d\n", uv_vertices.size());
  // for(unsigned int i = 0; i < uv_vertices.size(); i++) {
    // std::cout << glm::to_string(uv_vertices[i]) << std::endl;
  // }

  fclose(file);
  printf("Finish loading obj file %s.\n", path);
  return true;
}

GLuint load_texture_png(const char* path) {
  printf("Start loading texture(png format): %s.\n", path);

  GLuint texture_id;
  unsigned char* texture_buffer;
  unsigned int width, height;

  lodepng_decode24_file(&texture_buffer, &width, &height, path);

  // create texture
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_buffer);

  // add linear filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);

  printf("Finish loading texture(png format): %s.\n", path);
  return texture_id;
}
