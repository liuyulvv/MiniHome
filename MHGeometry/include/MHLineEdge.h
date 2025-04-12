#pragma once

/**
 * @file MHLineEdge.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include <vector>

#include "MHEdge.h"

namespace MHGeometry {

class MH_GEOMETRY_API MHLineEdge : public MHEdge {
public:
    MHLineEdge() = default;
    MHLineEdge(const MHVertex& source, const MHVertex& target);
    ~MHLineEdge() = default;
    MHLineEdge(const MHLineEdge& edge) = default;
    MHLineEdge(MHLineEdge&& edge) = default;
    MHLineEdge& operator=(const MHLineEdge& edge) = default;
    MHLineEdge& operator=(MHLineEdge&& edge) = default;

public:
    virtual MHTopoType getType() const override;
    virtual MHEdgeType getEdgeType() const override;
    virtual MHTopoBase* clone() const override;
    virtual void applyTransform(vtkSmartPointer<vtkTransform> transform) override;
    virtual const MHVertex& getSourceVertex() const override;
    virtual const MHVertex& getTargetVertex() const override;

public:
    void setSourceVertex(const MHVertex& vertex);
    void setTargetVertex(const MHVertex& vertex);

private:
    MHVertex m_sourceVertex;
    MHVertex m_targetVertex;
};

}  // namespace MHGeometry
