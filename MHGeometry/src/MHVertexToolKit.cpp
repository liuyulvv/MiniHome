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

MH_GEOMETRY_API bool areCollinear(const MHVertex& vertex1, const MHVertex& vertex2, const MHVertex& vertex3, double tolerance) {
    auto v1 = vertex2 - vertex1;
    auto v2 = vertex3 - vertex1;
    if (v1.length() < tolerance || v2.length() < tolerance) {
        return true;
    }
    auto cross = v1.cross(v2);
    auto crossLength = cross.length();
    auto normalizationFactor = v1.length() * v2.length();
    auto normalizedCrossLength = crossLength / normalizationFactor;
    return normalizedCrossLength < tolerance;
}

MH_GEOMETRY_API bool isParallelToXAxis(const MHVertex& vertex1, const MHVertex& vertex2, double tolerance) {
    MHVertex v = vertex2 - vertex1;
    double length = v.length();
    if (length < tolerance) {
        return true;
    }
    v = v.normalize();
    return (std::abs(v.y) < tolerance && std::abs(v.z) < tolerance);
}

MH_GEOMETRY_API bool isParallelToYAxis(const MHVertex& vertex1, const MHVertex& vertex2, double tolerance) {
    MHVertex v = vertex2 - vertex1;
    auto length = v.length();
    if (length < tolerance) {
        return true;
    }
    v = v.normalize();
    return (std::abs(v.x) < tolerance && std::abs(v.z) < tolerance);
}

MH_GEOMETRY_API bool isLeftTurn(const MHVertex& vertex1, const MHVertex& vertex2, const MHVertex& vertex3, double tolerance) {
    auto v1 = vertex2 - vertex1;
    auto v2 = vertex3 - vertex1;
    if (v1.length() < tolerance || v2.length() < tolerance) {
        return false;
    }
    auto cross = v1.cross(v2);
    return cross.z > -tolerance;
}

MH_GEOMETRY_API double angleToXAxis(const MHVertex& vertex1, const MHVertex& vertex2) {
    MHVertex v = vertex2 - vertex1;
    auto angle = std::atan2(v.y, v.x);
    return radianToAngle(angle);
}

}  // namespace MHGeometry::MHToolKit
