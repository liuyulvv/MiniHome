/**
 * @file MHWire.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include "MHWire.h"

namespace MHGeometry {

MHWire::MHWire(const MHWire& wire) {
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
                m_edges.push_back(std::make_unique<MHArcEdge>(arcEdge->getCenter(), arcEdge->getNormal(), arcEdge->getRadius(), arcEdge->getSourceAngle(), arcEdge->getTargetAngle(), arcEdge->getOrientation()));
                break;
            }
            default:
                break;
        }
    }
};

MHWire& MHWire::operator=(const MHWire& wire) {
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
                case MHEdgeType::ARC_EDGE: {
                    auto arcEdge = dynamic_cast<MHArcEdge*>(edge.get());
                    m_edges.push_back(std::make_unique<MHArcEdge>(arcEdge->getCenter(), arcEdge->getNormal(), arcEdge->getRadius(), arcEdge->getSourceAngle(), arcEdge->getTargetAngle(), arcEdge->getOrientation()));
                    break;
                }
                default:
                    break;
            }
        }
    }
    return *this;
}

MHTopoType MHWire::getType() const {
    return MHTopoType::WIRE;
}

std::unique_ptr<MHTopoBase> MHWire::clone() const {
    return std::make_unique<MHWire>(*this);
}

void MHWire::applyTransform(vtkSmartPointer<vtkTransform> transform) {
    for (const auto& edge : m_edges) {
        edge->applyTransform(transform);
    }
}

void MHWire::reversed() {
    for (auto& edge : m_edges) {
        edge->reversed();
    }
}

void MHWire::addEdge(const MHLineEdge& edge) {
    m_edges.push_back(std::make_unique<MHLineEdge>(edge.getSourceVertex(), edge.getTargetVertex()));
}

void MHWire::addEdge(const MHArcEdge& edge) {
    m_edges.push_back(std::make_unique<MHArcEdge>(edge.getCenter(), edge.getNormal(), edge.getRadius(), edge.getSourceAngle(), edge.getTargetAngle(), edge.getOrientation()));
}

void MHWire::addEdge(std::unique_ptr<MHEdge> edge) {
    m_edges.push_back(std::move(edge));
}

const std::vector<std::unique_ptr<MHEdge>>& MHWire::getEdges() const {
    return m_edges;
}

}  // namespace MHGeometry
