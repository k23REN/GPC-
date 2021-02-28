#include "VertexArray.h"
#include "GL/glew.h"

VertexArray::VertexArray(const void* a_verts, unsigned int a_numVerts, Layout a_layout,
                         const unsigned int* a_indices, unsigned int a_numIndices) 
    	: m_numVerts(a_numVerts),
        m_numIndices(a_numIndices) {
  //!頂点配列生成
  {
    glGenVertexArrays(1, &m_vertexArray);

    //! IDと関連づける
    glBindVertexArray(m_vertexArray);
  }

  unsigned vertexSize = 8 * sizeof(float);
  if (a_layout == PosNormSkinTex) {
    vertexSize = 8 * sizeof(float) + 8 * sizeof(char);
  }

  //!頂点バッファ生成
  {
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, a_numVerts * vertexSize, a_verts, GL_STATIC_DRAW);
  }

  //!インデックスバッファ生成
  {
    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_numIndices * sizeof(unsigned int), a_indices, GL_STATIC_DRAW);
  }

  //!頂点レイアウト指定
  if (a_layout == PosNormTex) {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
                          reinterpret_cast<void*>(sizeof(float) * 3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
                          reinterpret_cast<void*>(sizeof(float) * 6));
  } else if (a_layout == PosNormSkinTex) {
    //! 位置：3個のfloat
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
    //! 法線：3個のfloat
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
                          reinterpret_cast<void*>(sizeof(float) * 3));
    //! スキニングのボーン(整数)
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE, vertexSize,
                           reinterpret_cast<void*>(sizeof(float) * 6));
    //! スキニングの重み(floatへ変換)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
        3, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize,
        reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 4));
    //! テクスチャ座標
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(
        4, 2, GL_FLOAT, GL_FALSE, vertexSize,
        reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 8));
  }
}

VertexArray::~VertexArray() 
{
  glCreateBuffers(1, &m_vertexArray);
  glCreateBuffers(1, &m_indexBuffer);
  glCreateVertexArrays(1, &m_vertexArray);
}

void VertexArray::SetActive() 
{ 
  glBindVertexArray(m_vertexArray);
}
