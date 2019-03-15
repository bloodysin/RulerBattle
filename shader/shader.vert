#version 330 core

layout(location=0) in vec3 vertex_coord;
layout(location=1) in vec2 uv_vertex_coord;

uniform mat4 MVP;
out vec2 uv_coord;

void main() {
  gl_Position = MVP * vec4(vertex_coord, 1.0);
  uv_coord = uv_vertex_coord;
}
