/**
 * @file MHLineEdge.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include "MHLineEdge.h"

namespace MHGeometry {

MHLineEdge::MHLineEdge(const MHVertex& source, const MHVertex& target) : m_sourceVertex(source), m_targetVertex(target) {}

MHEdgeType MHLineEdge::getEdgeType() const {
    return MHEdgeType::LINE_EDGE;
}

MHTopoBase* MHLineEdge::clone() const {
    return new MHLineEdge(*this);
}

void MHLineEdge::applyTransform(vtkSmartPointer<vtkTransform> transform) {
    m_sourceVertex.applyTransform(transform);
    m_targetVertex.applyTransform(transform);
}

const MHVertex& MHLineEdge::getSourceVertex() const {
    return m_sourceVertex;
}

const MHVertex& MHLineEdge::getTargetVertex() const {
    return m_targetVertex;
}

void MHLineEdge::setSourceVertex(const MHVertex& vertex) {
    m_sourceVertex = vertex;
}

void MHLineEdge::setTargetVertex(const MHVertex& vertex) {
    m_targetVertex = vertex;
}

}  // namespace MHGeometry
