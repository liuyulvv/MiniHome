#pragma once

/**
 * @file MHRendererManager.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-10
 */

#ifdef MH_CORE_EXPORTS
#define MH_CORE_API __declspec(dllexport)
#else
#define MH_CORE_API __declspec(dllimport)
#endif

#include "MHRenderer.h"

namespace MHCore {

class MH_CORE_API MHRendererManager {
public:
    static MHRendererManager& getInstance() {
        static MHRendererManager instance;
        return instance;
    }

    MHRendererManager(const MHRendererManager& manager) = delete;
    MHRendererManager(MHRendererManager&& manager) = delete;
    MHRendererManager& operator=(const MHRendererManager& manager) = delete;
    MHRendererManager& operator=(MHRendererManager&& manager) = delete;

private:
    MHRendererManager();
    ~MHRendererManager() = default;

public:
    void setActiveMainRenderer(vtkSmartPointer<MHRenderer> renderer) {
        m_activeMainRenderer = renderer;
    }

    vtkSmartPointer<MHRenderer> getActiveMainRenderer() const {
        return m_activeMainRenderer;
    }

    std::vector<vtkSmartPointer<MHRenderer>> getActiveRenderers() const {
        return {m_activeMainRenderer, m_hoverRenderer};
    }

    vtkSmartPointer<MHRenderer> getMain3DRenderer() const {
        return m_main3DRenderer;
    }

    vtkSmartPointer<MHRenderer> getMain2DRenderer() const {
        return m_main2DRenderer;
    }

    vtkSmartPointer<MHRenderer> getHoverRenderer() const {
        return m_hoverRenderer;
    }

private:
    vtkSmartPointer<MHRenderer> m_activeMainRenderer = nullptr;
    vtkSmartPointer<MHRenderer> m_main3DRenderer = nullptr;
    vtkSmartPointer<MHRenderer> m_main2DRenderer = nullptr;
    vtkSmartPointer<MHRenderer> m_hoverRenderer = nullptr;
};

}  // namespace MHCore
