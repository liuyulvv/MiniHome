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
    m_mainRenderer->SetLayer(0);

    m_hoverRenderer = vtkSmartPointer<MHRenderer>::New();
    m_hoverRenderer->SetBackground(1.0, 1.0, 1.0);
    m_hoverRenderer->SetLayer(1);
}

}  // namespace MHCore
