#pragma once

/**
 * @file MHWire.hpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include <memory>
#include <vector>

#include "MHArcEdge.hpp"
#include "MHEdge.hpp"
#include "MHLineEdge.hpp"

namespace MHGeometry {

class MH_GEOMETRY_API MHWire : public MHTopoBase {
public:
    MHWire() = default;
    ~MHWire() = default;
    MHWire(const MHWire& wire) {
        m_edges.reserve(wire.m_edges.size());
        for (const auto& edge : wire.m_edges) {
            switch (edge->getEdgeType()) {
                case MHEdgeType::LINE_EDGE: {
                    auto lineEdge = dynamic_cast<MHLineEdge*>(edge.get());
                    m_edges.push_back(std::make_unique<MHLineEdge>(lineEdge->getSourceVertex(), lineEdge->getTargetVertex()));
                    break;
                }
                case MHEdgeType::ARC_EDGE: {
                    auto arcEdge = dynamic_cast<MHArcEdge*>(edge.get());
                    m_edges.push_back(std::make_unique<MHArcEdge>(arcEdge->getCenter(), arcEdge->getRadius(), arcEdge->getSourceAngle(), arcEdge->getTargetAngle(), arcEdge->getOrientation()));
                    break;
                }
                default:
                    break;
            }
        }
    };
    MHWire(MHWire&& wire) = default;
    MHWire& operator=(const MHWire& wire) {
        if (this != &wire) {
            m_edges.clear();
            m_edges.reserve(wire.m_edges.size());
            for (const auto& edge : wire.m_edges) {
                switch (edge->getEdgeType()) {
                    case MHEdgeType::LINE_EDGE: {
                        auto lineEdge = dynamic_cast<MHLineEdge*>(edge.get());
                        m_edges.push_back(std::make_unique<MHLineEdge>(lineEdge->getSourceVertex(), lineEdge->getTargetVertex()));
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        return *this;
    }
    MHWire& operator=(MHWire&& wire) = default;

public:
    virtual MHTopoType getType() const override {
        return MHTopoType::WIRE;
    }

    virtual MHTopoBase* clone() const override {
        return new MHWire(*this);
    }

public:
    void addEdge(const MHLineEdge& edge) {
        m_edges.push_back(std::make_unique<MHLineEdge>(edge.getSourceVertex(), edge.getTargetVertex()));
    }

    void addEdge(const MHArcEdge& edge) {
        m_edges.push_back(std::make_unique<MHArcEdge>(edge.getCenter(), edge.getRadius(), edge.getSourceAngle(), edge.getTargetAngle(), edge.getOrientation()));
    }

    void addEdge(std::unique_ptr<MHEdge> edge) {
        m_edges.push_back(std::move(edge));
    }

    const std::vector<std::unique_ptr<MHEdge>>& getEdges() const {
        return m_edges;
    }

private:
    std::vector<std::unique_ptr<MHEdge>> m_edges;
};

}  // namespace MHGeometry
