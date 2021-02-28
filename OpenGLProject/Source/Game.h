#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "Actor.h"
#include "SDL/SDL.h"
#include "Utility.h"
#include "Renderer.h"
#include "CameraActor.h"
#include "FPSActor.h"
#include "FollowActor.h"
#include "OrbitActor.h"
#include "SplineActor.h"
#include "SpriteComponent.h"
#include "Skeleton.h"
#include "Animation.h"

class Game : public std::enable_shared_from_this<Game> {
 public:
  Game();
  bool Initialize();
  void RunLoop();
  void Shutdown();

  /**
   * @fn
   * �A�N�^�[��z��ɒǉ����A�ꊇ�ŏ������s��
   * @brief �A�N�^�[��ǉ�����
   * @param (a_actor) �ǉ�����A�N�^�[
   * @return �Ȃ�
   */
  void AddActor(Actor* a_actor);

  /**
   * @fn
   * �A�N�^�[��z�񂩂�폜����
   * @brief �A�N�^�[���폜����
   * @param (a_actor) �폜����A�N�^�[
   * @return �Ȃ�
   */
  void RemoveActor(Actor* a_actor);

  ///
  ///�擾
  /// 
  [[nodiscard]] Renderer* GetRenderer() { return m_pRenderer; }
  [[nodiscard]] Skeleton* GetSkeleton(const std::string& fileName);
  [[nodiscard]] Animation* GetAnimation(const std::string& fileName);

  static inline constexpr int m_kWindowWidth = 1024;
  static inline constexpr int m_kWindowHeight = 768;

  enum Color 
  { 
      Red,
      Green,
      Blue
  };

 private:
  void ProcessInput();
  void HandleKeyPress(int key);
  void UpdateGame();
  void GenerateOutput();
  void LoadData();
  void UnloadData();

  Renderer* m_pRenderer = nullptr;
  SpriteComponent* m_crosshair = nullptr;

  std::vector<Actor*> m_actors;
  std::vector<Actor*> m_pendingActors;
  std::unordered_map<std::string, Skeleton*> m_skeletons;
  std::unordered_map<std::string, Animation*> m_anims;

  Uint32 m_ticksCount = 0;
  bool m_isRunning = true;
  bool m_updatingActors = false;

  FollowActor* m_pFollowActor = nullptr;
  SplineActor* m_pSplineActor = nullptr;
  Actor* m_startSphere = nullptr;
  Actor* m_endSphere = nullptr;

};
