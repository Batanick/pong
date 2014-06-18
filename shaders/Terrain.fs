#version 330 core

in float u;
in vec3 normal;
in vec3 light;

uniform sampler1D terrainTexture;
uniform vec3 cameraPos;


out vec3 color;

void main() {
  vec3 texColor = texture( terrainTexture, u ).rgb;

  float cosTheta = clamp( dot( normal,light ), 0,1 );
  color = texColor * (cosTheta * 0.7 + 0.3);
}