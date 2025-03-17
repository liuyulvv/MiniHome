/**
 * @file MHRenderer.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include "MHRenderer.h"

#include <vtkObjectFactory.h>

namespace MHCore {

vtkStandardNewMacro(MHRenderer);

MHRenderer::MHRenderer() {
    SetBackground(1.0, 1.0, 1.0);
}

MHRenderer::~MHRenderer() {
}

}  // namespace MHCore
