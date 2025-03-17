#pragma once

/**
 * @file MHRenderer.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#ifdef MH_CORE_EXPORTS
#define MH_CORE_API __declspec(dllexport)
#else
#define MH_CORE_API __declspec(dllimport)
#endif

#include <vtkOpenGLRenderer.h>

namespace MHCore {

class MH_CORE_API MHRenderer : public vtkOpenGLRenderer {
public:
    static MHRenderer* New();
    vtkTypeMacro(MHRenderer, vtkOpenGLRenderer);
    MHRenderer(const MHRenderer& renderer) = delete;
    MHRenderer(MHRenderer&& renderer) = delete;
    MHRenderer& operator=(const MHRenderer& renderer) = delete;
    MHRenderer& operator=(MHRenderer&& renderer) = delete;

protected:
    MHRenderer();
    ~MHRenderer();
};

}  // namespace MHCore
