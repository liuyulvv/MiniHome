#pragma once

/**
 * @file MHCylinderEntity.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-14
 */

#include <MHPlaneFace.h>

#include "MHArcEdge.h"
#include "MHHouseEntity.h"
#include "MHVertex.h"

namespace MHHouse {

class MHCylinderEntity : public MHHouseEntity {
public:
    explicit MHCylinderEntity(vtkSmartPointer<MHCore::MHRenderer> renderer = nullptr);
    ~MHCylinderEntity() = default;
    MHCylinderEntity(const MHCylinderEntity& entity) = delete;
    MHCylinderEntity(MHCylinderEntity&& entity) = delete;
    MHCylinderEntity& operator=(const MHCylinderEntity& entity) = delete;
    MHCylinderEntity& operator=(MHCylinderEntity&& entity) = delete;

public:
    virtual void destroy() override;

public:
    void updateCylinder(const MHGeometry::MHVertex& center, double radius, double height);
    void generateCylinder2D();
    void generateCylinder3D();
    std::unique_ptr<MHGeometry::MHArcEdge> getEdge();
    std::unique_ptr<MHGeometry::MHPlaneFace> getBaseFace() const;

private:
    void createDefaultTexture();

private:
    std::unique_ptr<MHGeometry::MHVertex> m_center;
    double m_radius = 325.0;
    double m_height = 2800.0;
    std::unique_ptr<MHGeometry::MHArcEdge> m_edge = nullptr;
    std::unique_ptr<MHGeometry::MHPlaneFace> m_baseFace = nullptr;
    std::shared_ptr<MHHouseEntity> m_cylinder2D = nullptr;
    vtkSmartPointer<vtkTexture> m_cylinder2DTexture = nullptr;
};

}  // namespace MHHouse
