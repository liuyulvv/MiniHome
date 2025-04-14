#pragma once

/**
 * @file MHCylinderManager.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-14
 */

#include <unordered_map>

#include "MHCylinderEntity.h"

namespace MHHouse {

class MHCylinderManager {
public:
    static MHCylinderManager& getInstance();
    MHCylinderManager(const MHCylinderManager& manager) = delete;
    MHCylinderManager(MHCylinderManager&& manager) = delete;
    MHCylinderManager& operator=(const MHCylinderManager& manager) = delete;
    MHCylinderManager& operator=(MHCylinderManager&& manager) = delete;

private:
    MHCylinderManager() = default;
    ~MHCylinderManager() = default;

public:
    void addCylinder(std::shared_ptr<MHCylinderEntity> pillar);
    void removeCylinder(const std::string& pillarId);
    void removeCylinder(std::shared_ptr<MHCylinderEntity> pillar);
    std::vector<std::shared_ptr<MHCylinderEntity>> getCylinders() const;

private:
    std::unordered_map<std::string, std::shared_ptr<MHCylinderEntity>> m_cylinders{};
};

}  // namespace MHHouse
