/**
 * @file MHHouseEntity.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include "MHHouseEntity.h"

#include <vtkCleanPolyData.h>
#include <vtkFeatureEdges.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkTextureMapToCylinder.h>
#include <vtkTextureMapToPlane.h>

#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <TopoDS.hxx>

#include "MHFace.h"
#include "MHFaceToolKit.h"
#include "MHPlaneFace.h"
#include "MHRendererManager.h"
#include "MHSpaceManager.h"
#include "MHVertex.h"

namespace MHHouse {

MHHouseEntity::MHHouseEntity(vtkSmartPointer<MHCore::MHRenderer> renderer) : MHCore::MHEntity(renderer) {
    m_outlineActor = vtkSmartPointer<vtkActor>::New();
    m_outlineActor->GetProperty()->EdgeVisibilityOn();
    m_outlineActor->GetProperty()->LightingOff();
    m_actor->GetProperty()->SetAmbient(1.0);
    m_actor->GetProperty()->SetDiffuse(0.2);
}

MHHouseEntity::~MHHouseEntity() {
    MHCore::MHRendererManager::getInstance().getHoverRenderer()->RemoveActor(m_outlineActor);
}

void MHHouseEntity::show(bool forceRender) {
    MHEntity::show(forceRender);
    for (auto& child : m_children) {
        if (child) {
            child->getActor()->setEntity(shared_from_this());
        }
    }
}

void MHHouseEntity::setTopo(const TopoDS_Shape& topo) {
    m_topo = topo;
}

void MHHouseEntity::updateTopo() {
    if (!m_topo.IsNull()) {
        if (m_topo.ShapeType() == TopAbs_FACE) {
            auto face = TopoDS::Face(m_topo);
            BRepMesh_IncrementalMesh mesher(face, 10.0);
            mesher.Perform();
            TopLoc_Location loc;
            Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, loc);

            if (triangulation.IsNull()) {
                auto faceType = MHGeometry::MHToolKit::getFaceType(face);
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
                auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper->SetInputConnection(textureMapper->GetOutputPort());
                mapper->SetResolveCoincidentTopologyToPolygonOffset();
                mapper->SetRelativeCoincidentTopologyPolygonOffsetParameters(m_offsetFactor, m_offsetUnits);
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
                return;
            } else {
                vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
                vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();
                vtkSmartPointer<vtkFloatArray> uvCoords = vtkSmartPointer<vtkFloatArray>::New();
                uvCoords->SetNumberOfComponents(2);
                uvCoords->SetName("TextureCoordinates");
                bool hasUVCoords = triangulation->HasUVNodes();
                const bool reversed = (face.Orientation() == TopAbs_REVERSED);
                for (Standard_Integer i = 1; i <= triangulation->NbNodes(); ++i) {
                    gp_Pnt pt = triangulation->Node(i);
                    if (!loc.IsIdentity()) {
                        pt.Transform(loc.Transformation());
                    }
                    points->InsertNextPoint(pt.X(), pt.Y(), pt.Z());
                    if (hasUVCoords) {
                        gp_Pnt2d uv = triangulation->UVNode(i);
                        float u = uv.X() / 1000.0;
                        float v = uv.Y() / 1000.0;
                        uvCoords->InsertNextTuple2(u, v);
                    }
                }
                const Poly_Array1OfTriangle& triangleArray = triangulation->Triangles();
                for (Standard_Integer i = 1; i <= triangulation->NbTriangles(); ++i) {
                    Poly_Triangle triangle = triangleArray(i);
                    Standard_Integer n1, n2, n3;
                    triangle.Get(n1, n2, n3);
                    if (reversed) {
                        std::swap(n2, n3);
                    }
                    vtkIdType tri[3] = {n1 - 1, n2 - 1, n3 - 1};
                    triangles->InsertNextCell(3, tri);
                }
                vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
                polyData->SetPoints(points);
                polyData->SetPolys(triangles);
                if (hasUVCoords) {
                    polyData->GetPointData()->SetTCoords(uvCoords);
                }
                auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper->SetInputData(polyData);
                mapper->SetResolveCoincidentTopologyToPolygonOffset();
                mapper->SetRelativeCoincidentTopologyPolygonOffsetParameters(m_offsetFactor, m_offsetUnits);
                m_actor->SetMapper(mapper);
                auto featureEdges = vtkSmartPointer<vtkFeatureEdges>::New();
                featureEdges->SetInputData(polyData);
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

void MHHouseEntity::onEnter(const MHCore::MHEntityInteractorInfo& info) {
    MHCore::MHEntity::onEnter(info);
    if (m_selected) {
        return;
    }
    MHCore::MHRendererManager::getInstance().getHoverRenderer()->AddActor(m_outlineActor);
    m_outlineActor->SetTexture(m_hoveredTexture);
    for (auto& child : m_children) {
        if (child) {
            child->onEnter(info);
        }
    }
}

void MHHouseEntity::onLeave(const MHCore::MHEntityInteractorInfo& info) {
    MHCore::MHEntity::onLeave(info);
    if (m_selected) {
        return;
    }
    MHCore::MHRendererManager::getInstance().getHoverRenderer()->RemoveActor(m_outlineActor);
    for (auto& child : m_children) {
        if (child) {
            child->onLeave(info);
        }
    }
}

void MHHouseEntity::onSelected(const MHCore::MHEntityInteractorInfo& info) {
    MHEntity::onSelected(info);
    if (m_selected) {
        MHCore::MHRendererManager::getInstance().getHoverRenderer()->AddActor(m_outlineActor);
        m_outlineActor->SetTexture(m_selectedTexture);
    } else {
        MHCore::MHRendererManager::getInstance().getHoverRenderer()->RemoveActor(m_outlineActor);
        m_outlineActor->SetTexture(m_hoveredTexture);
    }
    for (auto& child : m_children) {
        if (child) {
            child->onSelected(info);
        }
    }
}

void MHHouseEntity::onDelete() {
    MHEntity::onDelete();
    MHCore::MHRendererManager::getInstance().getHoverRenderer()->RemoveActor(m_outlineActor);
    MHSpaceManager::getInstance().generateSpaces();
}

void MHHouseEntity::setPolygonOffset(double factor, double units) {
    m_offsetFactor = factor;
    m_offsetUnits = units;
}

}  // namespace MHHouse
