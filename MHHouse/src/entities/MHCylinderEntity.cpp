/**
 * @file MHCylinderEntity.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-14
 */

#include "MHCylinderEntity.h"

#include <vtkImageReader2.h>
#include <vtkImageReader2Factory.h>

#include "MHFaceToolKit.h"
#include "MHRendererManager.h"

namespace MHHouse {

MHCylinderEntity::MHCylinderEntity(vtkSmartPointer<MHCore::MHRenderer> renderer) : MHHouseEntity(renderer) {
    createDefaultTexture();
    m_actor->SetTexture(m_texture);
}

void MHCylinderEntity::updateCylinder(const MHGeometry::MHVertex& center, double radius, double height) {
    m_center = std::make_unique<MHGeometry::MHVertex>(center);
    m_radius = radius;
    m_height = height;
}

void MHCylinderEntity::generateCylinder2D() {
    if (!m_center) {
        return;
    }
    m_edge = std::make_unique<MHGeometry::MHArcEdge>(*m_center, m_radius);
    MHGeometry::MHPlaneFace baseFace;
    MHGeometry::MHWire baseWire;
    baseWire.addEdge(*m_edge);
    baseFace.addWire(baseWire);
    m_baseFace = std::make_unique<MHGeometry::MHPlaneFace>(baseFace);
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->Identity();
    transform->Translate(0, 0, m_height);
    baseFace.applyTransform(transform);
    if (!m_cylinder2D) {
        m_cylinder2D = std::make_shared<MHHouseEntity>(MHCore::MHRendererManager::getInstance().getMain2DRenderer());
        m_cylinder2D->setTexture(m_cylinder2DTexture);
        addChild(m_cylinder2D);
        m_cylinder2D->setLayerMask(MHCore::MHEntityLayerMask::LAYER_2D);
    }
    m_cylinder2D->setTopo(MHGeometry::MHToolKit::toTopoDSFace(baseFace));
    m_cylinder2D->updateTopo();
}

void MHCylinderEntity::generateCylinder3D() {
    if (!m_baseFace || !m_edge) {
        return;
    }
    auto topoDSShapes = MHGeometry::MHToolKit::makePrism(*m_baseFace, MHGeometry::MHVertex(0, 0, 1), m_height);
    for (int i = 0; i < topoDSShapes.size(); ++i) {
        auto entity = std::make_shared<MHHouseEntity>(MHCore::MHRendererManager::getInstance().getMain3DRenderer());
        entity->setTopo(topoDSShapes[i]);
        entity->updateTopo();
        entity->setTexture(m_texture);
        addChild(entity);
        entity->setLayerMask(MHCore::MHEntityLayerMask::LAYER_3D);
    }
}

std::unique_ptr<MHGeometry::MHArcEdge> MHCylinderEntity::getEdge() {
    return std::make_unique<MHGeometry::MHArcEdge>(*m_edge);
}

std::unique_ptr<MHGeometry::MHPlaneFace> MHCylinderEntity::getBaseFace() const {
    if (m_baseFace) {
        return std::make_unique<MHGeometry::MHPlaneFace>(*m_baseFace);
    }
    return nullptr;
}

void MHCylinderEntity::createDefaultTexture() {
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
    m_cylinder2DTexture = vtkSmartPointer<vtkTexture>::New();
    m_cylinder2DTexture->SetInputConnection(textureReader2D->GetOutputPort());
    m_cylinder2DTexture->InterpolateOn();
}

}  // namespace MHHouse
