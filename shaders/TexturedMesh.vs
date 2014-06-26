#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;

uniform mat4 mvp;

out vec2 fragmentUv;

void main() {
	gl_Position = mvp * vec4( pos, 1 );

  fragmentUv = uv;
}