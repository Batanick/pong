#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

uniform mat4 projection;
uniform mat4 view;
uniform float heightScale;
uniform vec3 lightDir;

out float u;
out vec3 normal;
out vec3 light;
out vec3 xyz;

void main() {
	gl_Position = projection * view * vec4( vertexPosition, 1 );
	u = clamp( vertexPosition.y / heightScale, 0.0, 1.0 );
  normal = (view * vec4(vertexNormal, 0)).xyz;
  light = (view * vec4(lightDir, 0)).xyz;
  xyz = vertexPosition.xyz;
}