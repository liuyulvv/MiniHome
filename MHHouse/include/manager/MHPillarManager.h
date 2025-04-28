#pragma once

/**
 * @file MHPillarManager.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-13
 */

#ifdef MH_HOUSE_EXPORTS
#define MH_HOUSE_API __declspec(dllexport)
#else
#define MH_HOUSE_API __declspec(dllimport)
#endif

#include <unordered_map>

#include "MHPillarEntity.h"

namespace MHHouse {

class MH_HOUSE_API MHPillarManager {
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
    void clear();

private:
    std::unordered_map<std::string, std::shared_ptr<MHPillarEntity>> m_pillars{};
};

}  // namespace MHHouse
