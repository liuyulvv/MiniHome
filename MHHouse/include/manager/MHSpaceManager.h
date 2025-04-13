#pragma once

/**
 * @file MHSpaceManager.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include <unordered_map>

#include "MHSpaceEntity.h"

namespace MHHouse {

class MHSpaceManager {
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

private:
    std::unordered_map<std::string, std::shared_ptr<MHSpaceEntity>> m_spaces{};
};

}  // namespace MHHouse
