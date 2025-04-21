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
    auto main3DRenderer = MHCore::MHRendererManager::getInstance().getMain3DRenderer();
    auto main2DRenderer = MHCore::MHRendererManager::getInstance().getMain2DRenderer();
    auto hoverRenderer = MHCore::MHRendererManager::getInstance().getHoverRenderer();
    m_window->SetNumberOfLayers(2);
    m_window->AddRenderer(main3DRenderer);
    m_window->AddRenderer(main2DRenderer);
    m_window->AddRenderer(hoverRenderer);
    setRenderWindow(m_window);
    MHCore::MHMainVTKInteractorStyle::getInstance().init(m_window->GetInteractor());
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
    groundMapper->SetResolveCoincidentTopologyToPolygonOffset();
    groundMapper->SetRelativeCoincidentTopologyPolygonOffsetParameters(1.0, 1.0);
    auto groundActor = vtkSmartPointer<MHCore::MHActor>::New();
    groundActor->SetMapper(groundMapper);
    groundActor->GetProperty()->SetColor(0.95, 0.95, 0.95);
    groundActor->GetProperty()->SetEdgeVisibility(true);
    groundActor->GetProperty()->SetRepresentationToSurface();
    groundActor->GetProperty()->BackfaceCullingOn();
    groundActor->PickableOff();
    groundActor->GetProperty()->LightingOff();
    MHCore::MHRendererManager::getInstance().getMain2DRenderer()->AddActor(groundActor);
    MHCore::MHRendererManager::getInstance().getMain3DRenderer()->AddActor(groundActor);
}

}  // namespace MHWindow