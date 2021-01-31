#pragma once
#include "Component.h"
#include <cstddef>
#include "Mesh.h"
#include "Shader.h"

class MeshComponent : public Component {
 public:
  MeshComponent(Actor* a_owner, Mesh* a_mesh);
  ~MeshComponent();
  virtual void Draw(Sptr<Shader> a_shader);

  [[nodiscard]] const Mesh* GetMesh() const { return m_pMesh; }

  virtual void SetMesh(Mesh* a_mesh) { m_pMesh = a_mesh; }
  void SetTextureIndex(size_t a_index) { m_textureIndex = a_index; }

  void SetVisible(bool visible) { m_visible = visible; }
  bool GetVisible() const { return m_visible; }

 protected:
  Mesh* m_pMesh;
  size_t m_textureIndex;
  bool m_visible = true;
};
