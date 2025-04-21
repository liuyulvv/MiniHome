/**
 * @file MHHoleEntity.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-20
 */

#include "MHHoleEntity.h"

#include <vtkImageReader2.h>
#include <vtkImageReader2Factory.h>

#include "MHFaceToolKit.h"
#include "MHHoleManager.h"
#include "MHRendererManager.h"
#include "MHWallEntity.h"

namespace MHHouse {

MHHoleEntity::MHHoleEntity(vtkSmartPointer<MHCore::MHRenderer> renderer) : MHHouseEntity(renderer) {
    m_actor->GetProperty()->EdgeVisibilityOff();
    createDefaultTexture();
    m_actor->SetTexture(m_texture);
    m_pickerOrder = 1;
}

void MHHoleEntity::destroy() {
    MHHouseEntity::destroy();
    MHHoleManager::getInstance().removeHole(m_id);
    if (m_wallEntity) {
        m_wallEntity->removeHole(m_id);
        m_wallEntity = nullptr;
    }
}

void MHHoleEntity::onEnter() {
    MHHouseEntity::onEnter();
    if (m_selected) {
        return;
    }
    MHCore::MHRendererManager::getInstance().getHoverRenderer()->AddActor(m_outlineActor);
    m_outlineActor->SetTexture(m_hoveredTexture);
    for (auto &child : m_children) {
        if (child && !child->isSame(m_hole2D)) {
            child->onEnter();
        }
    }
}

void MHHoleEntity::onLeave() {
    MHHouseEntity::onLeave();
    if (m_selected) {
        return;
    }
    MHCore::MHRendererManager::getInstance().getHoverRenderer()->RemoveActor(m_outlineActor);
    for (auto &child : m_children) {
        if (child && !child->isSame(m_hole2D)) {
            child->onLeave();
        }
    }
}

void MHHoleEntity::onSelected(const MHCore::MHEntityInteractorInfo &info) {
    MHHouseEntity::onSelected(info);
    if (m_selected) {
        MHCore::MHRendererManager::getInstance().getHoverRenderer()->AddActor(m_outlineActor);
        m_outlineActor->SetTexture(m_selectedTexture);
    } else {
        MHCore::MHRendererManager::getInstance().getHoverRenderer()->RemoveActor(m_outlineActor);
        m_outlineActor->SetTexture(m_hoveredTexture);
    }
    for (auto &child : m_children) {
        if (child) {
            child->onSelected(info);
        }
    }
}

void MHHoleEntity::updateHole(std::shared_ptr<MHWallEntity> wallEntity, const MHGeometry::MHVertex &midVertex, std::unique_ptr<MHGeometry::MHEdge> midEdge, double height, double length, double width) {
    m_wallEntity = wallEntity;
    m_midVertex = std::make_unique<MHGeometry::MHVertex>(midVertex);
    m_midEdge = std::move(midEdge);
    m_height = height;
    m_length = length;
    m_width = width;
}

void MHHoleEntity::generateHole2D() {
    if (!m_midEdge || !m_midVertex) {
        return;
    }
    if (m_midEdge->getEdgeType() == MHGeometry::MHEdgeType::LINE_EDGE) {
        auto edgeDirection = (m_midEdge->getTargetVertex() - m_midEdge->getSourceVertex()).normalize();
        auto edgeNormal = edgeDirection.cross(MHGeometry::MHVertex(0, 0, 1)).normalize();

        MHGeometry::MHVertex edgeSourceVertex = *m_midVertex - edgeDirection * m_length / 2;
        MHGeometry::MHVertex edgeTargetVertex = *m_midVertex + edgeDirection * m_length / 2;

        MHGeometry::MHVertex innerSourceVertex = edgeSourceVertex - edgeNormal * m_width / 2;
        MHGeometry::MHVertex innerTargetVertex = edgeTargetVertex - edgeNormal * m_width / 2;
        MHGeometry::MHVertex outerSourceVertex = edgeSourceVertex + edgeNormal * m_width / 2;
        MHGeometry::MHVertex outerTargetVertex = edgeTargetVertex + edgeNormal * m_width / 2;

        MHGeometry::MHPlaneFace face2D;
        MHGeometry::MHWire wire2D;
        wire2D.addEdge(std::make_unique<MHGeometry::MHLineEdge>(innerSourceVertex, outerSourceVertex));
        wire2D.addEdge(std::make_unique<MHGeometry::MHLineEdge>(outerSourceVertex, outerTargetVertex));
        wire2D.addEdge(std::make_unique<MHGeometry::MHLineEdge>(outerTargetVertex, innerTargetVertex));
        wire2D.addEdge(std::make_unique<MHGeometry::MHLineEdge>(innerTargetVertex, innerSourceVertex));
        face2D.addWire(wire2D);
        m_baseFace = std::make_unique<MHGeometry::MHPlaneFace>(face2D);
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        transform->Identity();
        transform->Translate(0, 0, 2800);
        face2D.applyTransform(transform);

        if (!m_hole2D) {
            m_hole2D = std::make_shared<MHHouseEntity>(MHCore::MHRendererManager::getInstance().getMain2DRenderer());
            m_hole2D->setTexture(m_hole2DTexture);
            addChild(m_hole2D);
            m_hole2D->setPolygonOffset(-1.0, -1.0);
            m_hole2D->setLayerMask(MHCore::MHEntityLayerMask::LAYER_2D);
        }
        m_hole2D->setTopo(MHGeometry::MHToolKit::toTopoDSFace(face2D));
        m_hole2D->updateTopo();
    } else {
        auto arcEdge = static_cast<MHGeometry::MHArcEdge *>(m_midEdge.get());
        if (!arcEdge || arcEdge->getRadius() < m_width / 2) {
            return;
        }
        auto angleToXAxis = MHGeometry::MHToolKit::angleToXAxis(arcEdge->getCenter(), *m_midVertex);
        double angle = MHGeometry::MHToolKit::radianToAngle(m_length / 2 / arcEdge->getRadius());
        auto outerEdge = std::make_unique<MHGeometry::MHArcEdge>(arcEdge->getCenter(), arcEdge->getNormal(), arcEdge->getRadius() + m_width / 2, angleToXAxis - angle, angleToXAxis + angle);
        auto innerEdge = std::make_unique<MHGeometry::MHArcEdge>(arcEdge->getCenter(), arcEdge->getNormal(), arcEdge->getRadius() - m_width / 2, angleToXAxis - angle, angleToXAxis + angle);
        innerEdge->reversed();
        auto outerSourceVertex = outerEdge->getSourceVertex();
        auto outerTargetVertex = outerEdge->getTargetVertex();
        auto innerSourceVertex = innerEdge->getSourceVertex();
        auto innerTargetVertex = innerEdge->getTargetVertex();
        auto lineEdge1 = std::make_unique<MHGeometry::MHLineEdge>(outerTargetVertex, innerSourceVertex);
        auto lineEdge2 = std::make_unique<MHGeometry::MHLineEdge>(innerTargetVertex, outerSourceVertex);

        MHGeometry::MHPlaneFace face2D;
        MHGeometry::MHWire wire2D;
        wire2D.addEdge(std::move(outerEdge));
        wire2D.addEdge(std::move(lineEdge1));
        wire2D.addEdge(std::move(innerEdge));
        wire2D.addEdge(std::move(lineEdge2));
        face2D.addWire(std::move(wire2D));
        m_baseFace = std::make_unique<MHGeometry::MHPlaneFace>(face2D);
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        transform->Identity();
        transform->Translate(0, 0, 2800);
        face2D.applyTransform(transform);

        if (!m_hole2D) {
            m_hole2D = std::make_shared<MHHouseEntity>(MHCore::MHRendererManager::getInstance().getMain2DRenderer());
            m_hole2D->setTexture(m_hole2DTexture);
            addChild(m_hole2D);
            m_hole2D->setPolygonOffset(-1.0, -1.0);
            m_hole2D->setLayerMask(MHCore::MHEntityLayerMask::LAYER_2D);
        }
        m_hole2D->setTopo(MHGeometry::MHToolKit::toTopoDSFace(face2D));
        m_hole2D->updateTopo();
    }
}

void MHHoleEntity::generateHole3D() {
    if (!m_baseFace) {
        return;
    }
    m_solidShape = MHGeometry::MHToolKit::makePrismSolid(*m_baseFace, MHGeometry::MHVertex(0, 0, 1), m_height);
    // auto topoDSShapes = MHGeometry::MHToolKit::makePrismFace(*m_baseFace, MHGeometry::MHVertex(0, 0, 1), m_height);
    // for (int i = 0; i < topoDSShapes.size(); ++i) {
    //     auto entity = std::make_shared<MHHouseEntity>(MHCore::MHRendererManager::getInstance().getMain3DRenderer());
    //     entity->setTopo(topoDSShapes[i]);
    //     entity->updateTopo();
    //     entity->setTexture(m_texture);
    //     addChild(entity);
    //     entity->setLayerMask(MHCore::MHEntityLayerMask::LAYER_3D);
    // }
    if (m_wallEntity) {
        auto self = shared_from_this();
        m_wallEntity->addHole(std::dynamic_pointer_cast<MHHoleEntity>(self));
        m_wallEntity->generateWall3D();
        m_wallEntity->show();
    }
}

std::vector<std::unique_ptr<MHGeometry::MHEdge>> MHHoleEntity::getEdges() {
    return std::vector<std::unique_ptr<MHGeometry::MHEdge>>();
}

std::unique_ptr<MHGeometry::MHPlaneFace> MHHoleEntity::getBaseFace() const {
    return std::unique_ptr<MHGeometry::MHPlaneFace>();
}

std::shared_ptr<MHHouseEntity> MHHoleEntity::getHole2D() const {
    return m_hole2D;
}

TopoDS_Shape MHHoleEntity::getSolidShape() const {
    return m_solidShape;
}

void MHHoleEntity::createDefaultTexture() {
    vtkSmartPointer<vtkImageReader2Factory> readerFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
    vtkSmartPointer<vtkImageReader2> textureReader = readerFactory->CreateImageReader2("textures/default_wall.jpeg");
    textureReader->SetFileName("textures/default_wall.jpeg");
    textureReader->Update();
    m_texture->SetInputConnection(textureReader->GetOutputPort());
    m_texture->InterpolateOn();

    vtkSmartPointer<vtkImageReader2Factory> readerFactory2D = vtkSmartPointer<vtkImageReader2Factory>::New();
    vtkSmartPointer<vtkImageReader2> textureReader2D = readerFactory2D->CreateImageReader2("textures/default_pillar_2D.png");
    textureReader2D->SetFileName("textures/default_pillar_2D.png");
    textureReader2D->Update();
    m_hole2DTexture = vtkSmartPointer<vtkTexture>::New();
    m_hole2DTexture->SetInputConnection(textureReader2D->GetOutputPort());
    m_hole2DTexture->InterpolateOn();
}

}  // namespace MHHouse
