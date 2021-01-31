//GLSL3.3�g�p
#version 330

//�e�N�X�`�����W
in vec2 fragTexCoord;

in vec4 vertexColor;

//�ʒu���W
out vec4 outColor;

//�e�N�X�`��
uniform sampler2D uTexture;

void main()
{
  vec4 texColor = texture(uTexture, fragTexCoord);

  //�o�͂���F��ɐݒ�
  outColor = texColor;
}