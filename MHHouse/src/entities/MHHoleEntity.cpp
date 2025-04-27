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

void MHHoleEntity::onEnter(const MHCore::MHEntityInteractorInfo &info) {
    MHHouseEntity::onEnter(info);
    if (m_selected) {
        return;
    }
    MHCore::MHRendererManager::getInstance().getHoverRenderer()->AddActor(m_outlineActor);
    m_outlineActor->SetTexture(m_hoveredTexture);
    for (auto &child : m_children) {
        if (child && !child->isSame(m_hole2D)) {
            child->onEnter(info);
        }
    }
}

void MHHoleEntity::onLeave(const MHCore::MHEntityInteractorInfo &info) {
    MHHouseEntity::onLeave(info);
    if (m_selected) {
        return;
    }
    MHCore::MHRendererManager::getInstance().getHoverRenderer()->RemoveActor(m_outlineActor);
    for (auto &child : m_children) {
        if (child && !child->isSame(m_hole2D)) {
            child->onLeave(info);
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

void MHHoleEntity::updateHole(std::shared_ptr<MHWallEntity> wallEntity, const MHGeometry::MHVertex &midVertex, std::shared_ptr<MHGeometry::MHEdge> midEdge, double height, double length, double width) {
    m_wallEntity = wallEntity;
    m_midVertex = std::make_shared<MHGeometry::MHVertex>(midVertex);
    m_midEdge = midEdge;
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
        wire2D.addEdge(std::make_shared<MHGeometry::MHLineEdge>(innerSourceVertex, outerSourceVertex));
        wire2D.addEdge(std::make_shared<MHGeometry::MHLineEdge>(outerSourceVertex, outerTargetVertex));
        wire2D.addEdge(std::make_shared<MHGeometry::MHLineEdge>(outerTargetVertex, innerTargetVertex));
        wire2D.addEdge(std::make_shared<MHGeometry::MHLineEdge>(innerTargetVertex, innerSourceVertex));
        face2D.addWire(wire2D);
        m_baseFace = std::make_shared<MHGeometry::MHPlaneFace>(face2D);
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
        auto outerEdge = std::make_shared<MHGeometry::MHArcEdge>(arcEdge->getCenter(), arcEdge->getNormal(), arcEdge->getRadius() + m_width / 2, angleToXAxis - angle, angleToXAxis + angle);
        auto innerEdge = std::make_shared<MHGeometry::MHArcEdge>(arcEdge->getCenter(), arcEdge->getNormal(), arcEdge->getRadius() - m_width / 2, angleToXAxis - angle, angleToXAxis + angle);
        innerEdge->reversed();
        auto outerSourceVertex = outerEdge->getSourceVertex();
        auto outerTargetVertex = outerEdge->getTargetVertex();
        auto innerSourceVertex = innerEdge->getSourceVertex();
        auto innerTargetVertex = innerEdge->getTargetVertex();
        auto lineEdge1 = std::make_shared<MHGeometry::MHLineEdge>(outerTargetVertex, innerSourceVertex);
        auto lineEdge2 = std::make_shared<MHGeometry::MHLineEdge>(innerTargetVertex, outerSourceVertex);

        MHGeometry::MHPlaneFace face2D;
        MHGeometry::MHWire wire2D;
        wire2D.addEdge(outerEdge);
        wire2D.addEdge(lineEdge1);
        wire2D.addEdge(innerEdge);
        wire2D.addEdge(lineEdge2);
        face2D.addWire(wire2D);
        m_baseFace = std::make_shared<MHGeometry::MHPlaneFace>(face2D);
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
    if (!m_midEdge) {
        return;
    }
    m_children.clear();
    if (m_hole2D) {
        addChild(m_hole2D);
    }
    if (m_midEdge->getEdgeType() == MHGeometry::MHEdgeType::LINE_EDGE) {
        auto edgeDirection = (m_midEdge->getTargetVertex() - m_midEdge->getSourceVertex()).normalize();
        auto edgeNormal = edgeDirection.cross(MHGeometry::MHVertex(0, 0, 1)).normalize();

        MHGeometry::MHVertex zAxis(0, 0, 1);
        MHGeometry::MHVertex edgeSourceVertex = *m_midVertex - edgeDirection * m_length / 2;
        MHGeometry::MHVertex edgeTargetVertex = *m_midVertex + edgeDirection * m_length / 2;
        MHGeometry::MHVertex innerSourceVertex = edgeSourceVertex - edgeNormal * m_width / 2;
        MHGeometry::MHVertex topInnerSourceVertex = innerSourceVertex + zAxis * m_height;
        MHGeometry::MHVertex innerTargetVertex = edgeTargetVertex - edgeNormal * m_width / 2;
        MHGeometry::MHVertex topInnerTargetVertex = innerTargetVertex + zAxis * m_height;
        MHGeometry::MHVertex outerSourceVertex = edgeSourceVertex + edgeNormal * m_width / 2;
        MHGeometry::MHVertex topOuterSourceVertex = outerSourceVertex + zAxis * m_height;
        MHGeometry::MHVertex outerTargetVertex = edgeTargetVertex + edgeNormal * m_width / 2;
        MHGeometry::MHVertex topOuterTargetVertex = outerTargetVertex + zAxis * m_height;

        MHGeometry::MHWire wireInner;
        wireInner.addEdge(std::make_shared<MHGeometry::MHLineEdge>(innerSourceVertex, innerTargetVertex));
        wireInner.addEdge(std::make_shared<MHGeometry::MHLineEdge>(innerTargetVertex, topInnerTargetVertex));
        wireInner.addEdge(std::make_shared<MHGeometry::MHLineEdge>(topInnerTargetVertex, topInnerSourceVertex));
        wireInner.addEdge(std::make_shared<MHGeometry::MHLineEdge>(topInnerSourceVertex, innerSourceVertex));
        MHGeometry::MHWire wireOuter;
        wireOuter.addEdge(std::make_shared<MHGeometry::MHLineEdge>(outerSourceVertex, outerTargetVertex));
        wireOuter.addEdge(std::make_shared<MHGeometry::MHLineEdge>(outerTargetVertex, topOuterTargetVertex));
        wireOuter.addEdge(std::make_shared<MHGeometry::MHLineEdge>(topOuterTargetVertex, topOuterSourceVertex));
        wireOuter.addEdge(std::make_shared<MHGeometry::MHLineEdge>(topOuterSourceVertex, outerSourceVertex));

        m_solidShape = MHGeometry::MHToolKit::makeThruSectionSolid(wireInner, wireOuter);
    } else {
        auto arcEdge = static_cast<MHGeometry::MHArcEdge *>(m_midEdge.get());
        if (!arcEdge || arcEdge->getRadius() < m_width / 2) {
            return;
        }
        auto angleToXAxis = MHGeometry::MHToolKit::angleToXAxis(arcEdge->getCenter(), *m_midVertex);
        double angle = MHGeometry::MHToolKit::radianToAngle(m_length / 2 / arcEdge->getRadius());
        auto outerEdge = std::make_shared<MHGeometry::MHArcEdge>(arcEdge->getCenter(), arcEdge->getNormal(), arcEdge->getRadius() + m_width / 2, angleToXAxis - angle, angleToXAxis + angle);
        auto innerEdge = std::make_shared<MHGeometry::MHArcEdge>(arcEdge->getCenter(), arcEdge->getNormal(), arcEdge->getRadius() - m_width / 2, angleToXAxis - angle, angleToXAxis + angle);

        auto sourceDirection = (outerEdge->getSourceVertex() - innerEdge->getSourceVertex()).normalize();
        auto targetDirection = (outerEdge->getTargetVertex() - innerEdge->getTargetVertex()).normalize();

        MHGeometry::MHVertex zAxis(0, 0, 1);
        auto innerSource = innerEdge->getSourceVertex();
        auto innerTarget = innerEdge->getTargetVertex();
        auto topInnerSource = innerSource + zAxis * m_height;
        auto topInnerTarget = innerTarget + zAxis * m_height;

        auto outerSource = innerSource + sourceDirection * outerEdge->getRadius() * std::abs(std::cos(MHGeometry::MHToolKit::angleToRadian(angle)));
        auto outerTarget = innerTarget + targetDirection * outerEdge->getRadius() * std::abs(std::cos(MHGeometry::MHToolKit::angleToRadian(angle)));
        auto topOuterSource = outerSource + zAxis * m_height;
        auto topOuterTarget = outerTarget + zAxis * m_height;

        MHGeometry::MHWire wireInner;
        wireInner.addEdge(std::make_shared<MHGeometry::MHLineEdge>(innerSource, innerTarget));
        wireInner.addEdge(std::make_shared<MHGeometry::MHLineEdge>(innerTarget, topInnerTarget));
        wireInner.addEdge(std::make_shared<MHGeometry::MHLineEdge>(topInnerTarget, topInnerSource));
        wireInner.addEdge(std::make_shared<MHGeometry::MHLineEdge>(topInnerSource, innerSource));
        MHGeometry::MHWire wireOuter;
        wireOuter.addEdge(std::make_shared<MHGeometry::MHLineEdge>(outerSource, outerTarget));
        wireOuter.addEdge(std::make_shared<MHGeometry::MHLineEdge>(outerTarget, topOuterTarget));
        wireOuter.addEdge(std::make_shared<MHGeometry::MHLineEdge>(topOuterTarget, topOuterSource));
        wireOuter.addEdge(std::make_shared<MHGeometry::MHLineEdge>(topOuterSource, outerSource));

        m_solidShape = MHGeometry::MHToolKit::makeThruSectionSolid(wireInner, wireOuter);
    }
    if (m_wallEntity) {
        auto self = shared_from_this();
        m_wallEntity->addHole(std::dynamic_pointer_cast<MHHoleEntity>(self));
        m_wallEntity->generateWall3D();
        m_wallEntity->show();
    }
}

std::shared_ptr<MHGeometry::MHPlaneFace> MHHoleEntity::getBaseFace() const {
    return m_baseFace;
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
