/**
 * @file MHVertex.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include "MHVertex.h"

namespace MHGeometry {

MHVertex::MHVertex(double x, double y, double z) : x(x), y(y), z(z) {}

MHTopoType MHVertex::getType() const {
    return MHTopoType::VERTEX;
}

MHTopoBase* MHVertex::clone() const {
    return new MHVertex(this->x, this->y, this->z);
}

void MHVertex::applyTransform(vtkSmartPointer<vtkTransform> transform) {
    double point[3] = {this->x, this->y, this->z};
    transform->TransformPoint(point, point);
    this->x = point[0];
    this->y = point[1];
    this->z = point[2];
}

MHVertex MHVertex::normalize() const {
    double length = std::sqrt(x * x + y * y + z * z);
    if (length <= 1e-6) {
        throw std::runtime_error("Cannot normalize a zero vector.");
    }
    return MHVertex(x / length, y / length, z / length);
}

MHVertex MHVertex::cross(const MHVertex& vertex) const {
    return MHVertex(
        y * vertex.z - z * vertex.y,
        z * vertex.x - x * vertex.z,
        x * vertex.y - y * vertex.x);
}

double MHVertex::dot(const MHVertex& vertex) const {
    return x * vertex.x + y * vertex.y + z * vertex.z;
}

double MHVertex::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

bool MHVertex::isParallel(const MHVertex& vertex) const {
    double crossProductLength = std::sqrt(
        (y * vertex.z - z * vertex.y) * (y * vertex.z - z * vertex.y) +
        (z * vertex.x - x * vertex.z) * (z * vertex.x - x * vertex.z) +
        (x * vertex.y - y * vertex.x) * (x * vertex.y - y * vertex.x));
    return crossProductLength < 1e-6;
}

}  // namespace MHGeometry
