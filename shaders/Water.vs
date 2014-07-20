#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 vp;
uniform vec3 cameraPos;

uniform mat4 projection;
uniform mat4 view;
 
out vec4 vertexPos;
 
void main() {
  vec4 projPos = projection * view * vec4(pos, 1);
  gl_Position = projPos;
  vertexPos = projPos;
}