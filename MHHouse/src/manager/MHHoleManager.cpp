/**
 * @file MHHoleManager.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-21
 */

#include "MHHoleManager.h"

namespace MHHouse {

MHHoleManager& MHHoleManager::getInstance() {
    static MHHoleManager instance;
    return instance;
}

void MHHoleManager::addHole(std::shared_ptr<MHHoleEntity> hole) {
    if (hole) {
        m_holes[hole->getId()] = hole;
    }
}

void MHHoleManager::removeHole(const std::string& holeId) {
    auto it = m_holes.find(holeId);
    if (it != m_holes.end()) {
        m_holes.erase(it);
    }
}

void MHHoleManager::removeHole(std::shared_ptr<MHHoleEntity> hole) {
    if (hole) {
        auto it = m_holes.find(hole->getId());
        if (it != m_holes.end()) {
            m_holes.erase(it);
        }
    }
}

std::vector<std::shared_ptr<MHHoleEntity>> MHHoleManager::getHoles() const {
    std::vector<std::shared_ptr<MHHoleEntity>> holes;
    for (const auto& pair : m_holes) {
        holes.push_back(pair.second);
    }
    return holes;
}

void MHHoleManager::clear() {
    for (auto& pair : m_holes) {
        if (pair.second) {
            pair.second->destroy();
        }
    }
    m_holes.clear();
}

}  // namespace MHHouse
