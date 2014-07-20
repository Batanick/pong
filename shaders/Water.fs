#version 330 core

in vec3 fragmentPos;

uniform float time;
uniform vec3 mainColor;

out vec4 color;

void main() {
	  color = vec4(mainColor, 1);
}
