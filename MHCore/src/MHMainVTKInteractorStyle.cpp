/**
 * @file MHMainVTKInteractorStyle.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-18
 */

#include "MHMainVTKInteractorStyle.h"

#include <vtkMath.h>
#include <vtkPoints.h>
#include <vtkProp3DCollection.h>
#include <vtkRenderWindow.h>

#include "MHActor.h"
#include "MHEntityManager.h"
#include "MHRendererManager.h"

namespace MHCore {

MHMainVTKInteractorStyle* MHMainVTKInteractorStyle::m_instance = nullptr;

MHMainVTKInteractorStyle::MHMainVTKInteractorStyle() : m_maxElevation(vtkMath::RadiansFromDegrees(87.0)) {
    m_currentInteractorType = MHInteractorType::Top3D;
    m_cellPicker = nullptr;
    m_camera2D = vtkSmartPointer<vtkCamera>::New();
    m_camera2D->SetPosition(0.0, 0.0, 15000.0);
    m_camera2D->SetFocalPoint(0.0, 0.0, 0.0);
    m_camera2D->SetViewUp(0.0, 1.0, 0.0);
    m_camera2D->SetParallelScale(10000.0);
    m_camera2D->ParallelProjectionOn();

    m_camera3D = vtkSmartPointer<vtkCamera>::New();
    m_camera3D->SetPosition(0.0, -20000.0, 20000.0);
    m_camera3D->SetFocalPoint(0.0, 0.0, 0.0);
    m_camera3D->SetViewUp(0.0, 0.0, 1.0);

    m_interactorFilters = {};
    m_interactorInfo = {0, 0, 0, 0};

    m_lastX = 0;
    m_lastY = 0;
    m_leftPressed = false;
    m_rightPressed = false;
    AutoAdjustCameraClippingRangeOff();
}

MHMainVTKInteractorStyle::~MHMainVTKInteractorStyle() {
}

void MHMainVTKInteractorStyle::init(vtkRenderWindowInteractor* interactor) {
    m_interactor = interactor;
    m_interactor->SetInteractorStyle(this);
    auto mainRender = MHRendererManager::getInstance().getMainRenderer();
    auto hoverRender = MHRendererManager::getInstance().getHoverRenderer();
    m_interactor->SetRenderWindow(mainRender->GetRenderWindow());
    SetCurrentRenderer(mainRender);
    if (m_currentInteractorType == MHInteractorType::Top2D) {
        mainRender->SetActiveCamera(m_camera2D);
        hoverRender->SetActiveCamera(m_camera2D);
        m_layerMask = MHEntityLayerMask::LAYER_2D;
    } else {
        mainRender->SetActiveCamera(m_camera3D);
        hoverRender->SetActiveCamera(m_camera3D);
        m_layerMask = MHEntityLayerMask::LAYER_3D;
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
    auto mainRender = MHRendererManager::getInstance().getMainRenderer();
    auto hoverRender = MHRendererManager::getInstance().getHoverRenderer();
    if (m_currentInteractorType == MHInteractorType::Top2D) {
        mainRender->SetActiveCamera(m_camera2D);
        hoverRender->SetActiveCamera(m_camera2D);
    } else {
        mainRender->SetActiveCamera(m_camera3D);
        hoverRender->SetActiveCamera(m_camera3D);
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
    auto mainRender = MHRendererManager::getInstance().getMainRenderer();
    m_cellPicker->Pick(clickPos[0], clickPos[1], 0, mainRender);
    auto pickedProps = m_cellPicker->GetProp3Ds();
    auto pickedPositions = m_cellPicker->GetPickedPositions();
    pickedProps->InitTraversal();
    auto propSize = pickedProps->GetNumberOfItems();
    auto positionSize = pickedPositions->GetNumberOfPoints();
    auto cameraPosition = mainRender->GetActiveCamera()->GetPosition();
    double minDistance = std::numeric_limits<double>::max();
    std::shared_ptr<MHEntity> pickedEntity = nullptr;
    for (int i = 0; i < propSize && i < positionSize; ++i) {
        auto pickedProp = pickedProps->GetNextProp3D();
        auto pickedActor = dynamic_cast<MHActor*>(pickedProp);
        if (pickedActor) {
            auto pickedPosition = pickedPositions->GetPoint(i);
            auto actor = dynamic_cast<MHActor*>(pickedActor);
            if (actor) {
                auto entity = actor->getEntity();
                if (entity && (entity->getLayerMask() & m_layerMask)) {
                    auto distance = sqrt(pow(pickedPosition[0] - cameraPosition[0], 2) + pow(pickedPosition[1] - cameraPosition[1], 2) + pow(pickedPosition[2] - cameraPosition[2], 2));
                    if (distance < minDistance) {
                        minDistance = distance;
                        pickedEntity = entity;
                    }
                }
            }
        }
    }
    return pickedEntity;
}

std::vector<std::shared_ptr<MHEntity>> MHMainVTKInteractorStyle::pickEntities() {
    std::vector<std::shared_ptr<MHEntity>> entities;
    auto clickPos = m_interactor->GetEventPosition();
    auto mainRender = MHRendererManager::getInstance().getMainRenderer();
    m_cellPicker->Pick(clickPos[0], clickPos[1], 0, mainRender);
    auto pickedActors = m_cellPicker->GetActors();
    pickedActors->InitTraversal();
    for (int i = 0; i < pickedActors->GetNumberOfItems(); ++i) {
        auto pickedActor = pickedActors->GetNextActor();
        if (pickedActor) {
            auto actor = dynamic_cast<MHActor*>(pickedActor);
            if (actor) {
                auto entity = actor->getEntity();
                if (entity && (entity->getLayerMask() & m_layerMask)) {
                    entities.push_back(entity);
                }
            }
        }
    }
    return entities;
}

void MHMainVTKInteractorStyle::fillInteractorInfo() {
    auto pos = m_interactor->GetEventPosition();
    m_interactorInfo.screenX = pos[0];
    m_interactorInfo.screenY = pos[1];
    double worldPos[4];
    auto mainRender = MHRendererManager::getInstance().getMainRenderer();
    mainRender->SetDisplayPoint(m_interactorInfo.screenX, m_interactorInfo.screenY, 0);
    mainRender->DisplayToWorld();
    mainRender->GetWorldPoint(worldPos);
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
        auto camera = m_currentInteractorType == MHInteractorType::Top2D ? m_camera2D : m_camera3D;
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
    auto entity = pickEntity();
    if (entity) {
        if (m_hoveredEntity) {
            if (m_hoveredEntity->isSame(entity)) {
            } else {
                m_hoveredEntity->onLeave();
                m_hoveredEntity = entity;
                m_hoveredEntity->onEnter();
            }
        } else {
            m_hoveredEntity = entity;
            m_hoveredEntity->onEnter();
        }
        render();
    } else {
        if (m_hoveredEntity) {
            m_hoveredEntity->onLeave();
            m_hoveredEntity = nullptr;
            render();
        }
    }
}

void MHMainVTKInteractorStyle::OnChar() {
    if (m_interactor->GetKeyCode() == '2') {
        switchTo(MHInteractorType::Top2D);
    } else if (m_interactor->GetKeyCode() == '3') {
        switchTo(MHInteractorType::Top3D);
    }
}

}  // namespace MHCore