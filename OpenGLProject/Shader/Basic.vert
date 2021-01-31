//GLSL3.3使用
#version 330

//位置座標
in vec3 inPosition;

void main()
{
  gl_Position = vec4(inPosition);
}