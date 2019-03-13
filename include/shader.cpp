#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "shader.h"

GLuint loadShaders(const char* vertex_shader_path, const char* fragment_shader_path) {
  GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

  // load shader codes from path
  std::ifstream vertex_shader_stream(vertex_shader_path, std::ios::in);
  std::string vertex_shader_code;
  if(vertex_shader_stream.is_open()) {
    std::stringstream str_stream;
    str_stream << vertex_shader_stream.rdbuf();
    vertex_shader_code = str_stream.str();
    vertex_shader_stream.close();
  }
  else {
    printf("Fail to load vertex shader file in path: %s\n", vertex_shader_path);
    getchar();
    return 0;
  }

  std::ifstream fragment_shader_stream(fragment_shader_path, std::ios::in);
  std::string fragment_shader_code;
  if(fragment_shader_stream.is_open()) {
    std::stringstream str_stream;
    str_stream << fragment_shader_stream.rdbuf();
    fragment_shader_code = str_stream.str();
    fragment_shader_stream.close();
  }
  else {
    printf("Fail to load fragment shader file in path: %s\n", fragment_shader_path);
    getchar();
    return 0;
  }

  GLint result = GL_FALSE;
  int log_length;

  // compile shader codes
  printf("start compiling vertex shader.\n");
  char const* vertex_code_pointer = vertex_shader_code.c_str();
  glShaderSource(vertex_shader_id, 1, &vertex_code_pointer, NULL);
  glCompileShader(vertex_shader_id);

  printf("start compiling fragment shader.\n");
  char const* fragment_code_pointer = fragment_shader_code.c_str();
  glShaderSource(fragment_shader_id, 1, &fragment_code_pointer, NULL);
  glCompileShader(fragment_shader_id);

  // check shader compiling status
  glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &log_length);
  if(log_length > 0) {
    std::vector<char> err_msg(log_length + 1);
    glGetShaderInfoLog(vertex_shader_id, log_length, NULL, &err_msg[0]);
    printf("%s\n", &err_msg[0]);
  }
  glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &log_length);
  if(log_length > 0) {
    // if log length is more than 0, get error log
    std::vector<char> err_msg(log_length + 1);
    glGetShaderInfoLog(fragment_shader_id, log_length, NULL, &err_msg[0]);
    printf("%s\n", &err_msg[0]);
  }

  // link shader program
  printf("start linking shader program\n");
  GLuint program_id = glCreateProgram();
  glAttachShader(program_id, vertex_shader_id);
  glAttachShader(program_id, fragment_shader_id);
  glLinkProgram(program_id);

  // check the program
  glGetProgramiv(program_id, GL_LINK_STATUS, &result);
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
  if(log_length > 0) {
    std::vector<char> err_msg(log_length + 1);
    glGetProgramInfoLog(program_id, log_length, NULL, &err_msg[0]);
    printf("%s\n", &err_msg[0]);
  }

  glDetachShader(program_id, vertex_shader_id);
  glDetachShader(program_id, fragment_shader_id);

  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);

  return program_id;
}
