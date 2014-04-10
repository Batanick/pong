#version 330 core

layout (location = 0) in vec3 vertexPosition;

uniform mat4 mvp;
uniform vec3 vColor;

void main(){
    vec4 v = vec4( vertexPosition, 1 ); 
	gl_Position = mvp * v;
}