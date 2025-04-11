#pragma once

/**
 * @file MHDrawManager.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#ifdef MH_HOUSE_EXPORTS
#define MH_HOUSE_API __declspec(dllexport)
#else
#define MH_HOUSE_API __declspec(dllimport)
#endif

#include <memory>

#include "MHDrawBase.h"
#include "MHDrawWallLine.h"
#include "MHDrawWallRectangle.h"

namespace MHHouse {

class MH_HOUSE_API MHDrawManager final {
public:
    static MHDrawManager& getInstance() {
        static MHDrawManager instance;
        return instance;
    }

    MHDrawManager(const MHDrawManager& manager) = delete;
    MHDrawManager(MHDrawManager&& manager) = delete;
    MHDrawManager& operator=(const MHDrawManager& manager) = delete;
    MHDrawManager& operator=(MHDrawManager&& manager) = delete;

private:
    MHDrawManager();
    ~MHDrawManager() = default;

private:
    std::unique_ptr<MHDrawWallRectangle> m_drawWallRectangle = nullptr;
    std::unique_ptr<MHDrawWallLine> m_drawWallLine = nullptr;
};

}  // namespace MHHouse
