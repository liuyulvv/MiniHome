/**
 * @file MHWallEntity.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include "MHWallEntity.h"

#include <vtkImageReader2.h>
#include <vtkImageReader2Factory.h>

#include "MHEntityManager.h"
#include "MHFaceToolKit.h"
#include "MHPlaneFace.h"

namespace MHHouse {

MHWallEntity::MHWallEntity(vtkSmartPointer<MHCore::MHRenderer> renderer) : MHHouseEntity(renderer) {
    m_actor->GetProperty()->EdgeVisibilityOff();
    createDefaultTexture();
    m_actor->SetTexture(m_texture);
}

void MHWallEntity::updateWall(const MHGeometry::MHLineEdge& midEdge, double height, double width, MHWallPositionType positionType) {
    m_midEdge = std::make_unique<MHGeometry::MHLineEdge>(midEdge);
    m_height = height;
    m_width = width;
    m_positionType = positionType;
}

void MHWallEntity::updateWall(const MHGeometry::MHArcEdge& midEdge, double height, double width, MHWallPositionType positionType) {
    m_midEdge = std::make_unique<MHGeometry::MHArcEdge>(midEdge);
    m_height = height;
    m_width = width;
    m_positionType = positionType;
}

void MHWallEntity::generateWall2D() {
    if (!m_midEdge) {
        return;
    }
    auto edgeType = m_midEdge->getEdgeType();
    if (edgeType == MHGeometry::MHEdgeType::LINE_EDGE) {
        auto lineEdge = static_cast<MHGeometry::MHLineEdge*>(m_midEdge.get());
        if (lineEdge->length() < 1e-6) {
            return;
        }
        auto edgeDirection = (m_midEdge->getTargetVertex() - m_midEdge->getSourceVertex()).normalize();
        auto edgeNormal = edgeDirection.cross(MHGeometry::MHVertex(0, 0, 1)).normalize();
        m_edges.clear();
        MHGeometry::MHVertex innerSourceVertex, innerTargetVertex, outerSourceVertex, outerTargetVertex;
        switch (m_positionType) {
            case MHWallPositionType::LEFT: {
                innerSourceVertex = m_midEdge->getSourceVertex() - edgeNormal * m_width;
                innerTargetVertex = m_midEdge->getTargetVertex() - edgeNormal * m_width;
                outerSourceVertex = m_midEdge->getSourceVertex();
                outerTargetVertex = m_midEdge->getTargetVertex();
                break;
            }
            case MHWallPositionType::MID: {
                innerSourceVertex = m_midEdge->getSourceVertex() - edgeNormal * m_width / 2;
                innerTargetVertex = m_midEdge->getTargetVertex() - edgeNormal * m_width / 2;
                outerSourceVertex = m_midEdge->getSourceVertex() + edgeNormal * m_width / 2;
                outerTargetVertex = m_midEdge->getTargetVertex() + edgeNormal * m_width / 2;
                break;
            }
            case MHWallPositionType::RIGHT: {
                innerSourceVertex = m_midEdge->getSourceVertex();
                innerTargetVertex = m_midEdge->getTargetVertex();
                outerSourceVertex = m_midEdge->getSourceVertex() + edgeNormal * m_width;
                outerTargetVertex = m_midEdge->getTargetVertex() + edgeNormal * m_width;
                break;
            }
        }
        m_edges.push_back(std::make_unique<MHGeometry::MHLineEdge>(innerSourceVertex, outerSourceVertex));
        m_edges.push_back(std::make_unique<MHGeometry::MHLineEdge>(outerSourceVertex, outerTargetVertex));
        m_edges.push_back(std::make_unique<MHGeometry::MHLineEdge>(outerTargetVertex, innerTargetVertex));
        m_edges.push_back(std::make_unique<MHGeometry::MHLineEdge>(innerTargetVertex, innerSourceVertex));
    } else {
        auto arcEdge = static_cast<MHGeometry::MHArcEdge*>(m_midEdge.get());
        if (m_positionType == MHWallPositionType::MID && arcEdge->getRadius() < m_width / 2) {
            return;
        }
        if (m_positionType == MHWallPositionType::RIGHT && arcEdge->getRadius() < m_width) {
            return;
        }
        m_edges.clear();
        switch (m_positionType) {
            case MHWallPositionType::LEFT: {
                auto innerEdge = std::make_unique<MHGeometry::MHArcEdge>(arcEdge->getCenter(), arcEdge->getNormal(), arcEdge->getRadius(), arcEdge->getSourceAngle(), arcEdge->getTargetAngle());
                auto outerEdge = std::make_unique<MHGeometry::MHArcEdge>(arcEdge->getCenter(), arcEdge->getNormal(), arcEdge->getRadius() + m_width, arcEdge->getSourceAngle(), arcEdge->getTargetAngle());
                innerEdge->reversed();
                auto outerSourceVertex = outerEdge->getSourceVertex();
                auto outerTargetVertex = outerEdge->getTargetVertex();
                auto innerSourceVertex = innerEdge->getSourceVertex();
                auto innerTargetVertex = innerEdge->getTargetVertex();
                auto lineEdge1 = std::make_unique<MHGeometry::MHLineEdge>(outerTargetVertex, innerSourceVertex);
                auto lineEdge2 = std::make_unique<MHGeometry::MHLineEdge>(innerTargetVertex, outerSourceVertex);
                m_edges.push_back(std::move(outerEdge));
                m_edges.push_back(std::move(lineEdge1));
                m_edges.push_back(std::move(innerEdge));
                m_edges.push_back(std::move(lineEdge2));
                break;
            }
            case MHWallPositionType::MID: {
                auto innerEdge = std::make_unique<MHGeometry::MHArcEdge>(arcEdge->getCenter(), arcEdge->getNormal(), arcEdge->getRadius() - m_width / 2, arcEdge->getSourceAngle(), arcEdge->getTargetAngle());
                auto outerEdge = std::make_unique<MHGeometry::MHArcEdge>(arcEdge->getCenter(), arcEdge->getNormal(), arcEdge->getRadius() + m_width / 2, arcEdge->getSourceAngle(), arcEdge->getTargetAngle());
                innerEdge->reversed();
                auto outerSourceVertex = outerEdge->getSourceVertex();
                auto outerTargetVertex = outerEdge->getTargetVertex();
                auto innerSourceVertex = innerEdge->getSourceVertex();
                auto innerTargetVertex = innerEdge->getTargetVertex();
                auto lineEdge1 = std::make_unique<MHGeometry::MHLineEdge>(outerTargetVertex, innerSourceVertex);
                auto lineEdge2 = std::make_unique<MHGeometry::MHLineEdge>(innerTargetVertex, outerSourceVertex);
                m_edges.push_back(std::move(outerEdge));
                m_edges.push_back(std::move(lineEdge1));
                m_edges.push_back(std::move(innerEdge));
                m_edges.push_back(std::move(lineEdge2));
                break;
            }
            case MHWallPositionType::RIGHT: {
                auto innerEdge = std::make_unique<MHGeometry::MHArcEdge>(arcEdge->getCenter(), arcEdge->getNormal(), arcEdge->getRadius() - m_width, arcEdge->getSourceAngle(), arcEdge->getTargetAngle());
                auto outerEdge = std::make_unique<MHGeometry::MHArcEdge>(arcEdge->getCenter(), arcEdge->getNormal(), arcEdge->getRadius(), arcEdge->getSourceAngle(), arcEdge->getTargetAngle());
                innerEdge->reversed();
                auto outerSourceVertex = outerEdge->getSourceVertex();
                auto outerTargetVertex = outerEdge->getTargetVertex();
                auto innerSourceVertex = innerEdge->getSourceVertex();
                auto innerTargetVertex = innerEdge->getTargetVertex();
                auto lineEdge1 = std::make_unique<MHGeometry::MHLineEdge>(outerTargetVertex, innerSourceVertex);
                auto lineEdge2 = std::make_unique<MHGeometry::MHLineEdge>(innerTargetVertex, outerSourceVertex);
                m_edges.push_back(std::move(outerEdge));
                m_edges.push_back(std::move(lineEdge1));
                m_edges.push_back(std::move(innerEdge));
                m_edges.push_back(std::move(lineEdge2));
                break;
            }
        }
    }
    MHGeometry::MHPlaneFace baseFace;
    MHGeometry::MHWire baseWire;
    for (const auto& edge : m_edges) {
        auto edgeType = edge->getEdgeType();
        if (edgeType == MHGeometry::MHEdgeType::LINE_EDGE) {
            auto lineEdge = static_cast<MHGeometry::MHLineEdge*>(edge.get());
            baseWire.addEdge(*lineEdge);
        } else if (edgeType == MHGeometry::MHEdgeType::ARC_EDGE) {
            auto arcEdge = static_cast<MHGeometry::MHArcEdge*>(edge.get());
            baseWire.addEdge(*arcEdge);
        }
    }
    baseFace.addWire(baseWire);
    m_baseFace = std::make_unique<MHGeometry::MHPlaneFace>(baseFace);
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->Identity();
    transform->Translate(0, 0, m_height);
    baseFace.applyTransform(transform);
    if (!m_wall2D) {
        m_wall2D = std::make_shared<MHHouseEntity>();
        m_wall2D->setTexture(m_texture);
        addChild(m_wall2D);
    }
    m_wall2D->setTopo(MHGeometry::MHToolKit::toTopoDSFace(baseFace));
    m_wall2D->updateTopo();
}

void MHWallEntity::generateWall3D() {
    if (!m_baseFace || m_edges.empty()) {
        return;
    }
    auto topoDSShapes = MHGeometry::MHToolKit::makePrism(*m_baseFace, MHGeometry::MHVertex(0, 0, 1), m_height);
    for (int i = 0; i < topoDSShapes.size(); ++i) {
        auto entity = std::make_shared<MHHouseEntity>();
        entity->setTopo(topoDSShapes[i]);
        entity->updateTopo();
        entity->setTexture(m_texture);
        addChild(entity);
    }
}

std::vector<std::unique_ptr<MHGeometry::MHEdge>> MHWallEntity::getEdges() {
    std::vector<std::unique_ptr<MHGeometry::MHEdge>> edges;
    for (const auto& edge : m_edges) {
        auto clone = edge->clone();
        auto cloneEdge = dynamic_cast<MHGeometry::MHEdge*>(clone.release());
        auto edgeType = cloneEdge->getEdgeType();
        if (edgeType == MHGeometry::MHEdgeType::LINE_EDGE) {
            auto lineEdge = static_cast<MHGeometry::MHLineEdge*>(cloneEdge);
            auto newLineEdge = std::unique_ptr<MHGeometry::MHLineEdge>(lineEdge);
            edges.push_back(std::move(newLineEdge));
        } else if (edgeType == MHGeometry::MHEdgeType::ARC_EDGE) {
            auto arcEdge = static_cast<MHGeometry::MHArcEdge*>(cloneEdge);
            auto newArcEdge = std::unique_ptr<MHGeometry::MHArcEdge>(arcEdge);
            edges.push_back(std::move(newArcEdge));
        }
    }
    return edges;
}

std::unique_ptr<MHGeometry::MHPlaneFace> MHWallEntity::getBaseFace() const {
    if (m_baseFace) {
        auto clone = m_baseFace->clone();
        auto face = dynamic_cast<MHGeometry::MHPlaneFace*>(clone.release());
        return std::unique_ptr<MHGeometry::MHPlaneFace>(face);
    }
    return nullptr;
}

void MHWallEntity::createDefaultTexture() {
    vtkSmartPointer<vtkImageReader2Factory> readerFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
    vtkSmartPointer<vtkImageReader2> textureReader = readerFactory->CreateImageReader2("textures/default_wall.jpeg");
    textureReader->SetFileName("textures/default_wall.jpeg");
    textureReader->Update();
    m_texture->SetInputConnection(textureReader->GetOutputPort());
    m_texture->InterpolateOn();
}

}  // namespace MHHouse
