#include "PlaneActor.h"
#include "Game.h"
#include "MeshComponent.h"

PlaneActor::PlaneActor(Game* game)
	:Actor(game)
{
	SetScale(10.0f);
  MeshComponent* mc = new MeshComponent(
      this, GetGame()->GetRenderer()->GetMesh("../Assets/Plane.gpmesh"));
}
