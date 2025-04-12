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
MH_GEOMETRY_API TopoDS_Face toTopoDSFace(const MHPlaneFace& planeFace);

}  // namespace MHGeometry::MHToolKit
