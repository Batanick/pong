#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D texture;
uniform vec3 fontColor;

void main() {
	vec4 v = vec4(texture2D( texture, uv ));
	v.r *= fontColor.r;
	v.g *= fontColor.g;
	v.b *= fontColor.b;
	color = v;	
}