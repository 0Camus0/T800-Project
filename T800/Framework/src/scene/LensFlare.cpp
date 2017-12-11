#include "scene/LensFlare.h"

namespace t800 {
  void LensFlare::Init(const PrimitiveManager & mngr)
  {
    m_proj.Identity();
    m_quads.resize(1);
    m_quads[0].CreateInstance(mngr.GetPrimitive(PrimitiveManager::QUAD),&m_proj);
    m_quads[0].SetTexture(g_pBaseDriver->GetTexture(g_pBaseDriver->CreateTexture("sun.png")),0);
    m_quads[0].SetGlobalSignature(Signature::FSQUAD_1_TEX);
  }
  void LensFlare::Draw()
  {
    for (auto &it : m_quads) {
      it.Draw();
    }
  }
  void LensFlare::Update()
  {
  }
}