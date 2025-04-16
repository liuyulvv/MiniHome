/**
 * @file MHHouseEntity.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include "MHHouseEntity.h"

#include <vtkCleanPolyData.h>
#include <vtkFeatureEdges.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkTextureMapToCylinder.h>
#include <vtkTextureMapToPlane.h>

#include <IVtkTools_DisplayModeFilter.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <TopoDS.hxx>

#include "MHFace.h"
#include "MHFaceToolKit.h"
#include "MHPlaneFace.h"
#include "MHRendererManager.h"
#include "MHVertex.h"

namespace MHHouse {

MHHouseEntity::MHHouseEntity(vtkSmartPointer<MHCore::MHRenderer> renderer) : MHCore::MHEntity(renderer) {
    m_outlineActor = vtkSmartPointer<vtkActor>::New();
    m_outlineActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    m_outlineActor->GetProperty()->EdgeVisibilityOn();
    m_outlineActor->GetProperty()->LightingOff();
    m_actor->GetProperty()->SetAmbient(0.2);
    m_actor->GetProperty()->SetDiffuse(0.8);
}

MHHouseEntity::~MHHouseEntity() {
    MHCore::MHRendererManager::getInstance().getHoverRenderer()->RemoveActor(m_outlineActor);
}

void MHHouseEntity::setTopo(const TopoDS_Shape& topo) {
    m_topo = topo;
}

void MHHouseEntity::updateTopo() {
    if (!m_topo.IsNull()) {
        if (m_topo.ShapeType() == TopAbs_FACE) {
            auto face = TopoDS::Face(m_topo);
            auto faceType = MHGeometry::MHToolKit::getFaceType(face);
            double width = 0, height = 0;
            MHGeometry::MHToolKit::getFaceSize(face, width, height);

            auto shape = IVtkTools_ShapeDataSource::New();
            shape->SetShape(new IVtkOCC_Shape(m_topo));
            auto source = vtkSmartPointer<IVtkTools_ShapeDataSource>(shape);
            auto displayFilter = vtkSmartPointer<IVtkTools_DisplayModeFilter>::New();
            displayFilter->SetInputConnection(source->GetOutputPort());
            displayFilter->SetDisplayMode(IVtk_DisplayMode::DM_Shading);
            displayFilter->Update();

            auto textureMapper = vtkSmartPointer<vtkDataSetAlgorithm>::New();

            if (faceType == MHGeometry::MHFaceType::PLANE_FACE) {
                textureMapper = vtkSmartPointer<vtkTextureMapToPlane>::New();
            } else if (faceType == MHGeometry::MHFaceType::CYLINDRICAL_FACE) {
                textureMapper = vtkSmartPointer<vtkTextureMapToCylinder>::New();
            }

            textureMapper->SetInputConnection(displayFilter->GetOutputPort());
            textureMapper->Update();
            auto polyData = textureMapper->GetOutput();

            auto tcoords = polyData->GetPointData()->GetTCoords();
            if (tcoords) {
                for (vtkIdType i = 0; i < tcoords->GetNumberOfTuples(); ++i) {
                    double* tc = tcoords->GetTuple2(i);
                    tc[0] *= width / 1000.0;
                    tc[1] *= height / 1000.0;
                    tcoords->SetTuple2(i, tc[0], tc[1]);
                }
                tcoords->Modified();
                polyData->GetPointData()->SetTCoords(tcoords);
                polyData->Modified();
            }
            auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            mapper->SetInputConnection(textureMapper->GetOutputPort());
            m_actor->SetMapper(mapper);

            auto featureEdges = vtkSmartPointer<vtkFeatureEdges>::New();
            featureEdges->SetInputConnection(displayFilter->GetOutputPort());
            featureEdges->BoundaryEdgesOn();
            featureEdges->FeatureEdgesOff();
            featureEdges->NonManifoldEdgesOff();
            featureEdges->ManifoldEdgesOff();
            featureEdges->ColoringOff();
            featureEdges->Update();
            auto outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            outlineMapper->SetInputConnection(featureEdges->GetOutputPort());
            m_outlineActor->SetMapper(outlineMapper);
        } else {
            auto shape = IVtkTools_ShapeDataSource::New();
            shape->SetShape(new IVtkOCC_Shape(m_topo));
            auto source = vtkSmartPointer<IVtkTools_ShapeDataSource>(shape);
            auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            mapper->SetInputConnection(source->GetOutputPort());
            m_actor->SetMapper(mapper);
        }
    }
}

void MHHouseEntity::onEnter() {
    MHCore::MHEntity::onEnter();
    MHCore::MHRendererManager::getInstance().getHoverRenderer()->AddActor(m_outlineActor);
    m_outlineActor->GetProperty()->SetColor(0.0, 1.0, 1.0);
}

void MHHouseEntity::onLeave() {
    MHCore::MHEntity::onLeave();
    MHCore::MHRendererManager::getInstance().getHoverRenderer()->RemoveActor(m_outlineActor);
    m_outlineActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
}

}  // namespace MHHouse
