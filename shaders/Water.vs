#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 vp;
uniform vec3 lightDirection;

uniform mat4 projection;
uniform mat4 view;

out vec4 fPos; 
out vec4 pvPos;
out vec3 light;
 
void main() {
  fPos = vec4(pos, 1);
  pvPos = projection * view * fPos;
  light = (view * vec4(lightDirection, 0)).xyz;

  gl_Position = pvPos;
}