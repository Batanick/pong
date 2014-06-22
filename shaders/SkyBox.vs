#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 vp;
uniform vec3 cameraPos;

out vec3 fragmentPos;
 
void main() {
  vec3 vPosition = pos + cameraPos;
  gl_Position = vp * vec4(vPosition , 1);
  fragmentPos = vPosition;
}