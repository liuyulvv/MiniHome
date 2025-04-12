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

}  // namespace MHGeometry::MHToolKit
