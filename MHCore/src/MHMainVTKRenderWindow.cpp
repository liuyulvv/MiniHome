/**
 * @file MHMainVTKRenderWindow.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHMainVTKRenderWindow.h"

#include "vtkRendererCollection.h"

namespace MHCore {

void MHMainVTKRenderWindow::Render() {
    double renderBox[6];
    auto renderer = this->GetRenderers()->GetFirstRenderer();
    renderer->ComputeVisiblePropBounds(renderBox);
    renderer->ResetCameraClippingRange(renderBox);
    vtkGenericOpenGLRenderWindow::Render();
}

}  // namespace MHCore
