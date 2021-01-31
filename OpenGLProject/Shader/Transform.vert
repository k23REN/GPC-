//GLSL3.3使用
#version 330

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

//位置座標
in vec3 inPosition;

void main()
{
  vec4 pos = vec4(inPosition, 1.0f);
  gl_Position = pos * uWorldTransform * uViewProj;
}