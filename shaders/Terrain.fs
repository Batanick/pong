#version 330 core

in float u;
in vec3 normal;
in vec3 light;
in vec3 xyz;

uniform sampler1D terrainTexture;
uniform vec3 cameraPos;

out vec4 color;

float snoise(vec3 v);

void main() {
  float texCoord = texture( terrainTexture, u ).a;
  vec3 texColor = texture( terrainTexture, texCoord ).rgb;

  float cosTheta = clamp( dot( normal,light ), 0, 1 );
  color = vec4(texColor * (cosTheta * 0.7 + 0.3), 1);
}