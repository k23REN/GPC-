#include "Renderer.h"

#include <GL/glew.h>

#include <algorithm>

#include "MeshComponent.h"
#include "SpriteComponent.h"

Renderer::Renderer(Game* a_game) : m_pGame(a_game), m_pSpriteShader(nullptr) {}

Renderer::~Renderer() {}

bool Renderer::Initialize(float a_screenWidth, float a_screenHeight) {
  m_screenWidth = a_screenWidth;
  m_screenHeight = a_screenHeight;

  // OpenGL�����ݒ�
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    //�o�[�W������3.3�Ɏw��
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // RGBA��8�r�b�g�̃J���[�o�b�t�@���g��
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    // Z�o�b�t�@
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    //�_�u���o�b�t�@��L���ɂ���
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    //�n�[�h�E�F�A�A�N�Z�����[�V�������g��
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  }

  m_pWindow =
      SDL_CreateWindow("OpenGL", 100, 100, static_cast<int>(m_screenWidth),
                       static_cast<int>(m_screenHeight), SDL_WINDOW_OPENGL);
  if (!m_pWindow) {
    SDL_Log("CreateWindow Failed: %s", SDL_GetError());
    return false;
  }

  m_context = SDL_GL_CreateContext(m_pWindow);

  // GLEW������
  {
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
      SDL_Log("GLEW Initialize Failed");
      return false;
    }

    glGetError();
  }

  if (!LoadShaders()) {
    SDL_Log("LoadShaders Failed");
    return false;
  }

  CreateSpriteVerts();

  m_pointLight[0].m_position = Vector3(150.0f, -200.0f, 10.0f);
  m_pointLight[1].m_position = Vector3(100.0f, -300.0f, 10.0f);
  m_pointLight[2].m_position = Vector3(300.0f, 100.0f, 10.0f);
  m_pointLight[3].m_position = Vector3(100.0f, 100.0f, 10.0f);
  m_pointLight[0].m_diffuseColor = Vector3(2.0f, 0.0f, 0.0f);
  m_pointLight[1].m_diffuseColor = Vector3(0.0f, 2.0f, 0.0f);
  m_pointLight[2].m_diffuseColor = Vector3(0.0f, 0.0f, 2.0f);
  m_pointLight[3].m_diffuseColor = Vector3(1.0f, 0.8f, 0.8f);
  m_pointLight[0].m_specColor = Vector3(1.0f, 0.0f, 0.0f);
  m_pointLight[1].m_specColor = Vector3(0.0f, 1.0f, 0.0f);
  m_pointLight[2].m_specColor = Vector3(0.0f, 0.0f, 1.0f);
  m_pointLight[3].m_specColor = Vector3(1.0f, 0.8f, 0.8f);

  for (int i = 0; i < PointLightNum; i++) {
    m_pointLight[i].m_radius = 600.0f;
  }

  return true;
}

void Renderer::Shutdown() {
  m_pSpriteVerts = nullptr;
  m_pSpriteShader->Unload();
  m_pSpriteShader = nullptr;
  m_pMeshShader->Unload();
  m_pMeshShader = nullptr;
  m_pPhoneShader->Unload();
  m_pPhoneShader = nullptr;
  m_pSkinnedShader->Unload();
  m_pSkinnedShader = nullptr;
  SDL_GL_DeleteContext(m_context);
  SDL_DestroyWindow(m_pWindow);
}

void Renderer::UnloadData() {
  //! �e�N�X�`���j��
  for (auto i : m_textures) {
    i.second->Unload();
    i.second = nullptr;
  }
  m_textures.clear();

  //! ���b�V���j��
  for (auto i : m_meshes) {
    i.second->Unload();
    delete i.second;
  }
  m_meshes.clear();
}

void Renderer::Draw() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  //! �J���[�o�b�t�@�Ɛ[�x�o�b�t�@�����N���A
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //! �[�x�o�b�t�@�L����
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);

  m_pMeshShader->SetActive();

  m_pMeshShader->SetMatrixUniform("uViewProj", m_view * m_projection);

  SetLightUniforms(m_pMeshShader);
  for (auto mc : m_meshComps["BasicMesh"]) {
    if (mc == nullptr) continue;
    mc->Draw(m_pMeshShader);
  }

  m_pPhoneShader->SetActive();

  m_pPhoneShader->SetMatrixUniform("uViewProj", m_view * m_projection);

  SetLightUniforms(m_pPhoneShader);
  for (auto mc : m_meshComps["Phong"]) {
    if (mc == nullptr) continue;
    mc->Draw(m_pPhoneShader);
  }

  //! �X�L�����b�V��������΁A�����ŕ`�悷��
  {
    m_pSkinnedShader->SetActive();

    //! �r���[�ˉe�s����X�V
    m_pSkinnedShader->SetMatrixUniform("uViewProj", m_view * m_projection);

    //! ���C�e�B���O��uniform���X�V
    SetLightUniforms(m_pSkinnedShader);
    for (auto sk : m_skeletalMeshes) {
      if (sk->GetVisible()) {
        sk->Draw(m_pSkinnedShader);
      }
    }
  }

  glDisable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

  m_pSpriteShader->SetActive();
  m_pSpriteVerts->SetActive();
  for (auto sprite : m_sprites) {
    sprite->Draw(m_pSpriteShader);
  }

  SDL_GL_SwapWindow(m_pWindow);
}

