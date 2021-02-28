#pragma once
class VertexArray {
 public:
  enum Layout 
  { 
      PosNormTex,
      PosNormSkinTex
  };

  VertexArray(const void* a_verts, unsigned int a_numVerts,Layout a_layout,
              const unsigned int* a_indices, unsigned int a_numIndices);
  ~VertexArray();

 /**
   * @fn
   * ���_�z����A�N�e�B�u�ɂ���
   * @brief �`����\�ɐݒ�
   * @return �Ȃ�
   */
  void SetActive();

  [[nodiscard]] unsigned int GetNumIndices() const { return m_numIndices; }
  [[nodiscard]] unsigned int GetNumVerts() const { return m_numVerts; }

 private:
  //!���_��
  unsigned int m_numVerts;
  //!�C���f�b�N�X��
  unsigned int m_numIndices;
  //!���_�o�b�t�@��ID
  unsigned int m_vertexBuffer;
  //!�C���f�b�N�X�o�b�t�@��ID
  unsigned int m_indexBuffer;
  //!���_�z���ID
  unsigned int m_vertexArray;
};