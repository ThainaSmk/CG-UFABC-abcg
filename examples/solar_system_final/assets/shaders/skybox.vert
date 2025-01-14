#version 300 es

layout(location = 0) in vec3 inPosition;

out mediump vec3 fragTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main() {
  fragTexCoord = inPosition;

  vec4 P = projMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
  gl_Position = P.xyww;
}