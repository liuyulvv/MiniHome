#pragma once

/**
 * @file MHEdgeToolKit.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include <TopoDS_Edge.hxx>

#include "MHArcEdge.h"
#include "MHLineEdge.h"

namespace MHGeometry::MHToolKit {

enum class MHLineIntersectionType {
    NONE,
    INTERSECT,
};

MH_GEOMETRY_API TopoDS_Edge toTopoDSEdge(const MHLineEdge& lineEdge);
MH_GEOMETRY_API TopoDS_Edge toTopoDSEdge(const MHArcEdge& arcEdge);
MH_GEOMETRY_API MHEdgeType getEdgeType(const TopoDS_Edge& topoDSEdge);
MH_GEOMETRY_API MHLineEdge toMHLineEdge(const TopoDS_Edge& topoDSEdge);
MH_GEOMETRY_API MHArcEdge toMHArcEdge(const TopoDS_Edge& topoDSEdge);
MH_GEOMETRY_API MHLineIntersectionType getLineIntersectionVertex(const MHLineEdge& lineEdge1, const MHLineEdge& lineEdge2, MHVertex& intersectionVertex);

}  // namespace MHGeometry::MHToolKit
