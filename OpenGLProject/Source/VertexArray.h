#pragma once
class VertexArray {
 public:
  VertexArray(const float* a_verts, unsigned int a_numVerts,
              const unsigned int* a_indices, unsigned int a_numIndices);
  ~VertexArray();

    /**
   * @fn
   * 頂点配列をアクティブにする
   * @brief 描画を可能に設定
   * @return なし
   */
  void SetActive();

  [[nodiscard]] unsigned int GetNumIndices() const { return m_numIndices; }
  [[nodiscard]] unsigned int GetNumVerts() const { return m_numVerts; }

 private:
  //!頂点数
  unsigned int m_numVerts;
  //!インデックス数
  unsigned int m_numIndices;
  //!頂点バッファのID
  unsigned int m_vertexBuffer;
  //!インデックスバッファのID
  unsigned int m_indexBuffer;
  //!頂点配列のID
  unsigned int m_vertexArray;
};