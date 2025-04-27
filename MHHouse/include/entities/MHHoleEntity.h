#pragma once

/**
 * @file MHHoleEntity.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-20
 */

#include "MHArcEdge.h"
#include "MHHouseEntity.h"
#include "MHLineEdge.h"
#include "MHPlaneFace.h"
#include "MHVertex.h"

namespace MHHouse {

class MHWallEntity;

class MHHoleEntity : public MHHouseEntity {
public:
    explicit MHHoleEntity(vtkSmartPointer<MHCore::MHRenderer> renderer = nullptr);
    ~MHHoleEntity() = default;
    MHHoleEntity(const MHHoleEntity& entity) = delete;
    MHHoleEntity(MHHoleEntity&& entity) = delete;
    MHHoleEntity& operator=(const MHHoleEntity& entity) = delete;
    MHHoleEntity& operator=(MHHoleEntity&& entity) = delete;

public:
    virtual void destroy() override;
    virtual void onEnter(const MHCore::MHEntityInteractorInfo& info) override;
    virtual void onLeave(const MHCore::MHEntityInteractorInfo& info) override;
    virtual void onSelected(const MHCore::MHEntityInteractorInfo& info) override;

public:
    void updateHole(std::shared_ptr<MHWallEntity> wallEntity, const MHGeometry::MHVertex& midVertex, std::shared_ptr<MHGeometry::MHEdge> midEdge, double height, double length, double width);
    void generateHole2D();
    void generateHole3D();
    std::shared_ptr<MHGeometry::MHPlaneFace> getBaseFace() const;
    std::shared_ptr<MHHouseEntity> getHole2D() const;
    TopoDS_Shape getSolidShape() const;

private:
    void createDefaultTexture();

private:
    std::shared_ptr<MHGeometry::MHVertex> m_midVertex = nullptr;
    std::shared_ptr<MHGeometry::MHEdge> m_midEdge = nullptr;
    double m_height = 2000;
    double m_length = 1200.0;
    double m_width = 240.0;
    std::shared_ptr<MHGeometry::MHPlaneFace> m_baseFace = nullptr;
    std::shared_ptr<MHHouseEntity> m_hole2D = nullptr;
    vtkSmartPointer<vtkTexture> m_hole2DTexture = nullptr;
    std::shared_ptr<MHWallEntity> m_wallEntity = nullptr;
    TopoDS_Shape m_solidShape;
};

}  // namespace MHHouse
