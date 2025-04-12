#pragma once

/**
 * @file MHWireToolKit.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include <TopoDS_Wire.hxx>
#include <vector>

#include "MHWire.h"

namespace MHGeometry::MHToolKit {

MH_GEOMETRY_API TopoDS_Wire toTopoDSWire(const MHWire& wire);
MH_GEOMETRY_API MHWire toMHWire(const TopoDS_Wire& topoDSWire);
MH_GEOMETRY_API MHWire offsetWire(const MHWire& wire, double offsetDistance);

}  // namespace MHGeometry::MHToolKit
