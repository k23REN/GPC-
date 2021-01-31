#include "CameraComponent.h"
#include "Actor.h"
#include "Renderer.h"
#include "Game.h"

CameraComponent::CameraComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
{
}

void CameraComponent::SetViewMatrix(const Matrix4& view)
{
	//! ビュー行列をレンダラーに渡す
	Game* game = m_pOwner->GetGame();
	game->GetRenderer()->SetViewMatrix(view);
}
