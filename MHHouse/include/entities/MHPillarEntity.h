#pragma once

/**
 * @file MHPillarEntity.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-13
 */

#include "MHHouseEntity.h"
#include "MHLineEdge.h"
#include "MHPlaneFace.h"
#include "MHVertex.h"

namespace MHHouse {

class MHPillarEntity : public MHHouseEntity {
public:
    explicit MHPillarEntity(vtkSmartPointer<MHCore::MHRenderer> renderer = nullptr);
    ~MHPillarEntity() = default;
    MHPillarEntity(const MHPillarEntity& entity) = delete;
    MHPillarEntity(MHPillarEntity&& entity) = delete;
    MHPillarEntity& operator=(const MHPillarEntity& entity) = delete;
    MHPillarEntity& operator=(MHPillarEntity&& entity) = delete;

public:
    void updatePillar(const MHGeometry::MHLineEdge& midEdge, double height, double length, double width);
    void generatePillar2D();
    void generatePillar3D();
    std::vector<std::unique_ptr<MHGeometry::MHEdge>> getEdges();
    std::unique_ptr<MHGeometry::MHPlaneFace> getBaseFace() const;

private:
    void createDefaultTexture();

private:
    std::unique_ptr<MHGeometry::MHLineEdge> m_midEdge = nullptr;
    double m_height = 2800;
    double m_length = 650.0;
    double m_width = 650.0;
    std::vector<std::unique_ptr<MHGeometry::MHLineEdge>> m_edges;
    std::unique_ptr<MHGeometry::MHPlaneFace> m_baseFace = nullptr;
    std::shared_ptr<MHHouseEntity> m_pillar2D = nullptr;
    vtkSmartPointer<vtkTexture> m_pillar2DTexture = nullptr;
};

}  // namespace MHHouse
