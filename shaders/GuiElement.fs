#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D texture;
uniform vec3 oColor;

void main() {
  vec4 v = vec4(texture2D( texture, uv ));
  v.r *= oColor.r;
  v.g *= oColor.g;
  v.b *= oColor.b;
  color = v;
}