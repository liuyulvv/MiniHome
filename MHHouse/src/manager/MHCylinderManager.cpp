/**
 * @file MHCylinderManager.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-14
 */

#include "MHCylinderManager.h"

namespace MHHouse {

MHCylinderManager& MHCylinderManager::getInstance() {
    static MHCylinderManager instance;
    return instance;
}

void MHCylinderManager::addCylinder(std::shared_ptr<MHCylinderEntity> cylinder) {
    if (cylinder) {
        m_cylinders[cylinder->getId()] = cylinder;
    }
}

void MHCylinderManager::removeCylinder(const std::string& cylinderId) {
    auto it = m_cylinders.find(cylinderId);
    if (it != m_cylinders.end()) {
        m_cylinders.erase(it);
    }
}

void MHCylinderManager::removeCylinder(std::shared_ptr<MHCylinderEntity> cylinder) {
    if (cylinder) {
        auto it = m_cylinders.find(cylinder->getId());
        if (it != m_cylinders.end()) {
            m_cylinders.erase(it);
        }
    }
}

std::vector<std::shared_ptr<MHCylinderEntity>> MHCylinderManager::getCylinders() const {
    std::vector<std::shared_ptr<MHCylinderEntity>> cylinders;
    for (const auto& pair : m_cylinders) {
        cylinders.push_back(pair.second);
    }
    return cylinders;
}

void MHCylinderManager::clear() {
    for (const auto& pair : m_cylinders) {
        if (pair.second) {
            pair.second->destroy();
        }
    }
    m_cylinders.clear();
}

}  // namespace MHHouse
