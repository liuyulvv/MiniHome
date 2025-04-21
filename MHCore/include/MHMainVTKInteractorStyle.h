#pragma once

/**
 * @file MHMainVTKInteractorStyle.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-18
 */

#ifdef MH_CORE_EXPORTS
#define MH_CORE_API __declspec(dllexport)
#else
#define MH_CORE_API __declspec(dllimport)
#endif

#include <vtkCamera.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

#include <vector>

#include "MHEntity.h"
#include "MHInteractorFilter.h"

namespace MHCore {

enum class MHInteractorType {
    Top2D,
    Top3D
};

class MH_CORE_API MHMainVTKInteractorStyle : public vtkInteractorStyleTrackballCamera {
public:
    static MHMainVTKInteractorStyle& getInstance() {
        if (!m_instance) {
            m_instance = new MHMainVTKInteractorStyle();
        }
        return *m_instance;
    }

    vtkTypeMacro(MHMainVTKInteractorStyle, vtkInteractorStyleTrackballCamera);
    MHMainVTKInteractorStyle(const MHMainVTKInteractorStyle& interactor) = delete;
    MHMainVTKInteractorStyle(MHMainVTKInteractorStyle&& interactor) = delete;
    MHMainVTKInteractorStyle& operator=(const MHMainVTKInteractorStyle& interactor) = delete;
    MHMainVTKInteractorStyle& operator=(MHMainVTKInteractorStyle&& interactor) = delete;

private:
    MHMainVTKInteractorStyle();
    ~MHMainVTKInteractorStyle();

public:
    void init(vtkRenderWindowInteractor* interactor);
    void render();
    void switchTo(MHInteractorType type);
    void insertFilter(std::shared_ptr<MHInteractorFilter> filter);
    void removeFilter(std::shared_ptr<MHInteractorFilter> filter);
    std::shared_ptr<MHEntity> pickEntity();
    std::vector<std::shared_ptr<MHEntity>> pickEntities();
    void setSelectedEntity(std::shared_ptr<MHEntity> entity);

private:
    void fillInteractorInfo();

public:
    virtual void OnLeftButtonDown() override;
    virtual void OnLeftButtonUp() override;
    virtual void OnRightButtonDown() override;
    virtual void OnRightButtonUp() override;
    virtual void OnMouseMove() override;
    virtual void OnChar() override;
    virtual void OnKeyPress() override;
    virtual void OnKeyRelease() override;

private:
    static MHMainVTKInteractorStyle* m_instance;
    MHInteractorType m_currentInteractorType;
    vtkRenderWindowInteractor* m_interactor;  // not owned
    vtkSmartPointer<vtkCellPicker> m_cellPicker;
    vtkSmartPointer<vtkCamera> m_camera2D;
    vtkSmartPointer<vtkCamera> m_camera3D;
    std::vector<std::shared_ptr<MHInteractorFilter>> m_interactorFilters;
    MHInteractorInfo m_interactorInfo;

    const double m_maxElevation;
    int m_lastX;
    int m_lastY;
    bool m_leftPressed;
    bool m_rightPressed;
    MHEntityLayerMask m_layerMask = MHEntityLayerMask::NONE;
    std::shared_ptr<MHEntity> m_selectedEntity = nullptr;
    std::shared_ptr<MHEntity> m_hoveredEntity = nullptr;
    bool m_mouseMoveUpdateCamera = false;
};

}  // namespace MHCore