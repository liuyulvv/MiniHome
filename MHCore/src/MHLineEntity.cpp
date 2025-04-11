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

void MHLineEntity::setStartPoint(const MHGeometry::MHPoint& point) {
    m_startPoint = point;
}

void MHLineEntity::setEndPoint(const MHGeometry::MHPoint& point) {
    m_endPoint = point;
}

void MHLineEntity::update() {
    auto lineSource = vtkSmartPointer<vtkLineSource>::New();
    double start[3] = {m_startPoint.x, m_startPoint.y, m_startPoint.z};
    double end[3] = {m_endPoint.x, m_endPoint.y, m_endPoint.z};
    lineSource->SetPoint1(start);
    lineSource->SetPoint2(end);
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(lineSource->GetOutputPort());
    m_actor->SetMapper(mapper);
}

}  // namespace MHCore
