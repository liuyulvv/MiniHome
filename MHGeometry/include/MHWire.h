#pragma once

/**
 * @file MHWire.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include <memory>
#include <vector>

#include "MHArcEdge.h"
#include "MHEdge.h"
#include "MHLineEdge.h"

namespace MHGeometry {

class MH_GEOMETRY_API MHWire : public MHTopoBase {
public:
    MHWire() = default;
    ~MHWire() = default;
    MHWire(const MHWire& wire);
    MHWire(MHWire&& wire) = default;
    MHWire& operator=(const MHWire& wire);
    MHWire& operator=(MHWire&& wire) = default;

public:
    virtual MHTopoType getType() const override;
    virtual std::shared_ptr<MHTopoBase> clone() const override;
    virtual void applyTransform(vtkSmartPointer<vtkTransform> transform) override;
    virtual void reversed() override;

public:
    void addEdge(const MHLineEdge& edge);
    void addEdge(const MHArcEdge& edge);
    void addEdge(std::shared_ptr<MHEdge> edge);
    std::vector<std::shared_ptr<MHEdge>> getEdges() const;

private:
    std::vector<std::shared_ptr<MHEdge>> m_edges;
};

}  // namespace MHGeometry
