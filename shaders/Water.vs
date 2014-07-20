#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 vp;
uniform vec3 cameraPos;

uniform mat4 projection;
uniform mat4 view;
 
out vec3 fragmentPos;
 
void main() {
  vec3 vPosition = pos + cameraPos;
  gl_Position = projection * view * vec4(vPosition, 1);
}