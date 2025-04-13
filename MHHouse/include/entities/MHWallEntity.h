#pragma once

/**
 * @file MHWallEntity.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include <vector>

#include "MHHouseEntity.h"
#include "MHLineEdge.h"
#include "MHPlaneFace.h"

namespace MHHouse {

enum class MHWallPositionType {
    LEFT,
    MID,
    RIGHT,
};

class MHWallEntity final : public MHHouseEntity {
public:
    explicit MHWallEntity(vtkSmartPointer<MHCore::MHRenderer> renderer = nullptr);
    ~MHWallEntity() = default;
    MHWallEntity(const MHWallEntity& entity) = delete;
    MHWallEntity(MHWallEntity&& entity) = delete;
    MHWallEntity& operator=(const MHWallEntity& entity) = delete;
    MHWallEntity& operator=(MHWallEntity&& entity) = delete;

public:
    void updateWall(const MHGeometry::MHLineEdge& midEdge, double height, double width, MHWallPositionType positionType);
    void generateWall2D();
    void generateWall3D();
    std::vector<std::unique_ptr<MHGeometry::MHEdge>> getEdges();
    std::unique_ptr<MHGeometry::MHPlaneFace> getBaseFace() const;

private:
    void createDefaultTexture();

private:
    std::unique_ptr<MHGeometry::MHEdge> m_midEdge = nullptr;
    double m_height = 2800.0;
    double m_width = 240.0;
    MHWallPositionType m_positionType = MHWallPositionType::LEFT;
    std::vector<std::unique_ptr<MHGeometry::MHEdge>> m_edges;
    std::unique_ptr<MHGeometry::MHPlaneFace> m_baseFace = nullptr;
    std::shared_ptr<MHHouseEntity> m_wall2D = nullptr;
};

}  // namespace MHHouse
