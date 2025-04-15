/**
 * @file MHEdgeToolKit.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHEdgeToolKit.h"

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Line.hxx>
#include <Standard_Type.hxx>
#include <gp_Ax2.hxx>
#include <gp_Circ.hxx>

#include "MHVertexToolKit.h"

namespace MHGeometry::MHToolKit {

MH_GEOMETRY_API TopoDS_Edge toTopoDSEdge(const MHLineEdge& lineEdge) {
    TopoDS_Vertex sourceTopoDSVertex = toTopoDSVertex(lineEdge.getSourceVertex());
    TopoDS_Vertex targetTopoDSVertex = toTopoDSVertex(lineEdge.getTargetVertex());
    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(sourceTopoDSVertex, targetTopoDSVertex);
    return edge;
}

MH_GEOMETRY_API TopoDS_Edge toTopoDSEdge(const MHArcEdge& arcEdge) {
    auto centerVertex = arcEdge.getCenter();
    auto arcNormal = arcEdge.getNormal();
    gp_Dir normal(arcNormal.x, arcNormal.y, arcNormal.z);
    gp_Ax2 axis(gp_Pnt(centerVertex.x, centerVertex.y, centerVertex.z), normal);
    gp_Circ circle(axis, arcEdge.getRadius());
    Standard_Real sourceRadian = angleToRadian(arcEdge.getSourceAngle());
    Standard_Real targetRadian = angleToRadian(arcEdge.getTargetAngle());
    auto geomCircle = new Geom_Circle(circle);
    if (arcEdge.getOrientation() == MHTopoOrientation::CLOCK_WISE) {
        std::swap(sourceRadian, targetRadian);
    }
    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(geomCircle, sourceRadian, targetRadian);
    if (arcEdge.getOrientation() == MHTopoOrientation::CLOCK_WISE) {
        edge.Reverse();
    }
    return edge;
}

MH_GEOMETRY_API MHEdgeType getEdgeType(const TopoDS_Edge& topoDSEdge) {
    Standard_Real first, last;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(topoDSEdge, first, last);
    if (curve.IsNull()) {
        throw std::runtime_error("The provided edge is not valid.");
    }
    if (curve->DynamicType() == STANDARD_TYPE(Geom_Line)) {
        return MHEdgeType::LINE_EDGE;
    } else if (curve->DynamicType() == STANDARD_TYPE(Geom_Circle)) {
        return MHEdgeType::ARC_EDGE;
    } else {
        throw std::runtime_error("Unsupported edge type.");
    }
}

MH_GEOMETRY_API MHLineEdge toMHLineEdge(const TopoDS_Edge& topoDSEdge) {
    Standard_Real first, last;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(topoDSEdge, first, last);
    if (curve.IsNull() || curve->DynamicType() != STANDARD_TYPE(Geom_Line)) {
        throw std::runtime_error("The provided edge is not a line edge.");
    }
    gp_Pnt start = curve->Value(first);
    gp_Pnt target = curve->Value(last);
    MHVertex sourceVertex(start.X(), start.Y(), start.Z());
    MHVertex targetVertex(target.X(), target.Y(), target.Z());
    return MHLineEdge(sourceVertex, targetVertex);
}

MH_GEOMETRY_API MHArcEdge toMHArcEdge(const TopoDS_Edge& topoDSEdge) {
    Standard_Real first, last;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(topoDSEdge, first, last);
    if (curve.IsNull() || curve->DynamicType() != STANDARD_TYPE(Geom_Circle)) {
        throw std::runtime_error("The provided edge is not a circular arc.");
    }
    Handle(Geom_Circle) geomCircle = Handle(Geom_Circle)::DownCast(curve);
    const gp_Circ& circ = geomCircle->Circ();
    gp_Pnt center = circ.Location();
    gp_Dir normal = circ.Axis().Direction();
    Standard_Real radius = circ.Radius();
    gp_Pnt startPoint = geomCircle->Value(first);
    gp_Pnt endPoint = geomCircle->Value(last);
    gp_Vec centerToStart(center, startPoint);
    gp_Vec centerToEnd(center, endPoint);
    Standard_Real sourceRadian = atan2(centerToStart.Y(), centerToStart.X());
    Standard_Real targetRadian = atan2(centerToEnd.Y(), centerToEnd.X());
    MHTopoOrientation orientation = MHTopoOrientation::COUNTER_CLOCK_WISE;
    MHVertex centerVertex(center.X(), center.Y(), center.Z());
    MHVertex normalVertex(normal.X(), normal.Y(), normal.Z());
    return MHArcEdge(centerVertex, normalVertex, radius, radianToAngle(sourceRadian), radianToAngle(targetRadian), orientation);
}

}  // namespace MHGeometry::MHToolKit