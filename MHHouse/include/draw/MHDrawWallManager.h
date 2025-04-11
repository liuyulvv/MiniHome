#pragma once

/**
 * @file MHDrawWallManager.h
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

class MH_HOUSE_API MHDrawWallManager final {
public:
    static MHDrawWallManager& getInstance() {
        static MHDrawWallManager instance;
        return instance;
    }

    MHDrawWallManager(const MHDrawWallManager& manager) = delete;
    MHDrawWallManager(MHDrawWallManager&& manager) = delete;
    MHDrawWallManager& operator=(const MHDrawWallManager& manager) = delete;
    MHDrawWallManager& operator=(MHDrawWallManager&& manager) = delete;

private:
    MHDrawWallManager();
    ~MHDrawWallManager() = default;

public:
    void beginDraw(MHDrawType drawType);
    void endDraw();

private:
    MHDrawType m_drawType = MHDrawType::NONE;
    std::shared_ptr<MHDrawWallLine> m_drawWallLine = nullptr;
    std::shared_ptr<MHDrawWallRectangle> m_drawWallRectangle = nullptr;
};

}  // namespace MHHouse
