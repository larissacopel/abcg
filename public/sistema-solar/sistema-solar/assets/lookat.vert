#version 410 core

layout(location = 0) in vec3 inPosition;

uniform vec4 color;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec4 fragColor;

void main() {
  vec4 posEyeSpace = viewMatrix * modelMatrix * vec4(inPosition, 1);

  float i = 1.0 - (-posEyeSpace.z / 12.0);
  fragColor = vec4(i, i, i, 1) * color;

  gl_Position = projMatrix * posEyeSpace;
}