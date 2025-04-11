#pragma once

/**
 * @file MHLineEdge.hpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include <vector>

#include "MHEdge.hpp"

namespace MHGeometry {

class MH_GEOMETRY_API MHLineEdge : public MHEdge {
public:
    MHLineEdge() = default;
    MHLineEdge(const MHVertex& source, const MHVertex& target) : m_sourceVertex(source), m_targetVertex(target) {}
    ~MHLineEdge() = default;
    MHLineEdge(const MHLineEdge& edge) = default;
    MHLineEdge(MHLineEdge&& edge) = default;
    MHLineEdge& operator=(const MHLineEdge& edge) = default;
    MHLineEdge& operator=(MHLineEdge&& edge) = default;

public:
    virtual MHTopoType getType() const override {
        return MHTopoType::EDGE;
    }

    virtual MHEdgeType getEdgeType() const override {
        return MHEdgeType::LINE_EDGE;
    }

    virtual MHTopoBase* clone() const override {
        return new MHLineEdge(*this);
    }

    virtual const MHVertex& getSourceVertex() const override {
        return m_sourceVertex;
    }

    virtual const MHVertex& getTargetVertex() const override {
        return m_targetVertex;
    }

private:
    MHVertex m_sourceVertex;
    MHVertex m_targetVertex;
};

}  // namespace MHGeometry
