#version 330 core

layout(location=0) in vec3 vertex_coord_model;
layout(location=1) in vec2 uv_vertex_coord;
layout(location=2) in vec3 vertex_normal_model;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 light_position_world;

out vec2 uv_coord;
out vec3 position_world;
out vec3 vertex_normal_camera;
out vec3 eye_direction_camera;
out vec3 light_direction_camera;

void main() {
  gl_Position = MVP * vec4(vertex_coord_model, 1.0);
  position_world = (M * vec4(vertex_coord_model, 1.0)).xyz;
  vec3 vertex_coord_camera = (V * M * vec4(vertex_coord_model, 1)).xyz;
  eye_direction_camera = vec3(0, 0, 0) - vertex_coord_camera;

  vec3 light_position_camera = (V * vec4(light_position_world, 1)).xyz;
  light_direction_camera = light_position_camera + eye_direction_camera;

  vertex_normal_camera = (transpose(inverse(V * M)) * vec4(vertex_normal_model, 0)).xyz;
  uv_coord = uv_vertex_coord;
}
