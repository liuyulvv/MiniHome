/**
 * @file MHArcEdge.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include "MHArcEdge.h"

namespace MHGeometry {

MHArcEdge::MHArcEdge(const MHVertex& center, double radius) {
    m_center = center;
    m_radius = radius;
    computeVertex();
}

MHArcEdge::MHArcEdge(const MHVertex& center, const MHVertex& normal, double radius) {
    m_center = center;
    m_normal = normal;
    m_radius = radius;
    computeVertex();
}

MHArcEdge::MHArcEdge(const MHVertex& center, const MHVertex& normal, double radius, double sourceAngle, double targetAngle, MHTopoOrientation orientation) {
    m_center = center;
    m_radius = radius;
    m_sourceAngle = sourceAngle;
    m_targetAngle = targetAngle;
    m_orientation = orientation;
    computeVertex();
}

MHEdgeType MHArcEdge::getEdgeType() const {
    return MHEdgeType::ARC_EDGE;
}

MHTopoBase* MHArcEdge::clone() const {
    return new MHArcEdge(*this);
}

void MHArcEdge::applyTransform(vtkSmartPointer<vtkTransform> transform) {
    m_center.applyTransform(transform);
    m_normal.applyTransform(transform);
    computeVertex();
}

const MHVertex& MHArcEdge::getSourceVertex() const {
    return m_sourceVertex;
}

const MHVertex& MHArcEdge::getTargetVertex() const {
    return m_targetVertex;
}

const MHVertex& MHArcEdge::getCenter() const {
    return m_center;
}

const MHVertex& MHArcEdge::getNormal() const {
    return m_normal;
}

double MHArcEdge::getRadius() const {
    return m_radius;
}

double MHArcEdge::getSourceAngle() const {
    return m_sourceAngle;
}

double MHArcEdge::getTargetAngle() const {
    return m_targetAngle;
}

MHTopoOrientation MHArcEdge::getOrientation() const {
    return m_orientation;
}

void MHArcEdge::computeVertex() {
    while (m_sourceAngle < 0) {
        m_sourceAngle += 360;
    }
    while (m_sourceAngle > 360) {
        m_sourceAngle -= 360;
    }
    while (m_targetAngle < 0) {
        m_targetAngle += 360;
    }
    while (m_targetAngle > 360) {
        m_targetAngle -= 360;
    }
    if (abs(m_sourceAngle - m_targetAngle) < 1e-6) {
        m_sourceAngle = 0;
        m_targetAngle = 360;
    }
    double sourceRadian = m_sourceAngle / 180 * std::numbers::pi;
    double targetRadian = m_targetAngle / 180 * std::numbers::pi;
    MHVertex sourceVertexLocal(m_radius * std::cos(sourceRadian), m_radius * std::sin(sourceRadian), 0.0);
    MHVertex targetVertexLocal(m_radius * std::cos(targetRadian), m_radius * std::sin(targetRadian), 0.0);
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->Identity();
    MHVertex zAxis(0.0, 0.0, 1.0);
    MHVertex rotationAxis = zAxis.cross(m_normal).normalize();
    double rotationRadian = std::acos(zAxis.dot(m_normal));
    if (rotationAxis.length() > 1e-6) {
        transform->RotateWXYZ(rotationRadian * 180.0 / std::numbers::pi, rotationAxis.x, rotationAxis.y, rotationAxis.z);
    }
    double sourcePoint[3] = {sourceVertexLocal.x, sourceVertexLocal.y, sourceVertexLocal.z};
    double targetPoint[3] = {targetVertexLocal.x, targetVertexLocal.y, targetVertexLocal.z};
    transform->TransformPoint(sourcePoint, sourcePoint);
    transform->TransformPoint(targetPoint, targetPoint);
    m_sourceVertex = MHVertex(m_center.x + sourcePoint[0], m_center.y + sourcePoint[1], m_center.z + sourcePoint[2]);
    m_targetVertex = MHVertex(m_center.x + targetPoint[0], m_center.y + targetPoint[1], m_center.z + targetPoint[2]);
}

}  // namespace MHGeometry
