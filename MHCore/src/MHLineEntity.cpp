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

void MHLineEntity::update(const MHGeometry::MHVertex& sourceVertex, const MHGeometry::MHVertex& targetVertex) {
    auto lineSource = vtkSmartPointer<vtkLineSource>::New();
    double start[3] = {sourceVertex.x, sourceVertex.y, sourceVertex.z};
    double end[3] = {targetVertex.x, targetVertex.y, targetVertex.z};
    lineSource->SetPoint1(start);
    lineSource->SetPoint2(end);
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(lineSource->GetOutputPort());
    m_actor->SetMapper(mapper);
}

void MHLineEntity::update(const MHGeometry::MHVertex& sourceVertex, const MHGeometry::MHVertex& targetVertex, double length) {
    auto lineSource = vtkSmartPointer<vtkLineSource>::New();
    auto midVertex = (sourceVertex + targetVertex) / 2.0;
    auto direction = (targetVertex - sourceVertex).normalize();
    auto source = midVertex - direction * (length / 2.0);
    auto target = midVertex + direction * (length / 2.0);
    double start[3] = {source.x, source.y, source.z};
    double end[3] = {target.x, target.y, target.z};
    lineSource->SetPoint1(start);
    lineSource->SetPoint2(end);
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(lineSource->GetOutputPort());
    m_actor->SetMapper(mapper);
}

}  // namespace MHCore