void Renderer::AddSprite(SpriteComponent* a_sprite) {
  int myDrawOrder = a_sprite->GetDrawOrder();
  auto iter = m_sprites.begin();
  for (; iter != m_sprites.end(); ++iter) {
    if (myDrawOrder < (*iter)->GetDrawOrder()) {
      break;
    }
  }

  m_sprites.insert(iter, a_sprite);
}

void Renderer::RemoveSprite(SpriteComponent* a_sprite) {
  auto iter = std::find(m_sprites.begin(), m_sprites.end(), a_sprite);
  m_sprites.erase(iter);
}

void Renderer::AddMeshComp(MeshComponent* a_mesh) {
  //!�X�L�����b�V��
  if (a_mesh->GetIsSkeletal()) {
    SkeletalMeshComponent* skMesh = static_cast<SkeletalMeshComponent*>(a_mesh);
    m_skeletalMeshes.emplace_back(skMesh);
  } else {  //!�ʏ탁�b�V��
    m_meshComps[a_mesh->GetMesh()->GetShaderName().data()].emplace_back(a_mesh);
  }
}

void Renderer::RemoveMeshComp(MeshComponent* a_mesh) {
  //!�X�L�����b�V��
  if (a_mesh->GetIsSkeletal()) {
    SkeletalMeshComponent* skMesh = static_cast<SkeletalMeshComponent*>(a_mesh);
    auto iter =
        std::find(m_skeletalMeshes.begin(), m_skeletalMeshes.end(), skMesh);
    m_skeletalMeshes.erase(iter);
  } else {  //!�ʏ탁�b�V��
    auto shaderName = a_mesh->GetMesh()->GetShaderName().data();
    auto iter = std::find(m_meshComps[shaderName].begin(),
                          m_meshComps[shaderName].end(), a_mesh);
    m_meshComps[shaderName].erase(iter);
  }
}

Sptr<Texture> Renderer::GetTexture(const std::string& a_fileName) {
  Sptr<Texture> tex = nullptr;
  auto iter = m_textures.find(a_fileName);
  if (iter != m_textures.end()) {
    tex = iter->second;
  } else {
    tex = std::make_shared<Texture>();
    if (tex->Load(a_fileName)) {
      m_textures.emplace(a_fileName, tex);
    } else {
      tex = nullptr;
    }
  }
  return tex;
}

Mesh* Renderer::GetMesh(const std::string& a_fileName) {
  Mesh* m = nullptr;
  auto iter = m_meshes.find(a_fileName);
  if (iter != m_meshes.end()) {
    m = iter->second;
  } else {
    m = new Mesh();
    if (m->Load(a_fileName, this)) {
      m_meshes.emplace(a_fileName, m);
    } else {
      delete m;
      m = nullptr;
    }
  }
  return m;
}

bool Renderer::LoadShaders() {
  m_pSpriteShader = std::make_shared<Shader>();
  if (!m_pSpriteShader->Load("../Shader/Sprite.vert",
                             "../Shader/Sprite.frag")) {
    return false;
  }

  m_pSpriteShader->SetActive();

  Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.0f, 768.0f);
  m_pSpriteShader->SetMatrixUniform("uViewProj", viewProj);

  m_pPhoneShader = std::make_shared<Shader>();
  if (!m_pPhoneShader->Load("../Shader/Phong.vert", "../Shader/Phong.frag")) {
    return false;
  }

  m_pPhoneShader->SetActive();
  //! �J�����̍s��̍���
  m_view = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
  m_projection = Matrix4::CreatePerspectiveFOV(
      Math::ToRadians(70.0f), m_screenWidth, m_screenHeight, 25.0f, 10000.0f);
  m_pPhoneShader->SetMatrixUniform("uViewProj", m_view * m_projection);

  m_pMeshShader = std::make_shared<Shader>();
  if (!m_pMeshShader->Load("../Shader/BasicMesh.vert",
                           "../Shader/BasicMesh.frag")) {
    return false;
  }

  m_pMeshShader->SetActive();
  m_pMeshShader->SetMatrixUniform("uViewProj", m_view * m_projection);

  //! �X�L�����b�V������
  {
    m_pSkinnedShader = std::make_shared<Shader>();
    if (!m_pSkinnedShader->Load("../Shader/Skinned.vert", "../Shader/Phong.frag")) {
      return false;
    }

    m_pSkinnedShader->SetActive();
    //! �s��̍���
    m_view =
        Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
    m_projection = Matrix4::CreatePerspectiveFOV(
        Math::ToRadians(70.0f), m_screenWidth, m_screenHeight, 10.0f, 10000.0f);
    m_pSkinnedShader->SetMatrixUniform("uViewProj", m_view * m_projection);
  }
  return true;
}

