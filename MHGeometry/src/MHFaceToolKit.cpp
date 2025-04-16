/**
 * @file MHFaceToolKit.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include "MHFaceToolKit.h"

#include <BOPAlgo_Tools.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepBndLib.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRep_Builder.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom_Plane.hxx>
#include <ShapeFix_Face.hxx>
#include <Standard_Type.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Compound.hxx>

#include "MHEdgeToolKit.h"
#include "MHWireToolKit.h"

namespace MHGeometry::MHToolKit {

MH_GEOMETRY_API MHPlaneFace edgeToFace(const MHLineEdge& lineEdge, const MHVertex& direction, double length) {
    auto sourceVertex = lineEdge.getSourceVertex();
    auto targetVertex = lineEdge.getTargetVertex();

    auto normalizeDirection = direction.normalize();
    auto offset = normalizeDirection * length;

    auto newSourceVertex = sourceVertex + offset;
    auto newTargetVertex = targetVertex + offset;

    MHWire wire;
    wire.addEdge(MHLineEdge(sourceVertex, targetVertex));
    wire.addEdge(MHLineEdge(targetVertex, newTargetVertex));
    wire.addEdge(MHLineEdge(newTargetVertex, newSourceVertex));
    wire.addEdge(MHLineEdge(newSourceVertex, sourceVertex));
    MHPlaneFace planeFace;
    planeFace.addWire(wire);
    return planeFace;
}

MH_GEOMETRY_API TopoDS_Face toTopoDSFace(const MHPlaneFace& planeFace) {
    auto wires = planeFace.getWires();
    auto outerWire = wires[0];
    TopoDS_Wire topoDSWire = toTopoDSWire(outerWire);
    BRepBuilderAPI_MakeFace faceMaker(topoDSWire);
    for (size_t i = 1; i < wires.size(); ++i) {
        auto innerWire = wires[i];
        TopoDS_Wire innerTopoDSWire = toTopoDSWire(innerWire);
        faceMaker.Add(innerTopoDSWire);
    }
    return faceMaker.Face();
}

MH_GEOMETRY_API MHFaceType getFaceType(const TopoDS_Face& topoDSFace) {
    Handle(Geom_Surface) surface = BRep_Tool::Surface(topoDSFace);
    if (surface.IsNull()) {
        throw std::runtime_error("The provided face is not valid.");
    }
    if (surface->DynamicType() == STANDARD_TYPE(Geom_Plane)) {
        return MHFaceType::PLANE_FACE;
    } else if (surface->DynamicType() == STANDARD_TYPE(Geom_CylindricalSurface)) {
        return MHFaceType::CYLINDRICAL_FACE;
    } else {
        throw std::runtime_error("Unsupported face type.");
    }
}

MH_GEOMETRY_API MHPlaneFace toMHPlaneFace(const TopoDS_Face& topoDSFace, const MHVertex& normal) {
    ShapeFix_Face shapeFixFace;
    shapeFixFace.Init(topoDSFace);
    shapeFixFace.Perform();
    auto fixedFace = shapeFixFace.Face();
    MHPlaneFace planeFace;
    std::vector<MHWire> wires;
    for (TopExp_Explorer explorer(fixedFace, TopAbs_WIRE); explorer.More(); explorer.Next()) {
        const TopoDS_Wire& wire = TopoDS::Wire(explorer.Current());
        wires.push_back(toMHWire(wire));
    }
    for (int i = 0; i < wires.size(); i++) {
        if (i == 0) {
            planeFace.addWire(changeWireDirection(wires[i], true, normal));
        } else {
            planeFace.addWire(changeWireDirection(wires[i], false, normal));
        }
    }
    return planeFace;
}

MH_GEOMETRY_API std::vector<MHPlaneFace> makeArrangement(const std::vector<std::unique_ptr<MHEdge>>& edges) {
    BRep_Builder builder;
    TopoDS_Compound edgesCompound;
    TopoDS_Compound wiresCompound;
    TopoDS_Compound facesCompound;
    builder.MakeCompound(edgesCompound);
    builder.MakeCompound(wiresCompound);
    builder.MakeCompound(facesCompound);
    for (const auto& edge : edges) {
        if (edge->getEdgeType() == MHEdgeType::LINE_EDGE) {
            auto lineEdge = dynamic_cast<MHLineEdge*>(edge.get());
            TopoDS_Edge topoDSEdge = toTopoDSEdge(*lineEdge);
            builder.Add(edgesCompound, topoDSEdge);
        } else if (edge->getEdgeType() == MHEdgeType::ARC_EDGE) {
            auto arcEdge = dynamic_cast<MHArcEdge*>(edge.get());
            TopoDS_Edge topoDSEdge = toTopoDSEdge(*arcEdge);
            builder.Add(edgesCompound, topoDSEdge);
        }
    }
    auto edgesToWiresResult = BOPAlgo_Tools::EdgesToWires(edgesCompound, wiresCompound);
    auto wireCount = wiresCompound.NbChildren();
    if (edgesToWiresResult != 0 || wireCount == 0) {
        return {};
    }
    auto wiresToFacesResult = BOPAlgo_Tools::WiresToFaces(wiresCompound, facesCompound);
    auto faceCount = facesCompound.NbChildren();
    if (!wiresToFacesResult || faceCount == 0) {
        return {};
    }
    std::vector<TopoDS_Face> topoDSFaces;

    for (TopExp_Explorer explorer(facesCompound, TopAbs_FACE); explorer.More(); explorer.Next()) {
        const TopoDS_Face& topoFace = TopoDS::Face(explorer.Current());
        topoDSFaces.push_back(topoFace);
    }

    std::vector<MHPlaneFace> planeFaces;
    for (const auto& topoFace : topoDSFaces) {
        auto faceType = getFaceType(topoFace);
        if (faceType != MHFaceType::PLANE_FACE) {
            continue;
        }
        planeFaces.push_back(toMHPlaneFace(topoFace, MHVertex(0, 0, 1)));
    }
    return planeFaces;
}

MH_GEOMETRY_API bool isIntersect(const MHPlaneFace& face1, const MHPlaneFace& face2) {
    auto face1TopoDS = toTopoDSFace(face1);
    auto face2TopoDS = toTopoDSFace(face2);
    return isIntersect(face1TopoDS, face2TopoDS);
}

MH_GEOMETRY_API bool isIntersect(const TopoDS_Face& face1, const TopoDS_Face& face2) {
    BRepAlgoAPI_Common common(face1, face2);
    common.Build();
    if (!common.IsDone()) {
        return false;
    }
    auto resultShape = common.Shape();
    if (resultShape.IsNull()) {
        return false;
    }
    for (TopExp_Explorer explorer(resultShape, TopAbs_FACE); explorer.More(); explorer.Next()) {
        const TopoDS_Face& face = TopoDS::Face(explorer.Current());
        if (!face.IsNull()) {
            return true;
        }
    }
    return false;
}

MH_GEOMETRY_API void getFaceSize(const TopoDS_Face& face, double& width, double& height) {
    Bnd_Box boundingBox;
    BRepBndLib::Add(face, boundingBox);

    double xmin, ymin, zmin, xmax, ymax, zmax;
    boundingBox.Get(xmin, ymin, zmin, xmax, ymax, zmax);

    BRepAdaptor_Surface surface(face);
    Handle(Geom_Surface) geomSurface = surface.Surface().Surface();

    if (geomSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)) {
        gp_Pln plane = surface.Plane();
        gp_Dir normal = plane.Axis().Direction();

        if (fabs(normal.X()) > fabs(normal.Y()) && fabs(normal.X()) > fabs(normal.Z())) {
            width = ymax - ymin;
            height = zmax - zmin;
        } else if (fabs(normal.Y()) > fabs(normal.X()) && fabs(normal.Y()) > fabs(normal.Z())) {
            width = xmax - xmin;
            height = zmax - zmin;
        } else {
            width = xmax - xmin;
            height = ymax - ymin;
        }
    } else if (geomSurface->DynamicType() == STANDARD_TYPE(Geom_CylindricalSurface)) {
        Handle(Geom_CylindricalSurface) cylinderSurface = Handle(Geom_CylindricalSurface)::DownCast(geomSurface);
        double radius = cylinderSurface->Radius();
        gp_Ax3 axis = cylinderSurface->Position();
        gp_Dir axisDir = axis.Direction();
        if (fabs(axisDir.X()) > fabs(axisDir.Y()) && fabs(axisDir.X()) > fabs(axisDir.Z())) {
            height = xmax - xmin;
        } else if (fabs(axisDir.Y()) > fabs(axisDir.X()) && fabs(axisDir.Y()) > fabs(axisDir.Z())) {
            height = ymax - ymin;
        } else {
            height = zmax - zmin;
        }

        double uMin, uMax, vMin, vMax;
        uMin = surface.FirstUParameter();
        uMax = surface.LastUParameter();
        vMin = surface.FirstVParameter();
        vMax = surface.LastVParameter();
        double angleRange = uMax - uMin;
        width = angleRange * radius;
        if (fabs(angleRange - 2.0 * M_PI) < 1e-6) {
            width = 2.0 * M_PI * radius;
        }
    } else {
        double dims[3] = {xmax - xmin, ymax - ymin, zmax - zmin};
        std::sort(dims, dims + 3);
        width = dims[1];
        height = dims[2];
    }
}

}  // namespace MHGeometry::MHToolKit