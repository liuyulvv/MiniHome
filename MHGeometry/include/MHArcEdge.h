#pragma once

/**
 * @file MHArcEdge.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include <algorithm>
#include <cmath>
#include <numbers>

#include "MHEdge.h"
#include "MHVertex.h"
#include "MHVertexToolKit.h"

namespace MHGeometry {

class MH_GEOMETRY_API MHArcEdge : public MHEdge {
public:
    MHArcEdge() = default;
    MHArcEdge(const MHVertex& center, double radius);
    MHArcEdge(const MHVertex& center, const MHVertex& normal, double radius);
    MHArcEdge(const MHVertex& center, const MHVertex& normal, double radius, double sourceAngle, double targetAngle, MHTopoOrientation orientation = MHTopoOrientation::COUNTER_CLOCK_WISE);
    ~MHArcEdge() = default;
    MHArcEdge(const MHArcEdge& edge) = default;
    MHArcEdge(MHArcEdge&& edge) = default;
    MHArcEdge& operator=(const MHArcEdge& edge) = default;
    MHArcEdge& operator=(MHArcEdge&& edge) = default;

public:
    virtual MHEdgeType getEdgeType() const override;
    virtual std::shared_ptr<MHTopoBase> clone() const override;
    virtual void applyTransform(vtkSmartPointer<vtkTransform> transform) override;
    virtual const MHVertex& getSourceVertex() const override;
    virtual const MHVertex& getTargetVertex() const override;
    virtual void reversed() override;

public:
    const MHVertex& getCenter() const;
    const MHVertex& getNormal() const;
    double getRadius() const;
    double getSourceAngle() const;
    double getTargetAngle() const;
    MHTopoOrientation getOrientation() const;

private:
    void computeVertex();

private:
    MHVertex m_center{0.0, 0.0, 0.0};
    MHVertex m_normal{0.0, 0.0, 1.0};
    double m_radius = 1.0;
    double m_sourceAngle{0.0};
    double m_targetAngle{360.0};
    MHVertex m_sourceVertex{1.0, 0.0, 0.0};
    MHVertex m_targetVertex{1.0, 0.0, 0.0};
    MHTopoOrientation m_orientation{MHTopoOrientation::COUNTER_CLOCK_WISE};
};

}  // namespace MHGeometry
