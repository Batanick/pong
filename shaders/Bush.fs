#version 330 core

in vec3 normalView;
in vec3 lightView;
in vec3 posView;
in vec2 uvCoord;

uniform sampler2D texture;

out vec4 color;

void main() {
  float cosTheta = clamp( dot( normalize(normalView), normalize(lightView) ), 0, 1 );
  vec4 diffuse = texture2D( texture, uvCoord ); 
  color = vec4(diffuse.rgb * (cosTheta * 0.7 + 0.3), diffuse.a) ;
}