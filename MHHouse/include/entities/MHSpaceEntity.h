#pragma once

/**
 * @file MHSpaceEntity.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include <memory>

#include "MHHouseEntity.h"

namespace MHHouse {

class MHSpaceEntity : public MHHouseEntity {
public:
    MHSpaceEntity(vtkSmartPointer<MHCore::MHRenderer> renderer = nullptr);
    ~MHSpaceEntity() = default;
    MHSpaceEntity(const MHSpaceEntity& entity) = delete;
    MHSpaceEntity(MHSpaceEntity&& entity) = delete;
    MHSpaceEntity& operator=(const MHSpaceEntity& entity) = delete;
    MHSpaceEntity& operator=(MHSpaceEntity&& entity) = delete;

public:
    virtual void onDelete() override;

public:
    void updateSpace(const TopoDS_Shape& topo);
    void generateSpace2D();
    void generateSpace3D();

private:
    void createDefaultTexture();

private:
    std::shared_ptr<MHHouseEntity> m_space2DEntity = nullptr;
};

}  // namespace MHHouse
