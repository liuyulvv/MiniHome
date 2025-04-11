#pragma once

/**
 * @file MHArcEdge.hpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include <algorithm>
#include <cmath>
#include <numbers>

#include "MHEdge.hpp"
#include "MHVertex.hpp"

namespace MHGeometry {

class MH_GEOMETRY_API MHArcEdge : public MHEdge {
public:
    MHArcEdge() = default;

    MHArcEdge(const MHVertex& center, double radius) {
        m_center = center;
        m_radius = radius;
        computeVertex();
    }

    MHArcEdge(const MHVertex& center, double radius, double sourceAngle, double targetAngle, MHTopoOrientation orientation = MHTopoOrientation::COUNTER_CLOCK_WISE) {
        m_center = center;
        m_radius = radius;
        m_sourceAngle = sourceAngle;
        m_targetAngle = targetAngle;
        m_orientation = orientation;
        computeVertex();
    }

    ~MHArcEdge() = default;
    MHArcEdge(const MHArcEdge& edge) = default;
    MHArcEdge(MHArcEdge&& edge) = default;
    MHArcEdge& operator=(const MHArcEdge& edge) = default;
    MHArcEdge& operator=(MHArcEdge&& edge) = default;

public:
    virtual MHTopoType getType() const override {
        return MHTopoType::EDGE;
    }

    virtual MHEdgeType getEdgeType() const override {
        return MHEdgeType::ARC_EDGE;
    }

    virtual MHTopoBase* clone() const override {
        return new MHArcEdge(*this);
    }

    virtual const MHVertex& getSourceVertex() const override {
        return m_sourceVertex;
    }

    virtual const MHVertex& getTargetVertex() const override {
        return m_targetVertex;
    }

    const MHVertex& getCenter() const {
        return m_center;
    }

    double getRadius() const {
        return m_radius;
    }

    double getSourceAngle() const {
        return m_sourceAngle;
    }

    double getTargetAngle() const {
        return m_targetAngle;
    }

    MHTopoOrientation getOrientation() const {
        return m_orientation;
    }

private:
    void computeVertex() {
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
        m_sourceVertex = MHVertex(m_center.x + m_radius * std::cos(sourceRadian), m_center.y + m_radius * std::sin(sourceRadian), m_center.z);
        m_targetVertex = MHVertex(m_center.x + m_radius * std::cos(targetRadian), m_center.y + m_radius * std::sin(targetRadian), m_center.z);
    }

private:
    MHVertex m_center{0.0, 0.0, 0.0};
    double m_radius = 1.0;
    double m_sourceAngle{0.0};
    double m_targetAngle{360.0};
    MHVertex m_sourceVertex{1.0, 0.0, 0.0};
    MHVertex m_targetVertex{1.0, 0.0, 0.0};
    MHTopoOrientation m_orientation{MHTopoOrientation::COUNTER_CLOCK_WISE};
};

}  // namespace MHGeometry
