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

MHTopoBase* MHPlaneFace::clone() const {
    return new MHPlaneFace(*this);
}

void MHPlaneFace::applyTransform(vtkSmartPointer<vtkTransform> transform) {
    for (auto& wire : m_wires) {
        wire.applyTransform(transform);
    }
}

void MHPlaneFace::addWire(const MHWire& wire) {
    m_wires.push_back(wire);
}

std::vector<MHWire> MHPlaneFace::getWires() const {
    return m_wires;
}

}  // namespace MHGeometry
