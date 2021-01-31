//GLSL3.3使用
#version 330

//テクスチャ座標
in vec2 fragTexCoord;

in vec4 vertexColor;

//位置座標
out vec4 outColor;

//テクスチャ
uniform sampler2D uTexture;

void main()
{
  vec4 texColor = texture(uTexture, fragTexCoord);

  //出力する色を青に設定
  outColor = texColor;
}