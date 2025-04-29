/**
 * @file MHWallManager.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include "MHWallManager.h"

namespace MHHouse {

MHWallManager& MHWallManager::getInstance() {
    static MHWallManager instance;
    return instance;
}

void MHWallManager::addWall(std::shared_ptr<MHWallEntity> wall) {
    if (wall) {
        m_walls[wall->getId()] = wall;
    }
}

void MHWallManager::removeWall(const std::string& wallId) {
    auto it = m_walls.find(wallId);
    if (it != m_walls.end()) {
        m_walls.erase(it);
    }
}

void MHWallManager::removeWall(std::shared_ptr<MHWallEntity> wall) {
    if (wall) {
        auto it = m_walls.find(wall->getId());
        if (it != m_walls.end()) {
            m_walls.erase(it);
        }
    }
}

std::vector<std::shared_ptr<MHWallEntity>> MHWallManager::getWalls() const {
    std::vector<std::shared_ptr<MHWallEntity>> walls;
    for (const auto& pair : m_walls) {
        walls.push_back(pair.second);
    }
    return walls;
}

void MHWallManager::clear() {
    while (!m_walls.empty()) {
        auto it = m_walls.begin();
        if (it->second) {
            it->second->destroy();
        }
    }
    m_walls.clear();
}

}  // namespace MHHouse
