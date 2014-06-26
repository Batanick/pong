#version 330 core

in vec2 fragmentUv;

uniform sampler2D texture;

out vec3 color;

void main() {
  color = texture2D( texture, fragmentUv ).rgb;
}