/**
 * @file MHCylinderEntity.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-14
 */

#include "MHCylinderEntity.h"

#include <vtkImageReader2.h>
#include <vtkImageReader2Factory.h>

#include <BRepPrimAPI_MakePrism.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>

#include "MHFaceToolKit.h"

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
        m_cylinder2D = std::make_shared<MHHouseEntity>();
        m_cylinder2D->setTexture(m_texture);
        addChild(m_cylinder2D);
    }
    m_cylinder2D->setTopo(MHGeometry::MHToolKit::toTopoDSFace(baseFace));
    m_cylinder2D->updateTopo();
}

void MHCylinderEntity::generateCylinder3D() {
    if (!m_baseFace || !m_edge) {
        return;
    }
    auto topoShape = MHGeometry::MHToolKit::toTopoDSFace(*m_baseFace);
    BRepPrimAPI_MakePrism makePrism(topoShape, gp_Vec(0, 0, m_height));
    TopoDS_Shape prism = makePrism.Shape();
    auto bottomShape = makePrism.FirstShape();
    auto topShape = makePrism.LastShape();
    for (TopExp_Explorer explorer(prism, TopAbs_FACE); explorer.More(); explorer.Next()) {
        auto currentShape = explorer.Current();
        if (currentShape.IsSame(topShape) || currentShape.IsSame(bottomShape)) {
            continue;
        }
        auto entity = std::make_shared<MHHouseEntity>();
        const TopoDS_Face& topoFace = TopoDS::Face(explorer.Current());
        entity->setTopo(topoFace);
        entity->updateTopo();
        entity->setTexture(m_texture);
        addChild(entity);
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
}

}  // namespace MHHouse
