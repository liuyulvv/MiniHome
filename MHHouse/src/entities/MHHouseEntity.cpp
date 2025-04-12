/**
 * @file MHHouseEntity.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include "MHHouseEntity.h"

#include <vtkCleanPolyData.h>
#include <vtkFeatureEdges.h>
#include <vtkPolyDataMapper.h>
#include <vtkTextureMapToPlane.h>

#include <IVtkTools_ShapeDataSource.hxx>

#include "MHFace.h"
#include "MHFaceToolKit.h"
#include "MHPlaneFace.h"
#include "MHVertex.h"

namespace MHHouse {

MHHouseEntity::MHHouseEntity(vtkSmartPointer<MHCore::MHRenderer> renderer) : MHCore::MHEntity(renderer) {
    m_outlineActor = vtkSmartPointer<vtkActor>::New();
    m_outlineActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    m_outlineActor->GetProperty()->EdgeVisibilityOn();
    m_actor->GetProperty()->LightingOff();
}

MHHouseEntity::~MHHouseEntity() {
    m_renderer->RemoveActor(m_outlineActor);
}

void MHHouseEntity::show(bool forceRender) {
    m_renderer->AddActor(m_actor);
    m_renderer->AddActor(m_outlineActor);
    for (const auto& child : m_children) {
        if (child) {
            child->show(false);
        }
    }
    if (forceRender) {
        m_renderer->render();
    }
}

void MHHouseEntity::updateTopo() {
    if (m_topo) {
        auto topoType = m_topo->getType();
        TopoDS_Shape topoDSShape;
        switch (topoType) {
            case MHGeometry::MHTopoType::FACE: {
                auto face = dynamic_cast<MHGeometry::MHFace*>(m_topo.get());
                auto faceType = face->getFaceType();
                switch (faceType) {
                    case MHGeometry::MHFaceType::PLANE_FACE: {
                        auto planeFace = dynamic_cast<MHGeometry::MHPlaneFace*>(face);
                        topoDSShape = MHGeometry::MHToolKit::toTopoDSFace(*planeFace);
                        break;
                    }
                }
                break;
            }
        }
        if (!topoDSShape.IsNull()) {
            auto shape = IVtkTools_ShapeDataSource::New();
            shape->SetShape(new IVtkOCC_Shape(topoDSShape));
            auto source = vtkSmartPointer<IVtkTools_ShapeDataSource>(shape);
            auto cleanFilter = vtkSmartPointer<vtkCleanPolyData>::New();
            cleanFilter->SetInputConnection(source->GetOutputPort());
            cleanFilter->SetTolerance(1e-6);
            cleanFilter->Update();
            auto textureMapper = vtkSmartPointer<vtkTextureMapToPlane>::New();
            textureMapper->SetInputConnection(cleanFilter->GetOutputPort());
            textureMapper->Update();
            auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            mapper->SetInputConnection(textureMapper->GetOutputPort());
            m_actor->SetMapper(mapper);

            auto featureEdges = vtkSmartPointer<vtkFeatureEdges>::New();
            featureEdges->SetInputConnection(cleanFilter->GetOutputPort());
            featureEdges->BoundaryEdgesOn();
            featureEdges->FeatureEdgesOff();
            featureEdges->NonManifoldEdgesOff();
            featureEdges->ManifoldEdgesOff();
            featureEdges->ColoringOff();
            featureEdges->Update();
            auto outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            outlineMapper->SetInputConnection(featureEdges->GetOutputPort());
            m_outlineActor->SetMapper(outlineMapper);
        }
    }
}

}  // namespace MHHouse
