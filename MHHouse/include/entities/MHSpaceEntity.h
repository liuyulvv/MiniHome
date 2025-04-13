#pragma once

/**
 * @file MHSpaceEntity.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

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
};

}  // namespace MHHouse
