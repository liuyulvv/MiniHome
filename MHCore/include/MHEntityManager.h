#pragma once

/**
 * @file MHEntityManager.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-18
 */

#ifdef MH_CORE_EXPORTS
#define MH_CORE_API __declspec(dllexport)
#else
#define MH_CORE_API __declspec(dllimport)
#endif

#include <unordered_map>

#include "MHEntity.h"

namespace MHCore {

class MH_CORE_API MHEntityManager {
public:
    static MHEntityManager& getInstance();
    MHEntityManager(const MHEntityManager& manager) = delete;
    MHEntityManager(MHEntityManager&& manager) = delete;
    MHEntityManager& operator=(const MHEntityManager& manager) = delete;
    MHEntityManager& operator=(MHEntityManager&& manager) = delete;

private:
    MHEntityManager();
    ~MHEntityManager();

public:
    void clear();
    void addEntity(const std::shared_ptr<MHEntity>& entity);
    void removeEntity(const std::string& id);
    void removeEntity(const std::shared_ptr<MHEntity>& entity);
    std::shared_ptr<MHEntity> getEntity(const std::string& id);

private:
    std::unordered_map<std::string, std::shared_ptr<MHEntity>> m_entities;
};

}  // namespace MHCore
