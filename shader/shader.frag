#version 330 core

in vec2 uv_coord;

out vec3 color;

uniform sampler2D texture_sampler;

void main() {
  // color = vec3(0.96, 0.78, 0.56);
  color = texture(texture_sampler, uv_coord).rgb;
}
