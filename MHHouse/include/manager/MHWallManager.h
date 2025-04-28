#pragma once

/**
 * @file MHWallManager.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#ifdef MH_HOUSE_EXPORTS
#define MH_HOUSE_API __declspec(dllexport)
#else
#define MH_HOUSE_API __declspec(dllimport)
#endif

#include <unordered_map>

#include "MHWallEntity.h"

namespace MHHouse {

class MH_HOUSE_API MHWallManager {
public:
    static MHWallManager& getInstance();
    MHWallManager(const MHWallManager& manager) = delete;
    MHWallManager(MHWallManager&& manager) = delete;
    MHWallManager& operator=(const MHWallManager& manager) = delete;
    MHWallManager& operator=(MHWallManager&& manager) = delete;

private:
    MHWallManager() = default;
    ~MHWallManager() = default;

public:
    void addWall(std::shared_ptr<MHWallEntity> wall);
    void removeWall(const std::string& wallId);
    void removeWall(std::shared_ptr<MHWallEntity> wall);
    std::vector<std::shared_ptr<MHWallEntity>> getWalls() const;
    void clear();

private:
    std::unordered_map<std::string, std::shared_ptr<MHWallEntity>> m_walls{};
};

}  // namespace MHHouse
