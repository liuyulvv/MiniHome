/**
 * @file MHWireToolKit.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHWireToolKit.h"

#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepGProp.hxx>
#include <BRepOffsetAPI_MakeOffset.hxx>
#include <ShapeAnalysis.hxx>
#include <ShapeExtend_WireData.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <gp_Pln.hxx>

#include "MHEdgeToolKit.h"

namespace MHGeometry::MHToolKit {

MH_GEOMETRY_API TopoDS_Wire toTopoDSWire(const MHWire& wire) {
    std::vector<TopoDS_Edge> topoDSEdges;
    BRepBuilderAPI_MakeWire wireBuilder;
    const auto& edges = wire.getEdges();
    for (const auto& edge : edges) {
        if (edge->getEdgeType() == MHEdgeType::LINE_EDGE) {
            auto lineEdge = dynamic_cast<MHLineEdge*>(edge.get());
            TopoDS_Edge topoDSEdge = toTopoDSEdge(*lineEdge);
            topoDSEdges.push_back(topoDSEdge);
        } else if (edge->getEdgeType() == MHEdgeType::ARC_EDGE) {
            auto arcEdge = dynamic_cast<MHArcEdge*>(edge.get());
            TopoDS_Edge topoDSEdge = toTopoDSEdge(*arcEdge);
            topoDSEdges.push_back(topoDSEdge);
        }
    }
    for (const auto& topoDSEdge : topoDSEdges) {
        wireBuilder.Add(topoDSEdge);
    }
    auto topoDSWire = wireBuilder.Wire();
    return topoDSWire;
}

MH_GEOMETRY_API MHWire toMHWire(const TopoDS_Wire& topoDSWire) {
    MHWire wire;
    for (TopExp_Explorer edgeExplorer(topoDSWire, TopAbs_EDGE); edgeExplorer.More(); edgeExplorer.Next()) {
        const TopoDS_Edge& topoDSEdge = TopoDS::Edge(edgeExplorer.Current());
        auto edgeType = getEdgeType(topoDSEdge);
        if (edgeType == MHEdgeType::LINE_EDGE) {
            wire.addEdge(toMHLineEdge(topoDSEdge));
        } else if (edgeType == MHEdgeType::ARC_EDGE) {
            wire.addEdge(toMHArcEdge(topoDSEdge));
        }
    }
    return wire;
}

MH_GEOMETRY_API MHWire offsetWire(const MHWire& wire, double offsetDistance) {
    auto topoDSWire = toTopoDSWire(wire);
    BRepOffsetAPI_MakeOffset offsetMaker(topoDSWire, GeomAbs_JoinType::GeomAbs_Intersection);
    offsetMaker.Perform(offsetDistance);
    auto offsetTopoDSWire = offsetMaker.Shape();
    return toMHWire(TopoDS::Wire(offsetTopoDSWire));
}

MH_GEOMETRY_API bool isWireCounterClockWise(const MHWire& wire, const MHVertex& normal) {
    auto topoDSWire = toTopoDSWire(wire);
    return isWireCounterClockWise(topoDSWire, normal);
}

MH_GEOMETRY_API bool isWireCounterClockWise(const TopoDS_Wire& wire, const MHVertex& normal) {
    gp_Pln plane(gp_Pnt(0, 0, 0), gp_Dir(normal.x, normal.y, normal.z));
    TopoDS_Face face = BRepBuilderAPI_MakeFace(plane, -1.0, 1.0, -1.0, 1.0);
    Handle(ShapeExtend_WireData) wireData = new ShapeExtend_WireData(wire);
    double area = ShapeAnalysis::TotCross2D(wireData, face);
    return area > 0;
}

MH_GEOMETRY_API MHWire changeWireDirection(MHWire& wire, bool counterClockWise, const MHVertex& normal) {
    if (counterClockWise == isWireCounterClockWise(wire, normal)) {
        return wire;
    }
    wire.reversed();
    return wire;
}

}  // namespace MHGeometry::MHToolKit
