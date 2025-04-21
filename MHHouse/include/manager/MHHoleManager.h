#pragma once

/**
 * @file MHHoleManager.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-21
 */

#include <map>
#include <memory>

#include "MHHoleEntity.h"

namespace MHHouse {

class MHHoleManager {
public:
    static MHHoleManager& getInstance();
    MHHoleManager(const MHHoleManager& manager) = delete;
    MHHoleManager(MHHoleManager&& manager) = delete;
    MHHoleManager& operator=(const MHHoleManager& manager) = delete;
    MHHoleManager& operator=(MHHoleManager&& manager) = delete;

public:
    void addHole(std::shared_ptr<MHHoleEntity> hole);
    void removeHole(const std::string& holeId);
    void removeHole(std::shared_ptr<MHHoleEntity> hole);
    std::vector<std::shared_ptr<MHHoleEntity>> getHoles() const;

private:
    MHHoleManager() = default;
    ~MHHoleManager() = default;

private:
    std::unordered_map<std::string, std::shared_ptr<MHHoleEntity>> m_holes{};
};

}  // namespace MHHouse
