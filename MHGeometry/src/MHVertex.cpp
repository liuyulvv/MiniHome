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

std::unique_ptr<MHTopoBase> MHVertex::clone() const {
    return std::make_unique<MHVertex>(*this);
}

void MHVertex::applyTransform(vtkSmartPointer<vtkTransform> transform) {
    double point[3] = {this->x, this->y, this->z};
    transform->TransformPoint(point, point);
    this->x = point[0];
    this->y = point[1];
    this->z = point[2];
}

MHVertex MHVertex::operator+(const MHVertex& vertex) const {
    return MHVertex(x + vertex.x, y + vertex.y, z + vertex.z);
}

MHVertex MHVertex::operator-(const MHVertex& vertex) const {
    return MHVertex(x - vertex.x, y - vertex.y, z - vertex.z);
}

MHVertex MHVertex::operator*(double scalar) const {
    return MHVertex(x * scalar, y * scalar, z * scalar);
}

MHVertex MHVertex::operator/(double scalar) const {
    if (abs(scalar) < 1e-6) {
        throw std::runtime_error("Division by zero.");
    }
    return MHVertex(x / scalar, y / scalar, z / scalar);
}

MHVertex MHVertex::operator-() const {
    return MHVertex(-x, -y, -z);
}

MHVertex MHVertex::normalize() const {
    double length = std::sqrt(x * x + y * y + z * z);
    if (length <= 1e-6) {
        return *this;
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
