/**
 * @file MHLineEntity.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHLineEntity.h"

#include <vtkLineSource.h>
#include <vtkPolyDataMapper.h>

namespace MHCore {

MHLineEntity::MHLineEntity(vtkSmartPointer<MHRenderer> renderer) : MHEntity(renderer) {
}

void MHLineEntity::setSourceVertex(const MHGeometry::MHVertex& vertex) {
    m_lineEdge.setSourceVertex(vertex);
}

void MHLineEntity::setTargetVertex(const MHGeometry::MHVertex& vertex) {
    m_lineEdge.setTargetVertex(vertex);
}

void MHLineEntity::update() {
    auto lineSource = vtkSmartPointer<vtkLineSource>::New();
    auto sourceVertex = m_lineEdge.getSourceVertex();
    auto targetVertex = m_lineEdge.getTargetVertex();
    double start[3] = {sourceVertex.x, sourceVertex.y, sourceVertex.z};
    double end[3] = {targetVertex.x, targetVertex.y, targetVertex.z};
    lineSource->SetPoint1(start);
    lineSource->SetPoint2(end);
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(lineSource->GetOutputPort());
    m_actor->SetMapper(mapper);
}

}  // namespace MHCore
