#version 300 es

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec4 inTangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform mediump vec4 lightPosWorldSpace;

out mediump vec2 fragTexCoord;
out mediump vec3 fragPObj;
out mediump vec3 fragTObj;
out mediump vec3 fragBObj;
out mediump vec3 fragNObj;

out mediump vec3 fragLEye;
out mediump vec3 fragVEye;

out mediump vec4 fragPosition;

void main() {
  vec3 PEye = (viewMatrix * modelMatrix * vec4(inPosition, 1.0)).xyz;
  vec3 LEye = (viewMatrix * normalize(lightPosWorldSpace - modelMatrix * vec4(inPosition, 1.0))).xyz;

  fragTexCoord = inTexCoord;

  fragPObj = inPosition;
  fragTObj = inTangent.xyz; 
  fragBObj = inTangent.w * cross(inNormal, inTangent.xyz);
  fragNObj = inNormal;

  fragLEye = LEye;
  fragVEye = -PEye;

  fragPosition = vec4(modelMatrix * vec4(inPosition, 1.0));

  gl_Position = projMatrix * vec4(PEye, 1.0);
}