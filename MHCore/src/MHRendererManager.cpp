/**
 * @file MHRendererManager.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-10
 */

#include "MHRendererManager.h"

namespace MHCore {

MHRendererManager::MHRendererManager() {
    m_mainRenderer = vtkSmartPointer<MHRenderer>::New();
    m_mainRenderer->SetBackground(1.0, 1.0, 1.0);
}

}  // namespace MHCore
