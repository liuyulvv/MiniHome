/**
 * @file MHEntityManager.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-18
 */

#include "MHEntityManager.h"

namespace MHCore {

MHEntityManager& MHEntityManager::getInstance() {
    static MHEntityManager instance;
    return instance;
}

MHEntityManager::MHEntityManager() {
    m_entities = {};
}

MHEntityManager::~MHEntityManager() {
    clear();
}

void MHEntityManager::clear() {
    m_entities.clear();
}

void MHEntityManager::addEntity(const std::shared_ptr<MHEntity>& entity) {
    m_entities[entity->getId()] = entity;
}

void MHEntityManager::removeEntity(const std::string& id) {
    m_entities.erase(id);
}

void MHEntityManager::removeEntity(const std::shared_ptr<MHEntity>& entity) {
    removeEntity(entity->getId());
}

std::shared_ptr<MHEntity> MHEntityManager::getEntity(const std::string& id) {
    auto iter = m_entities.find(id);
    if (iter != m_entities.end()) {
        return iter->second;
    }
    return nullptr;
}

}  // namespace MHCore
