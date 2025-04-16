#pragma once

/**
 * @file MHVertexToolKit.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include <MHVertex.h>

#include <TopoDS_Vertex.hxx>
#include <gp_Pnt.hxx>

namespace MHGeometry::MHToolKit {

MH_GEOMETRY_API double angleToRadian(double angle);
MH_GEOMETRY_API double radianToAngle(double radian);
MH_GEOMETRY_API TopoDS_Vertex toTopoDSVertex(const MHVertex& vertex);
MH_GEOMETRY_API MHVertex toMHVertex(const TopoDS_Vertex& vertex);
MH_GEOMETRY_API bool areCollinear(const MHVertex& vertex1, const MHVertex& vertex2, const MHVertex& vertex3, double tolerance = 1e-6);
MH_GEOMETRY_API bool isParallelToXAxis(const MHVertex& vertex1, const MHVertex& vertex2, double tolerance = 1e-6);
MH_GEOMETRY_API bool isParallelToYAxis(const MHVertex& vertex1, const MHVertex& vertex2, double tolerance = 1e-6);
MH_GEOMETRY_API bool isLeftTurn(const MHVertex& vertex1, const MHVertex& vertex2, const MHVertex& vertex3, double tolerance = 1e-6);
MH_GEOMETRY_API double angleToXAxis(const MHVertex& vertex1, const MHVertex& vertex2);

}  // namespace MHGeometry::MHToolKit
