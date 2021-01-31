#include "VertexArray.h"
#include "GL/glew.h"

VertexArray::VertexArray(const float* a_verts, unsigned int a_numVerts,
                         const unsigned int* a_indices, unsigned int a_numIndices) 
    	: m_numVerts(a_numVerts),
        m_numIndices(a_numIndices) {
  //頂点配列生成
  {
    glGenVertexArrays(1, &m_vertexArray);

    // IDと関連づける
    glBindVertexArray(m_vertexArray);
  }

  //頂点バッファ生成
  {
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, a_numVerts * 8 * sizeof(float), a_verts, GL_STATIC_DRAW);
  }

  //インデックスバッファ生成
  {
    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_numIndices * sizeof(unsigned int), a_indices, GL_STATIC_DRAW);
  }

  //頂点レイアウト指定
  {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float) * 3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
                          reinterpret_cast<void*>(sizeof(float) * 6));
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
