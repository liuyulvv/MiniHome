/**
 * @file MHMainVTKInteractorStyle.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-18
 */

#include "MHMainVTKInteractorStyle.h"

#include <vtkMath.h>
#include <vtkRenderWindow.h>

#include "MHActor.h"
#include "MHEntityManager.h"

namespace MHCore {

MHMainVTKInteractorStyle::MHMainVTKInteractorStyle() : m_maxElevation(vtkMath::RadiansFromDegrees(87.0)) {
    m_currentInteractorType = MHInteractorType::Top3D;
    m_cellPicker = nullptr;
    m_camera2D = vtkSmartPointer<vtkCamera>::New();
    m_camera2D->SetPosition(0.0, 0.0, 15.0);
    m_camera2D->SetFocalPoint(0.0, 0.0, 0.0);
    m_camera2D->SetViewUp(0.0, 1.0, 0.0);
    m_camera2D->SetParallelScale(10000.0);
    m_camera2D->ParallelProjectionOn();

    m_camera3D = vtkSmartPointer<vtkCamera>::New();
    m_camera3D->SetPosition(0.0, -20000.0, 20000.0);
    m_camera3D->SetFocalPoint(0.0, 0.0, 0.0);
    m_camera3D->SetViewUp(0.0, 0.0, 1.0);
    m_camera3D->SetClippingRange(0.1, 50000.0);

    m_interactorFilters = {};
    m_interactorInfo = {0, 0, 0, 0};

    m_lastX = 0;
    m_lastY = 0;
    m_leftPressed = false;
    m_rightPressed = false;
}

MHMainVTKInteractorStyle::~MHMainVTKInteractorStyle() {
}

void MHMainVTKInteractorStyle::init(vtkRenderWindowInteractor* interactor, vtkRenderer* renderer) {
    m_interactor = interactor;
    m_renderer = renderer;
    m_interactor->SetInteractorStyle(this);
    m_interactor->SetRenderWindow(renderer->GetRenderWindow());
    SetCurrentRenderer(renderer);
    if (m_currentInteractorType == MHInteractorType::Top2D) {
        renderer->SetActiveCamera(m_camera2D);
    } else {
        renderer->SetActiveCamera(m_camera3D);
    }
    m_cellPicker = vtkSmartPointer<vtkCellPicker>::New();
}

void MHMainVTKInteractorStyle::render() {
    m_interactor->GetRenderWindow()->Render();
}

void MHMainVTKInteractorStyle::switchTo(MHInteractorType type) {
    if (m_currentInteractorType == type) {
        return;
    }
    m_currentInteractorType = type;
    if (m_currentInteractorType == MHInteractorType::Top2D) {
        m_renderer->SetActiveCamera(m_camera2D);
    } else {
        m_renderer->SetActiveCamera(m_camera3D);
    }
    render();
}

void MHMainVTKInteractorStyle::insertFilter(std::shared_ptr<MHInteractorFilter> filter) {
    m_interactorFilters.push_back(filter);
    std::sort(m_interactorFilters.begin(), m_interactorFilters.end(), [](const auto& lhs, const auto& rhs) {
        return lhs->getPriority() < rhs->getPriority();
    });
}

void MHMainVTKInteractorStyle::removeFilter(std::shared_ptr<MHInteractorFilter> filter) {
    auto iter = std::find(m_interactorFilters.begin(), m_interactorFilters.end(), filter);
    if (iter != m_interactorFilters.end()) {
        m_interactorFilters.erase(iter);
    }
}

std::shared_ptr<MHEntity> MHMainVTKInteractorStyle::pickEntity() {
    auto clickPos = m_interactor->GetEventPosition();
    m_cellPicker->Pick(clickPos[0], clickPos[1], 0, m_renderer);
    auto pickedActor = m_cellPicker->GetActor();
    if (pickedActor) {
        auto actor = dynamic_cast<MHActor*>(pickedActor);
        if (actor) {
            auto entity = actor->getEntity();
            if (entity) {
                return MHEntityManager::getInstance().getEntity(entity->getId());
            }
        }
    }
    return nullptr;
}

void MHMainVTKInteractorStyle::fillInteractorInfo() {
    auto pos = m_interactor->GetEventPosition();
    m_interactorInfo.screenX = pos[0];
    m_interactorInfo.screenY = pos[1];
    double worldPos[4];
    m_renderer->SetDisplayPoint(m_interactorInfo.screenX, m_interactorInfo.screenY, 0);
    m_renderer->DisplayToWorld();
    m_renderer->GetWorldPoint(worldPos);
    m_interactorInfo.worldX = worldPos[0];
    m_interactorInfo.worldY = worldPos[1];
}

void MHMainVTKInteractorStyle::OnLeftButtonDown() {
    m_leftPressed = true;
    fillInteractorInfo();
    m_lastX = m_interactorInfo.screenX;
    m_lastY = m_interactorInfo.screenY;
    for (const auto& filter : m_interactorFilters) {
        if (filter->onLeftButtonDown(m_interactorInfo)) {
            return;
        }
    }
}

void MHMainVTKInteractorStyle::OnLeftButtonUp() {
    m_leftPressed = false;
    fillInteractorInfo();
    for (const auto& filter : m_interactorFilters) {
        if (filter->onLeftButtonUp(m_interactorInfo)) {
            return;
        }
    }
}

void MHMainVTKInteractorStyle::OnRightButtonDown() {
    m_rightPressed = true;
    fillInteractorInfo();
    for (const auto& filter : m_interactorFilters) {
        if (filter->onRightButtonDown(m_interactorInfo)) {
            return;
        }
    }
}

void MHMainVTKInteractorStyle::OnRightButtonUp() {
    m_rightPressed = false;
    fillInteractorInfo();
    for (const auto& filter : m_interactorFilters) {
        if (filter->onRightButtonUp(m_interactorInfo)) {
            return;
        }
    }
}

void MHMainVTKInteractorStyle::OnMouseMove() {
    fillInteractorInfo();
    for (const auto& filter : m_interactorFilters) {
        if (filter->onMouseMove(m_interactorInfo)) {
            return;
        }
    }
    if ((m_leftPressed && m_currentInteractorType == MHInteractorType::Top2D) || m_rightPressed) {
        this->Pan();
        return;
    } else if (m_leftPressed && m_currentInteractorType == MHInteractorType::Top3D) {
        double deltaX = -(m_interactorInfo.screenX - m_lastX) * 0.1;
        double deltaY = -(m_interactorInfo.screenY - m_lastY) * 0.1;
        auto camera = m_renderer->GetActiveCamera();
        double cameraPos[3];
        double focalPoint[3];
        camera->GetPosition(cameraPos);
        camera->GetFocalPoint(focalPoint);

        double direction[3] = {cameraPos[0] - focalPoint[0], cameraPos[1] - focalPoint[1], cameraPos[2] - focalPoint[2]};
        double radius = sqrt(direction[0] * direction[0] + direction[1] * direction[1] + direction[2] * direction[2]);
        double azimuth = atan2(direction[1], direction[0]) + vtkMath::RadiansFromDegrees(deltaX);
        double elevation = asin(direction[2] / radius) + vtkMath::RadiansFromDegrees(deltaY);
        if (elevation > m_maxElevation) {
            elevation = m_maxElevation;
        } else if (elevation < -m_maxElevation) {
            elevation = -m_maxElevation;
        }
        cameraPos[0] = focalPoint[0] + radius * cos(elevation) * cos(azimuth);
        cameraPos[1] = focalPoint[1] + radius * cos(elevation) * sin(azimuth);
        cameraPos[2] = focalPoint[2] + radius * sin(elevation);
        camera->SetPosition(cameraPos);
        camera->SetViewUp(0, 0, 1);
        m_lastX = m_interactorInfo.screenX;
        m_lastY = m_interactorInfo.screenY;
        render();
        return;
    }
}

}  // namespace MHCore