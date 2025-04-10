/**
 * @file MHRenderer.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include "MHRenderer.h"

#include <vtkRenderWindow.h>

namespace MHCore {

void MHRenderer::render() {
    GetRenderWindow()->Render();
}

MHRenderer::MHRenderer() {
    SetBackground(1.0, 1.0, 1.0);
}

MHRenderer::~MHRenderer() {
}

}  // namespace MHCore
