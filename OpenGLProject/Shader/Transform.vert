//GLSL3.3�g�p
#version 330

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

//�ʒu���W
in vec3 inPosition;

void main()
{
  vec4 pos = vec4(inPosition, 1.0f);
  gl_Position = pos * uWorldTransform * uViewProj;
}