#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

uniform vec3 lightDir;

out vec3 normalView;
out vec3 lightView;
out vec3 posView;
out vec2 uvCoord;

void main() {
  vec4 position = world * vec4( vertexPosition, 1 );

  normalView = (view * vec4(vertexNormal, 0)).xyz;
  lightView = (view * vec4(lightDir, 0)).xyz;
  posView = (view * position).xyz;
   
  uvCoord = uv;
  gl_Position = projection * view * position;
}