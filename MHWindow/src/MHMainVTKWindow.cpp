/**
 * @file MHMainVTKWindow.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include "MHMainVTKWindow.h"

#include <vtkPlaneSource.h>

#include "MHActor.h"
#include "MHEntity.h"
#include "MHEntityManager.h"
#include "MHMainVTKInteractorStyle.h"
#include "MHRenderer.h"
#include "MHRendererManager.h"

namespace MHWindow {

MHMainVTKWindow::MHMainVTKWindow(QWidget* parent) : QVTKOpenGLNativeWidget(parent) {
    m_window = vtkSmartPointer<MHCore::MHMainVTKRenderWindow>::New();
    auto mainRenderer = MHCore::MHRendererManager::getInstance().getMainRenderer();
    m_window->AddRenderer(mainRenderer);
    setRenderWindow(m_window);
    MHCore::MHMainVTKInteractorStyle::getInstance().init(m_window->GetInteractor(), mainRenderer);
    createGround();
}

MHMainVTKWindow::~MHMainVTKWindow() {
    MHCore::MHEntityManager::getInstance().clear();
    m_window = nullptr;
}

void MHMainVTKWindow::createGround() {
    auto ground = vtkSmartPointer<vtkPlaneSource>::New();
    ground->SetOrigin(-20000.0, -20000.0, 0.0);
    ground->SetPoint1(20000.0, -20000.0, 0.0);
    ground->SetPoint2(-20000.0, 20000.0, 0.0);
    ground->SetResolution(40, 40);
    auto groundMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    groundMapper->SetInputConnection(ground->GetOutputPort());
    auto groundActor = vtkSmartPointer<MHCore::MHActor>::New();
    groundActor->SetMapper(groundMapper);
    groundActor->GetProperty()->SetEdgeVisibility(true);
    groundActor->GetProperty()->SetRepresentationToSurface();
    groundActor->GetProperty()->BackfaceCullingOn();
    groundActor->PickableOff();
    groundActor->GetProperty()->LightingOff();
    MHCore::MHRendererManager::getInstance().getMainRenderer()->AddActor(groundActor);
}

}  // namespace MHWindow