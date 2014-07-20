#version 330 core

in vec4 vertexPos;

uniform mat4 projection;
uniform mat4 view;
uniform float time;
uniform vec3 mainColor;

uniform sampler2D reflectionTex;

out vec4 color;

void main() {
	vec2 vDeviceReflection = vertexPos.st / vertexPos.q;
	vec2 vTexReflection = vec2(0.5, 0.5) + 0.5 * vDeviceReflection;
  vec4 result = texture2D (reflectionTex, vTexReflection * vec2(-1, 1));

	color = vec4(result.xyz, 0.8);
}
