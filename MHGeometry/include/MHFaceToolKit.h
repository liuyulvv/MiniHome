#pragma once

/**
 * @file MHFaceToolKit.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include <TopoDS_Face.hxx>

#include "MHLineEdge.h"
#include "MHPlaneFace.h"

namespace MHGeometry::MHToolKit {

MH_GEOMETRY_API MHPlaneFace edgeToFace(const MHLineEdge& lineEdge, const MHVertex& direction, double length);
MH_GEOMETRY_API std::vector<TopoDS_Face> makePrismFace(const MHPlaneFace& face, const MHVertex& direction, double length);
MH_GEOMETRY_API TopoDS_Shape makePrismSolid(const MHPlaneFace& face, const MHVertex& direction, double length);
MH_GEOMETRY_API TopoDS_Face toTopoDSFace(const MHPlaneFace& planeFace);
MH_GEOMETRY_API MHFaceType getFaceType(const TopoDS_Face& topoDSFace);
MH_GEOMETRY_API MHPlaneFace toMHPlaneFace(const TopoDS_Face& topoDSFace, const MHVertex& normal);
MH_GEOMETRY_API std::vector<MHPlaneFace> makeArrangement(const std::vector<std::unique_ptr<MHEdge>>& edges);
MH_GEOMETRY_API bool isIntersect(const MHPlaneFace& face1, const MHPlaneFace& face2);
MH_GEOMETRY_API bool isIntersect(const TopoDS_Face& face1, const TopoDS_Face& face2);
MH_GEOMETRY_API TopoDS_Shape booleanDifference(const TopoDS_Shape& mainShape, const std::vector<TopoDS_Shape>& toolShapes);
MH_GEOMETRY_API std::vector<TopoDS_Face> getFaces(const TopoDS_Shape& shape);
MH_GEOMETRY_API TopoDS_Shape makeThruSectionSolid(const MHWire& wire1, const MHWire& wire2);
MH_GEOMETRY_API std::vector<TopoDS_Face> makeThruSectionFace(const MHWire& wire1, const MHWire& wire2);

}  // namespace MHGeometry::MHToolKit