void Renderer::CreateSpriteVerts() {
  float vertices[] = {
      -0.5f, 0.5f,  0.f, 0.f, 0.f, 0.0f, 0.f, 0.f,  //! ����
      0.5f,  0.5f,  0.f, 0.f, 0.f, 0.0f, 1.f, 0.f,  //! �E��
      0.5f,  -0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f,  //! �E��
      -0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f   //! ����
  };

  unsigned int indices[] = {0, 1, 2, 2, 3, 0};

  m_pSpriteVerts = std::make_shared<VertexArray>(vertices, 4, VertexArray::PosNormTex, indices, 6);
}

void Renderer::SetLightUniforms(Sptr<Shader> a_shader) {
  Matrix4 invView = m_view;
  invView.Invert();
  a_shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
  //! ����
  a_shader->SetVectorUniform("uAmbientLight", m_ambientLight);
  //! ���s��
  a_shader->SetVectorUniform("uDirLight.mDirection", m_dirLight.m_direction);
  a_shader->SetVectorUniform("uDirLight.mDiffuseColor",
                             m_dirLight.m_diffuseColor);
  a_shader->SetVectorUniform("uDirLight.mSpecColor", m_dirLight.m_specColor);

  //! �_����
  {
    a_shader->SetVectorUniform("uPointLight[0].mPosition",
                               m_pointLight[0].m_position);
    a_shader->SetVectorUniform("uPointLight[1].mPosition",
                               m_pointLight[1].m_position);
    a_shader->SetVectorUniform("uPointLight[2].mPosition",
                               m_pointLight[2].m_position);
    a_shader->SetVectorUniform("uPointLight[3].mPosition",
                               m_pointLight[3].m_position);

    a_shader->SetVectorUniform("uPointLight[0].mDiffuseColor",
                               m_pointLight[0].m_diffuseColor);
    a_shader->SetVectorUniform("uPointLight[1].mDiffuseColor",
                               m_pointLight[1].m_diffuseColor);
    a_shader->SetVectorUniform("uPointLight[2].mDiffuseColor",
                               m_pointLight[2].m_diffuseColor);
    a_shader->SetVectorUniform("uPointLight[3].mDiffuseColor",
                               m_pointLight[3].m_diffuseColor);

    a_shader->SetVectorUniform("uPointLight[0].mSpecColor",
                               m_pointLight[0].m_specColor);
    a_shader->SetVectorUniform("uPointLight[1].mSpecColor",
                               m_pointLight[1].m_specColor);
    a_shader->SetVectorUniform("uPointLight[2].mSpecColor",
                               m_pointLight[2].m_specColor);
    a_shader->SetVectorUniform("uPointLight[3].mSpecColor",
                               m_pointLight[3].m_specColor);

    a_shader->SetFloatUniform("uPointLight[0].mRadius",
                              m_pointLight[0].m_radius);
    a_shader->SetFloatUniform("uPointLight[1].mRadius",
                              m_pointLight[1].m_radius);
    a_shader->SetFloatUniform("uPointLight[2].mRadius",
                              m_pointLight[2].m_radius);
    a_shader->SetFloatUniform("uPointLight[3].mRadius",
                              m_pointLight[3].m_radius);
  }
}

Vector3 Renderer::Unproject(const Vector3& a_screenPoint) const {
  //! screenPoint��-1����+1�܂ł̃f�o�C�X���W�ɕϊ�����
  Vector3 deviceCoord = a_screenPoint;
  deviceCoord.x /= (m_screenWidth)*0.5f;
  deviceCoord.y /= (m_screenHeight)*0.5f;

  //! �t�ˉe�s��Ńx�N�g����ϊ�����
  Matrix4 unprojection = m_view * m_projection;
  unprojection.Invert();
  return Vector3::TransformWithPerspDiv(deviceCoord, unprojection);
}

void Renderer::GetScreenDirection(Vector3& a_outStart,
                                  Vector3& a_outDir) const {
  //! �n�_���ߐڕ��ʂł̉�ʂ̒��S�ɂ���
  Vector3 screenPoint(0.0f, 0.0f, 0.0f);
  a_outStart = Unproject(screenPoint);
  //! �I�_���ߐڕ��ʂƉ������ʂ̊Ԃ̉�ʂ̒��S�ɒ�߂�
  screenPoint.z = 0.9f;
  Vector3 end = Unproject(screenPoint);
  //! �����x�N�g�������߂�
  a_outDir = end - a_outStart;
  a_outDir.Normalize();
}
