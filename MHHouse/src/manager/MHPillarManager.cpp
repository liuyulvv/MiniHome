/**
 * @file MHPillarManager.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-13
 */

#include "MHPillarManager.h"

namespace MHHouse {

MHPillarManager& MHPillarManager::getInstance() {
    static MHPillarManager instance;
    return instance;
}

void MHPillarManager::addPillar(std::shared_ptr<MHPillarEntity> pillar) {
    if (pillar) {
        m_pillars[pillar->getId()] = pillar;
    }
}

void MHPillarManager::removePillar(const std::string& pillarId) {
    auto it = m_pillars.find(pillarId);
    if (it != m_pillars.end()) {
        m_pillars.erase(it);
    }
}

void MHPillarManager::removePillar(std::shared_ptr<MHPillarEntity> pillar) {
    if (pillar) {
        auto it = m_pillars.find(pillar->getId());
        if (it != m_pillars.end()) {
            m_pillars.erase(it);
        }
    }
}

std::vector<std::shared_ptr<MHPillarEntity>> MHPillarManager::getPillars() const {
    std::vector<std::shared_ptr<MHPillarEntity>> pillars;
    for (const auto& pair : m_pillars) {
        pillars.push_back(pair.second);
    }
    return pillars;
}

}  // namespace MHHouse
