/**
 * @file MHFaceToolKit.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include "MHFaceToolKit.h"

#include <BRepBuilderAPI_MakeFace.hxx>

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

}  // namespace MHGeometry::MHToolKit