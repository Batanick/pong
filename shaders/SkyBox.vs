#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 mvp;
uniform vec3 cameraPos;
 
void main() {
   gl_Position = mvp * vec4(pos + cameraPos, 1);
}