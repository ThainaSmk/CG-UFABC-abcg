#version 300 es

in mediump vec3 fragTexCoord;

out mediump vec4 outColor;

uniform samplerCube skyTex;

void main() { outColor = texture(skyTex, fragTexCoord); }