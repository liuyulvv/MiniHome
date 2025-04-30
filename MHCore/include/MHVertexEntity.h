#pragma once

/**
 * @file MHVertexEntity.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-29
 */

#include <vtkDiskSource.h>

#include "MHEntity.h"
#include "MHVertex.h"

namespace MHCore {

class MH_CORE_API MHVertexEntity : public MHEntity {
public:
    MHVertexEntity();
    ~MHVertexEntity() = default;
    MHVertexEntity(const MHVertexEntity& entity) = delete;
    MHVertexEntity(MHVertexEntity&& entity) = delete;
    MHVertexEntity& operator=(const MHVertexEntity& entity) = delete;
    MHVertexEntity& operator=(MHVertexEntity&& entity) = delete;

public:
    void setVertex(const MHGeometry::MHVertex& vertex);
    void setRadius(double radius);
    void updateScale();

private:
    MHGeometry::MHVertex m_vertex;
    vtkSmartPointer<vtkDiskSource> m_source;
    double m_radius = 10.0;
};

}  // namespace MHCore
