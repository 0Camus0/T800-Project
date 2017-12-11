#pragma once
#include <vector>
#include "scene/Quad.h"
#include "utils/xMaths.h"
#include "scene/PrimitiveManager.h"
#include "scene/PrimitiveInstance.h"
namespace t800 {
class LensFlare {
public:
  XMATRIX44 m_proj;
  const XVECTOR2 CENTER_SCREEN = XVECTOR2(0.5,0.5);
  std::vector<PrimitiveInst> m_quads;
  void Init(const PrimitiveManager& mngr);
  void Draw();
  void Update();
};
}