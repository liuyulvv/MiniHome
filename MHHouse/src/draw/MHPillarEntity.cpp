/**
 * @file MHPillarEntity.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-13
 */

#include "MHPillarEntity.h"

#include <vtkImageReader2.h>
#include <vtkImageReader2Factory.h>

#include "MHFaceToolKit.h"

namespace MHHouse {

MHPillarEntity::MHPillarEntity(vtkSmartPointer<MHCore::MHRenderer> renderer) : MHHouseEntity(renderer) {
    m_actor->GetProperty()->EdgeVisibilityOff();
    createDefaultTexture();
    m_actor->SetTexture(m_texture);
}

void MHPillarEntity::updatePillar(const MHGeometry::MHLineEdge& midEdge, double height, double length, double width) {
    m_midEdge = std::make_unique<MHGeometry::MHLineEdge>(midEdge);
    m_height = height;
    m_length = length;
    m_width = width;
}

void MHPillarEntity::generatePillar2D() {
    if (!m_midEdge) {
        return;
    }
    auto edgeDirection = (m_midEdge->getTargetVertex() - m_midEdge->getSourceVertex()).normalize();
    auto edgeNormal = edgeDirection.cross(MHGeometry::MHVertex(0, 0, 1)).normalize();
    m_edges.clear();
    MHGeometry::MHVertex innerSourceVertex = m_midEdge->getSourceVertex() - edgeNormal * m_width / 2;
    MHGeometry::MHVertex innerTargetVertex = m_midEdge->getTargetVertex() - edgeNormal * m_width / 2;
    MHGeometry::MHVertex outerSourceVertex = m_midEdge->getSourceVertex() + edgeNormal * m_width / 2;
    MHGeometry::MHVertex outerTargetVertex = m_midEdge->getTargetVertex() + edgeNormal * m_width / 2;
    m_edges.push_back(std::make_unique<MHGeometry::MHLineEdge>(innerSourceVertex, outerSourceVertex));
    m_edges.push_back(std::make_unique<MHGeometry::MHLineEdge>(outerSourceVertex, outerTargetVertex));
    m_edges.push_back(std::make_unique<MHGeometry::MHLineEdge>(outerTargetVertex, innerTargetVertex));
    m_edges.push_back(std::make_unique<MHGeometry::MHLineEdge>(innerTargetVertex, innerSourceVertex));
    MHGeometry::MHPlaneFace baseFace;
    MHGeometry::MHWire baseWire;
    for (const auto& edge : m_edges) {
        baseWire.addEdge(*edge);
    }
    baseFace.addWire(baseWire);
    m_baseFace = std::make_unique<MHGeometry::MHPlaneFace>(baseFace);
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->Identity();
    transform->Translate(0, 0, m_height);
    baseFace.applyTransform(transform);
    if (!m_pillar2D) {
        m_pillar2D = std::make_shared<MHHouseEntity>();
        m_pillar2D->setTexture(m_texture);
        addChild(m_pillar2D);
    }
    m_pillar2D->setTopo(MHGeometry::MHToolKit::toTopoDSFace(baseFace));
    m_pillar2D->updateTopo();
}

void MHPillarEntity::generatePillar3D() {
    if (!m_baseFace || m_edges.empty()) {
        return;
    }
    for (const auto& edge : m_edges) {
        auto planeFace = MHGeometry::MHToolKit::edgeToFace(*edge, {0, 0, 1}, m_height);
        auto topoDSFace = MHGeometry::MHToolKit::toTopoDSFace(planeFace);
        auto entity = std::make_shared<MHHouseEntity>();
        entity->setTopo(topoDSFace);
        entity->updateTopo();
        entity->setTexture(m_texture);
        addChild(entity);
    }
}

std::vector<std::unique_ptr<MHGeometry::MHEdge>> MHPillarEntity::getEdges() {
    std::vector<std::unique_ptr<MHGeometry::MHEdge>> edges;
    for (const auto& edge : m_edges) {
        auto clone = edge->clone();
        auto cloneEdge = dynamic_cast<MHGeometry::MHEdge*>(clone.release());
        auto lineEdge = static_cast<MHGeometry::MHLineEdge*>(cloneEdge);
        auto newLineEdge = std::unique_ptr<MHGeometry::MHLineEdge>(lineEdge);
        edges.push_back(std::move(newLineEdge));
    }
    return edges;
}

std::unique_ptr<MHGeometry::MHPlaneFace> MHPillarEntity::getBaseFace() const {
    if (m_baseFace) {
        return std::make_unique<MHGeometry::MHPlaneFace>(*m_baseFace);
    }
    return nullptr;
}

void MHPillarEntity::createDefaultTexture() {
    vtkSmartPointer<vtkImageReader2Factory> readerFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
    vtkSmartPointer<vtkImageReader2> textureReader = readerFactory->CreateImageReader2("textures/default_wall.jpeg");
    textureReader->SetFileName("textures/default_wall.jpeg");
    textureReader->Update();
    m_texture->SetInputConnection(textureReader->GetOutputPort());
    m_texture->InterpolateOn();
}

}  // namespace MHHouse
