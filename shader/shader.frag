#version 330 core

in vec2 uv_coord;
in vec3 position_world;
in vec3 vertex_normal_camera;
in vec3 eye_direction_camera;
in vec3 light_direction_camera;

out vec3 color;

uniform sampler2D texture_sampler;
uniform vec3 light_position_world;
uniform vec3 material_ambient;

void main() {
  // color = vec3(0.96, 0.78, 0.56);
  // color = texture(texture_sampler, uv_coord).rgb;
  vec3 light_color = vec3(1, 1, 1);
  float light_power = 80.0f;

  vec3 material_diffuse_color = texture(texture_sampler, uv_coord).rgb;
  vec3 material_ambient_color = material_ambient * material_diffuse_color;
  vec3 material_specular_color = vec3(0.3, 0.3, 0.3);

  float distance = length(light_position_world - position_world);
  vec3 n = normalize(vertex_normal_camera);
  vec3 l = normalize(light_direction_camera);
  float cos_theta = clamp(dot(n, l), 0, 1);

  vec3 E = normalize(eye_direction_camera);
  vec3 R = reflect(-l, n);
  float cos_alpha = clamp(dot(E, R), 0, 1);

  color = material_ambient_color 
    + material_diffuse_color * light_color * light_power * cos_theta / (distance * distance)
    + material_specular_color * light_color * light_power * pow(cos_alpha, 5) / (distance * distance);
}
