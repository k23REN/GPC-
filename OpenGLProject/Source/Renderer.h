#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Math.h"
#include "Texture.h"
#include "Mesh.h"
//#include "Game.h"
#include "Shader.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"

struct DirectionalLight {
  Vector3 m_direction;
  Vector3 m_diffuseColor;
  Vector3 m_specColor;
};

struct PointLight {
  Vector3 m_position;
  Vector3 m_diffuseColor;
  Vector3 m_specColor;
  float m_radius = 0.0f;
};

class Renderer {
 public:
  Renderer(class Game* a_game);
  ~Renderer();

  bool Initialize(float a_screenWidth, float a_screenHeight);
  void Shutdown();
  void UnloadData();

  void Draw();

  /**
   * @fn
   * �X�v���C�g��z��ɒǉ����A�ꊇ�ŏ������s��
   * @brief �X�v���C�g��ǉ�����
   * @param (a_sprite) �ǉ�����X�v���C�g
   * @return �Ȃ�
   */
  void AddSprite(SpriteComponent* a_sprite);

  /**
   * @fn
   * �X�v���C�g��z�񂩂�폜����
   * @brief �X�v���C�g���폜����
   * @param (a_sprite) �폜����X�v���C�g
   * @return �Ȃ�
   */
  void RemoveSprite(SpriteComponent* a_sprite);

  void AddMeshComp(MeshComponent* a_mesh);
  void RemoveMeshComp(MeshComponent* a_mesh);

  Vector3 Unproject(const Vector3& a_screenPoint) const;
  void GetScreenDirection(Vector3& a_outStart, Vector3& a_outDir) const;

  ///
  ///�擾
  ///
  [[nodiscard]] Sptr<Texture> GetTexture(const std::string& a_fileName);
  [[nodiscard]] Mesh* GetMesh(const std::string& a_fileName);
  [[nodiscard]] DirectionalLight& GetDirectionalLight() { return m_dirLight; }
  [[nodiscard]] float GetScreenWidth() const { return m_screenWidth; }
  [[nodiscard]] float GetScreenHeight() const { return m_screenHeight; }

  ///
  ///�ݒ�
  ///
  void SetViewMatrix(const Matrix4& a_view) { m_view = a_view; }
  void SetAmbientLight(const Vector3& a_ambient) { m_ambientLight = a_ambient; }

 private:
  bool LoadShaders();
  void CreateSpriteVerts();
  void SetLightUniforms(Sptr<Shader> shader);

  std::unordered_map<std::string, Sptr<Texture>> m_textures;
  std::unordered_map<std::string, Mesh*> m_meshes;

  //! �X�v���C�g�z��
  std::vector<SpriteComponent*> m_sprites;

  //! ���b�V���z��
  std::unordered_map<std::string, std::vector<MeshComponent*>> m_meshComps;
  std::vector<SkeletalMeshComponent*> m_skeletalMeshes;

  Sptr<Game> m_pGame;

  //! �X�v���C�g�`��֌W
  Sptr<Shader> m_pSpriteShader;
  Sptr<VertexArray> m_pSpriteVerts;

  Sptr<Shader> m_pMeshShader = nullptr;
  Sptr<Shader> m_pPhoneShader = nullptr;
  Sptr<Shader> m_pSkinnedShader = nullptr;

  Matrix4 m_view;
  Matrix4 m_projection;

  //! ��ʂ̕��ƍ���
  float m_screenWidth = 0.0f;
  float m_screenHeight = 0.0f;

  static inline constexpr int PointLightNum = 4;
  Vector3 m_ambientLight;
  DirectionalLight m_dirLight;
  PointLight m_pointLight[PointLightNum];

  ///
  /// SDL�n�ϐ�
  ///
  SDL_Window* m_pWindow = nullptr;
  SDL_GLContext m_context = nullptr;
};