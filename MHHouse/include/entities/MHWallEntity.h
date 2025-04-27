#pragma once

/**
 * @file MHWallEntity.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include <unordered_map>
#include <vector>

#include "MHArcEdge.h"
#include "MHHouseEntity.h"
#include "MHLineEdge.h"
#include "MHPlaneFace.h"

namespace MHHouse {

class MHHoleEntity;
class MHWallManager;

enum class MHWallPositionType {
    LEFT,
    MID,
    RIGHT,
};

class MHWallEntity final : public MHHouseEntity {
public:
    friend class MHWallManager;
    explicit MHWallEntity(vtkSmartPointer<MHCore::MHRenderer> renderer = nullptr);
    ~MHWallEntity() = default;
    MHWallEntity(const MHWallEntity& entity) = delete;
    MHWallEntity(MHWallEntity&& entity) = delete;
    MHWallEntity& operator=(const MHWallEntity& entity) = delete;
    MHWallEntity& operator=(MHWallEntity&& entity) = delete;

public:
    virtual void destroy() override;

public:
    void updateWall(const MHGeometry::MHLineEdge& positionEdge, double height, double width, MHWallPositionType positionType);
    void updateWall(const MHGeometry::MHArcEdge& positionEdge, double height, double width, MHWallPositionType positionType);
    void generateWall2D();
    void generateWall3D();
    std::vector<std::shared_ptr<MHGeometry::MHEdge>> getEdges() const;
    std::shared_ptr<MHGeometry::MHPlaneFace> getBaseFace() const;
    std::shared_ptr<MHGeometry::MHEdge> getMidEdge() const;
    void addHole(std::shared_ptr<MHHoleEntity> holeEntity);
    void removeHole(std::shared_ptr<MHHoleEntity> holeEntity);
    void removeHole(const std::string& holeId);

private:
    void createDefaultTexture();

private:
    std::shared_ptr<MHGeometry::MHEdge> m_positionEdge = nullptr;
    double m_height = 2800.0;
    double m_width = 240.0;
    MHWallPositionType m_positionType = MHWallPositionType::LEFT;
    std::vector<std::shared_ptr<MHGeometry::MHEdge>> m_edges;
    std::shared_ptr<MHGeometry::MHPlaneFace> m_baseFace = nullptr;
    std::shared_ptr<MHHouseEntity> m_wall2D = nullptr;
    vtkSmartPointer<vtkTexture> m_wall2DTexture = nullptr;
    std::unordered_map<std::string, std::shared_ptr<MHHoleEntity>> m_holeEntities;
};

}  // namespace MHHouse
