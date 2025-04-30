/**
 * @file MHVertexEntity.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-29
 */

#include "MHVertexEntity.h"

#include <vtkCamera.h>
#include <vtkCoordinate.h>
#include <vtkPolyDataMapper.h>

#include <numbers>

#include "MHRendererManager.h"

namespace MHCore {

MHVertexEntity::MHVertexEntity() : MHEntity(MHRendererManager::getInstance().getMain2DRenderer()) {
    m_actor->GetProperty()->LightingOff();
}

void MHVertexEntity::setVertex(const MHGeometry::MHVertex& vertex) {
    m_vertex = vertex;

    m_source = vtkSmartPointer<vtkDiskSource>::New();
    m_source->SetCircumferentialResolution(32);
    m_source->SetCenter(vertex.x, vertex.y, vertex.z);
    m_source->SetInnerRadius(0.0);
    m_source->SetOuterRadius(m_radius);
    m_source->Update();

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(m_source->GetOutputPort());
    m_actor->SetMapper(mapper);
    m_actor->setRenderCallback([this]() {
        this->updateScale();
    });
    updateScale();
}

void MHVertexEntity::setRadius(double radius) {
    m_radius = radius;
}

void MHVertexEntity::updateScale() {
    if (!m_source || !m_renderer)
        return;
    double parallelScale = m_renderer->GetActiveCamera()->GetParallelScale();
    int* rendererSize = m_renderer->GetSize();
    double worldUnitsPerPixel = 2.0 * parallelScale / rendererSize[1];
    double worldRadius = m_radius * worldUnitsPerPixel;
    m_source->SetOuterRadius(worldRadius);
    m_source->Update();
}

}  // namespace MHCore
