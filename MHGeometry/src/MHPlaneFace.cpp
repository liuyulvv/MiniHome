/**
 * @file MHPlaneFace.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include "MHPlaneFace.h"

namespace MHGeometry {

MHFaceType MHPlaneFace::getFaceType() const {
    return MHFaceType::PLANE_FACE;
}

std::unique_ptr<MHTopoBase> MHPlaneFace::clone() const {
    return std::make_unique<MHPlaneFace>(*this);
}

void MHPlaneFace::applyTransform(vtkSmartPointer<vtkTransform> transform) {
    for (auto& wire : m_wires) {
        wire.applyTransform(transform);
    }
}

void MHPlaneFace::reversed() {
    for (auto& wire : m_wires) {
        wire.reversed();
    }
}

void MHPlaneFace::addWire(const MHWire& wire) {
    m_wires.push_back(wire);
}

std::vector<MHWire> MHPlaneFace::getWires() const {
    return m_wires;
}

}  // namespace MHGeometry
