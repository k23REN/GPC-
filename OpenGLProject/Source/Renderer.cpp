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

  // OpenGL初期設定
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    //バージョンを3.3に指定
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // RGBAは8ビットのカラーバッファを使う
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    // Zバッファ
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    //ダブルバッファを有効にする
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    //ハードウェアアクセラレーションを使う
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

  // GLEW初期化
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
  //! テクスチャ破棄
  for (auto i : m_textures) {
    i.second->Unload();
    i.second = nullptr;
  }
  m_textures.clear();

  //! メッシュ破棄
  for (auto i : m_meshes) {
    i.second->Unload();
    delete i.second;
  }
  m_meshes.clear();
}

void Renderer::Draw() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  //! カラーバッファと深度バッファ両方クリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //! 深度バッファ有効化
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

  //! スキンメッシュがあれば、ここで描画する
  {
    m_pSkinnedShader->SetActive();

    //! ビュー射影行列を更新
    m_pSkinnedShader->SetMatrixUniform("uViewProj", m_view * m_projection);

    //! ライティングのuniformを更新
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
  //!スキンメッシュ
  if (a_mesh->GetIsSkeletal()) {
    SkeletalMeshComponent* skMesh = static_cast<SkeletalMeshComponent*>(a_mesh);
    m_skeletalMeshes.emplace_back(skMesh);
  } else {  //!通常メッシュ
    m_meshComps[a_mesh->GetMesh()->GetShaderName().data()].emplace_back(a_mesh);
  }
}

void Renderer::RemoveMeshComp(MeshComponent* a_mesh) {
  //!スキンメッシュ
  if (a_mesh->GetIsSkeletal()) {
    SkeletalMeshComponent* skMesh = static_cast<SkeletalMeshComponent*>(a_mesh);
    auto iter =
        std::find(m_skeletalMeshes.begin(), m_skeletalMeshes.end(), skMesh);
    m_skeletalMeshes.erase(iter);
  } else {  //!通常メッシュ
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
  //! カメラの行列の合成
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

  //! スキンメッシュ生成
  {
    m_pSkinnedShader = std::make_shared<Shader>();
    if (!m_pSkinnedShader->Load("../Shader/Skinned.vert", "../Shader/Phong.frag")) {
      return false;
    }

    m_pSkinnedShader->SetActive();
    //! 行列の合成
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
      -0.5f, 0.5f,  0.f, 0.f, 0.f, 0.0f, 0.f, 0.f,  //! 左上
      0.5f,  0.5f,  0.f, 0.f, 0.f, 0.0f, 1.f, 0.f,  //! 右上
      0.5f,  -0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f,  //! 右下
      -0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f   //! 左下
  };

  unsigned int indices[] = {0, 1, 2, 2, 3, 0};

  m_pSpriteVerts = std::make_shared<VertexArray>(vertices, 4, VertexArray::PosNormTex, indices, 6);
}

void Renderer::SetLightUniforms(Sptr<Shader> a_shader) {
  Matrix4 invView = m_view;
  invView.Invert();
  a_shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
  //! 環境光
  a_shader->SetVectorUniform("uAmbientLight", m_ambientLight);
  //! 平行光
  a_shader->SetVectorUniform("uDirLight.mDirection", m_dirLight.m_direction);
  a_shader->SetVectorUniform("uDirLight.mDiffuseColor",
                             m_dirLight.m_diffuseColor);
  a_shader->SetVectorUniform("uDirLight.mSpecColor", m_dirLight.m_specColor);

  //! 点光源
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
  //! screenPointを-1から+1までのデバイス座標に変換する
  Vector3 deviceCoord = a_screenPoint;
  deviceCoord.x /= (m_screenWidth)*0.5f;
  deviceCoord.y /= (m_screenHeight)*0.5f;

  //! 逆射影行列でベクトルを変換する
  Matrix4 unprojection = m_view * m_projection;
  unprojection.Invert();
  return Vector3::TransformWithPerspDiv(deviceCoord, unprojection);
}

void Renderer::GetScreenDirection(Vector3& a_outStart,
                                  Vector3& a_outDir) const {
  //! 始点を近接平面での画面の中心にする
  Vector3 screenPoint(0.0f, 0.0f, 0.0f);
  a_outStart = Unproject(screenPoint);
  //! 終点を近接平面と遠方平面の間の画面の中心に定める
  screenPoint.z = 0.9f;
  Vector3 end = Unproject(screenPoint);
  //! 方向ベクトルを求める
  a_outDir = end - a_outStart;
  a_outDir.Normalize();
}
