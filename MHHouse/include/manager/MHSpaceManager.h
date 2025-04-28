#pragma once

/**
 * @file MHSpaceManager.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#ifdef MH_HOUSE_EXPORTS
#define MH_HOUSE_API __declspec(dllexport)
#else
#define MH_HOUSE_API __declspec(dllimport)
#endif

#include <unordered_map>

#include "MHSpaceEntity.h"

namespace MHHouse {

class MH_HOUSE_API MHSpaceManager {
public:
    static MHSpaceManager& getInstance();
    MHSpaceManager(const MHSpaceManager& manager) = delete;
    MHSpaceManager(MHSpaceManager&& manager) = delete;
    MHSpaceManager& operator=(const MHSpaceManager& manager) = delete;
    MHSpaceManager& operator=(MHSpaceManager&& manager) = delete;

private:
    MHSpaceManager() = default;
    ~MHSpaceManager() = default;

public:
    void generateSpaces();

public:
    void addSpace(std::shared_ptr<MHSpaceEntity> space);
    void removeSpace(const std::string& spaceId);
    void removeSpace(std::shared_ptr<MHSpaceEntity> space);
    void clear();

private:
    std::unordered_map<std::string, std::shared_ptr<MHSpaceEntity>> m_spaces{};
};

}  // namespace MHHouse
