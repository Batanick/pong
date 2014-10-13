#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

uniform mat4 projection;
uniform mat4 view;

uniform vec3 lightDir;

out vec3 normalView;
out vec3 lightView;
out vec3 posView;

void main() {
  vec4 position = vec4( vertexPosition, 1 );

  normalView = (view * vec4(vertexNormal, 0)).xyz;
  lightView = (view * vec4(lightDir, 0)).xyz;
  posView = (view * position).xyz;
   
  gl_Position = projection * view * position;

}