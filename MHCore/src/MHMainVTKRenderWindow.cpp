/**
 * @file MHMainVTKRenderWindow.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHMainVTKRenderWindow.h"

#include <vtkCamera.h>
#include <vtkRendererCollection.h>

namespace MHCore {

void MHMainVTKRenderWindow::Render() {
    double bounds[6];
    auto renderer = this->GetRenderers()->GetFirstRenderer();
    renderer->ComputeVisiblePropBounds(bounds);
    double dx = bounds[1] - bounds[0];
    double dy = bounds[3] - bounds[2];
    double dz = bounds[5] - bounds[4];
    double diagonal = std::sqrt(dx * dx + dy * dy + dz * dz);
    double nearDistance = diagonal * 0.01;
    double farDistance = diagonal * 10.0;
    auto camera = renderer->GetActiveCamera();
    camera->SetClippingRange(nearDistance, farDistance);
    vtkGenericOpenGLRenderWindow::Render();
}

}  // namespace MHCore
