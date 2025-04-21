/**
 * @file MHRendererManager.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-10
 */

#include "MHRendererManager.h"

namespace MHCore {

MHRendererManager::MHRendererManager() {
    m_main3DRenderer = vtkSmartPointer<MHRenderer>::New();
    m_main3DRenderer->SetBackground(0.95, 0.95, 0.95);
    m_main3DRenderer->SetLayer(0);

    m_main2DRenderer = vtkSmartPointer<MHRenderer>::New();
    m_main2DRenderer->SetBackground(0.95, 0.95, 0.95);
    m_main2DRenderer->SetLayer(0);

    m_hoverRenderer = vtkSmartPointer<MHRenderer>::New();
    m_hoverRenderer->SetBackground(1.0, 1.0, 1.0);
    m_hoverRenderer->SetLayer(1);

    m_activeMainRenderer = m_main3DRenderer;
}

}  // namespace MHCore
