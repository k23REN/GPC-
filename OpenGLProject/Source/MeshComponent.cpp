#include "MeshComponent.h"

#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"

MeshComponent::MeshComponent(Actor* a_owner, Mesh* a_mesh, bool a_isSkeletal)
    : Component(a_owner), m_pMesh(nullptr), m_textureIndex(0) {
  m_pMesh = a_mesh;
  m_isSkeletal = a_isSkeletal;
  m_pOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent() {
  m_pOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Sptr<Shader> a_shader) {
  if (m_pMesh && m_visible) {
    //! シェーダに値を渡す
    a_shader->SetMatrixUniform("uWorldTransform",
                               m_pOwner->GetWorldTransform());
    a_shader->SetFloatUniform("uSpecPower", m_pMesh->GetSpecPower());

    Sptr<Texture> t = m_pMesh->GetTexture(m_textureIndex);
    if (t) {
      t->SetActive();
    }

    Sptr<VertexArray> va = m_pMesh->GetVertexArray();
    va->SetActive();
    // Draw
    glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
  }
}
