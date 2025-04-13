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
    vtkSmartPointer<MHRenderer> getMainRenderer() const {
        return m_mainRenderer;
    }

    vtkSmartPointer<MHRenderer> getHoverRenderer() const {
        return m_hoverRenderer;
    }

private:
    vtkSmartPointer<MHRenderer> m_mainRenderer = nullptr;
    vtkSmartPointer<MHRenderer> m_hoverRenderer = nullptr;
};

}  // namespace MHCore
