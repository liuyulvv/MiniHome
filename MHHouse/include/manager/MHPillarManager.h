#pragma once

/**
 * @file MHPillarManager.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-13
 */

#include <unordered_map>

#include "MHPillarEntity.h"

namespace MHHouse {

class MHPillarManager {
public:
    static MHPillarManager& getInstance();
    MHPillarManager(const MHPillarManager& manager) = delete;
    MHPillarManager(MHPillarManager&& manager) = delete;
    MHPillarManager& operator=(const MHPillarManager& manager) = delete;
    MHPillarManager& operator=(MHPillarManager&& manager) = delete;

private:
    MHPillarManager() = default;
    ~MHPillarManager() = default;

public:
    void addPillar(std::shared_ptr<MHPillarEntity> pillar);
    void removePillar(const std::string& pillarId);
    void removePillar(std::shared_ptr<MHPillarEntity> pillar);
    std::vector<std::shared_ptr<MHPillarEntity>> getPillars() const;

private:
    std::unordered_map<std::string, std::shared_ptr<MHPillarEntity>> m_pillars{};
};

}  // namespace MHHouse
