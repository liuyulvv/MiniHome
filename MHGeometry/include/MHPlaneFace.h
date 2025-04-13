#pragma once

/**
 * @file MHPlaneFace.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include <vector>

#include "MHFace.h"

namespace MHGeometry {

class MH_GEOMETRY_API MHPlaneFace : public MHFace {
public:
    MHPlaneFace() = default;
    ~MHPlaneFace() = default;
    MHPlaneFace(const MHPlaneFace& face) = default;
    MHPlaneFace(MHPlaneFace&& face) = default;
    MHPlaneFace& operator=(const MHPlaneFace& face) = default;
    MHPlaneFace& operator=(MHPlaneFace&& face) = default;

public:
    virtual MHFaceType getFaceType() const override;
    virtual std::unique_ptr<MHTopoBase> clone() const override;
    virtual void applyTransform(vtkSmartPointer<vtkTransform> transform) override;

public:
    void addWire(const MHWire& wire);
    std::vector<MHWire> getWires() const;

private:
    std::vector<MHWire> m_wires{};
};

}  // namespace MHGeometry
