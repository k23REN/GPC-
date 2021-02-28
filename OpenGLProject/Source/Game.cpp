#include "Game.h"

#include <algorithm>

#include "BGSpriteComponent.h"
#include "GL/glew.h"
#include "SDL/SDL_image.h"
#include "MeshComponent.h"
#include "Utility.h"
#include "PlaneActor.h"
#include "FPSActor.h"

Game::Game() {}

bool Game::Initialize() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    SDL_Log("SDL Initialize Failed : %s", SDL_GetError());
    return false;
  }

  if (IMG_Init(IMG_INIT_PNG) == 0) {
    SDL_Log("SDL_image Initialize Failed : %s", SDL_GetError());
    return false;
  }

  	m_pRenderer = new Renderer(this);
  if (!m_pRenderer->Initialize(static_cast<float>(m_kWindowWidth), static_cast<float>(m_kWindowHeight))) {
    SDL_Log("Failed to initialize renderer");
    m_pRenderer = nullptr;
    return false;
  }

  LoadData();

  m_ticksCount = SDL_GetTicks();

  return true;
}

void Game::RunLoop() {
  while (m_isRunning) {
    ProcessInput();
    UpdateGame();
    GenerateOutput();
  }
}

void Game::ProcessInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        m_isRunning = false;
        break;

      case SDL_KEYDOWN:
        if (!event.key.repeat) {
          HandleKeyPress(event.key.keysym.sym);
        }
        break;

      case SDL_MOUSEBUTTONDOWN:
        HandleKeyPress(event.button.button);
        break;

      default:
        break;
    }
  }

  const Uint8* state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_ESCAPE]) {
    m_isRunning = false;
  }

  m_updatingActors = true;
  for (auto actor : m_actors) {
    actor->ProcessInput(state);
  }
  m_updatingActors = false;
}

void Game::HandleKeyPress(int key) {
  switch (key) {
    case '1':
    case '2':
    case '3':
    case '4':
      break;
    case SDL_BUTTON_LEFT: {

      break;
    }
    default:
      break;
  }
}

void Game::UpdateGame() {
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticksCount + 16))
    ;

  float deltaTime = (SDL_GetTicks() - m_ticksCount) / 1000.0f;
  if (deltaTime > 0.05f) {
    deltaTime = 0.05f;
  }
  m_ticksCount = SDL_GetTicks();

  m_updatingActors = true;
  for (auto actor : m_actors) {
    actor->Update(deltaTime);
  }
  m_updatingActors = false;

  for (auto pending : m_pendingActors) {
    pending->ComputeWorldTransform();
    m_actors.emplace_back(pending);
  }
  m_pendingActors.clear();

  std::vector<Actor*> deadActors;
  for (auto actor : m_actors) {
    if (actor->GetState() == Actor::eDead) {
      deadActors.emplace_back(actor);
    }
  }

  for (auto actor : deadActors) {
    delete actor;
  }
}

void Game::GenerateOutput() { m_pRenderer->Draw(); }

void Game::LoadData() {
  Actor* a = new Actor(this);
  Quaternion q(Vector3::UnitY, -Math::PiOver2);

  //! 箱と球生成
  {
    a->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
    a->SetScale(100.0f);
    q = Quaternion::Concatenate(
        q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
    a->SetRotation(q);
    MeshComponent* mc = new MeshComponent(a, m_pRenderer->GetMesh("../Assets/Cube.gpmesh"), false);

    a = new Actor(this);
    a->SetPosition(Vector3(200.0f, -75.0f, 0.0f));
    a->SetScale(3.0f);
    mc = new MeshComponent(a, m_pRenderer->GetMesh("../Assets/Sphere.gpmesh"), false);
  }

  //! マップ生成
  {
    const float start = -1250.0f;
    const float size = 250.0f;
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        a = new PlaneActor(this);
        a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
        a->SetRotation(Quaternion::Identity);
      }
    }

    q = Quaternion(Vector3::UnitX, Math::PiOver2);
    for (int i = 0; i < 10; i++) {
      a = new PlaneActor(this);
      a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
      a->SetRotation(q);

      a = new PlaneActor(this);
      a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
      a->SetRotation(q);
    }

    q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
    for (int i = 0; i < 10; i++) {
      a = new PlaneActor(this);
      a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
      a->SetRotation(q);

      a = new PlaneActor(this);
      a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
      a->SetRotation(q);
    }
  }

  //! ライト生成
  m_pRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
  DirectionalLight& dir = m_pRenderer->GetDirectionalLight();
  dir.m_direction = Vector3(0.0f, -0.707f, -0.707f);
  dir.m_diffuseColor = Vector3(0.78f, 0.88f, 1.0f);
  dir.m_specColor = Vector3(0.8f, 0.8f, 0.8f);

  //! カメラ生成
  m_pFollowActor = new FollowActor(this);
  
  //! UI生成
  a = new Actor(this);
  a->SetPosition(Vector3(-350.0f, -350.0f, 0.0f));
  SpriteComponent* sc = new SpriteComponent(a);
  sc->SetTexture(m_pRenderer->GetTexture("../Assets/HealthBar.png"));

  //! クロスヘア読み込み
  a = new Actor(this);
  a->SetPosition(Vector3(2.0f, 2.0f, 0.0f));
  m_crosshair = new SpriteComponent(a);
  m_crosshair->SetTexture(
      m_pRenderer->GetTexture("../Assets/Crosshair.png"));

  a = new Actor(this);
  a->SetPosition(Vector3(375.0f, -275.0f, 0.0f));
  a->SetScale(0.75f);
  sc = new SpriteComponent(a);
  sc->SetTexture(m_pRenderer->GetTexture("../Assets/Radar.png"));

}

void Game::UnloadData() {
  while (!m_actors.empty()) {
    delete m_actors.back();
  }

  if (m_pRenderer) {
    m_pRenderer->UnloadData();
  }
}

void Game::Shutdown() {
  UnloadData();
  if (m_pRenderer) {
    m_pRenderer->Shutdown();
  }
  SDL_Quit();
}

void Game::AddActor(Actor* actor) {
  if (m_updatingActors) {
    m_pendingActors.emplace_back(actor);
  } else {
    m_actors.emplace_back(actor);
  }
}

void Game::RemoveActor(Actor* actor) {
  auto iter = std::find(begin(m_pendingActors), end(m_pendingActors), actor);
  if (iter != end(m_pendingActors)) {
    std::iter_swap(iter, end(m_pendingActors) - 1);
    m_pendingActors.pop_back();
  }

  iter = std::find(begin(m_actors), end(m_actors), actor);
  if (iter != end(m_actors)) {
    std::iter_swap(iter, end(m_actors) - 1);
    m_actors.pop_back();
  }
}

Skeleton* Game::GetSkeleton(const std::string& fileName) {
  auto iter = m_skeletons.find(fileName);
  if (iter != m_skeletons.end()) {
    return iter->second;
  } else {
    Skeleton* sk = new Skeleton();
    if (sk->Load(fileName)) {
      m_skeletons.emplace(fileName, sk);
    } else {
      delete sk;
      sk = nullptr;
    }
    return sk;
  }
}

Animation* Game::GetAnimation(const std::string& fileName) {
  auto iter = m_anims.find(fileName);
  if (iter != m_anims.end()) {
    return iter->second;
  } else {
    Animation* anim = new Animation();
    if (anim->Load(fileName)) {
      m_anims.emplace(fileName, anim);
    } else {
      delete anim;
      anim = nullptr;
    }
    return anim;
  }
}
