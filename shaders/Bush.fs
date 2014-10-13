#version 330 core

in vec3 normalView;
in vec3 lightView;
in vec3 posView;

uniform vec3 vColor;

out vec3 color;

void main() {
  float cosTheta = clamp( dot( normalize(normalView), normalize(lightView) ), 0, 1 );
  color = vColor * (cosTheta * 0.7 + 0.3);
}