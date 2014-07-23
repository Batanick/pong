#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

uniform mat4 projection;
uniform mat4 view;

uniform float heightScale;
uniform vec3 lightDir;
uniform vec4 waterLevel;

out float u;
out vec3 normal;
out vec3 light;
out vec3 xyz;

void main() {
  vec4 position = vec4( vertexPosition, 1 );

  gl_Position = projection * view * position;
  gl_ClipDistance[0] = dot( waterLevel, position );

  u = vertexPosition.y / heightScale;
  normal = (view * vec4(vertexNormal, 0)).xyz;
  light = (view * vec4(lightDir, 0)).xyz;
  xyz = vertexPosition.xyz;
}