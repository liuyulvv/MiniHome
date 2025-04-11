#pragma once

/**
 * @file MHEdge.hpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHVertex.hpp"

namespace MHGeometry {

enum class MHEdgeType {
    LINE_EDGE,
    ARC_EDGE,
};

class MH_GEOMETRY_API MHEdge : public MHTopoBase {
public:
    MHEdge() = default;
    virtual ~MHEdge() = default;
    MHEdge(const MHEdge& edge) = default;
    MHEdge(MHEdge&& edge) = default;
    MHEdge& operator=(const MHEdge& edge) = default;
    MHEdge& operator=(MHEdge&& edge) = default;

public:
    virtual MHEdgeType getEdgeType() const = 0;
    virtual const MHVertex& getSourceVertex() const = 0;
    virtual const MHVertex& getTargetVertex() const = 0;
};

}  // namespace MHGeometry
