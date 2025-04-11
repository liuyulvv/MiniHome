/**
 * @file MHVertexToolKit.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHVertexToolKit.h"

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRep_Tool.hxx>
#include <numbers>

namespace MHGeometry::MHToolKit {

MH_GEOMETRY_API double angleToRadian(double angle) {
    return angle * std::numbers::pi / 180.0;
}

MH_GEOMETRY_API double radianToAngle(double radian) {
    return radian * 180.0 / std::numbers::pi;
}

MH_GEOMETRY_API TopoDS_Vertex toTopoDSVertex(const MHVertex& vertex) {
    return BRepBuilderAPI_MakeVertex(gp_Pnt(vertex.x, vertex.y, vertex.z));
}

MH_GEOMETRY_API MHVertex toMHVertex(const TopoDS_Vertex& vertex) {
    gp_Pnt point = BRep_Tool::Pnt(vertex);
    return MHVertex(point.X(), point.Y(), point.Z());
}

}  // namespace MHGeometry::MHToolKit
