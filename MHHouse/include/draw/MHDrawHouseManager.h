#pragma once

/**
 * @file MHDrawHouseManager.h
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
#include "MHWallEntity.h"

namespace MHHouse {

class MH_HOUSE_API MHDrawHouseManager final {
public:
    static MHDrawHouseManager& getInstance() {
        static MHDrawHouseManager instance;
        return instance;
    }

    MHDrawHouseManager(const MHDrawHouseManager& manager) = delete;
    MHDrawHouseManager(MHDrawHouseManager&& manager) = delete;
    MHDrawHouseManager& operator=(const MHDrawHouseManager& manager) = delete;
    MHDrawHouseManager& operator=(MHDrawHouseManager&& manager) = delete;

private:
    MHDrawHouseManager();
    ~MHDrawHouseManager() = default;

public:
    void beginDraw(MHDrawType drawType);
    void endDraw();
    static MHWallPositionType getDrawWallPositionType();
    static double getDrawWallHeight();
    static double getDrawWallWidth();

private:
    MHDrawType m_drawType = MHDrawType::NONE;
    std::shared_ptr<MHDrawWallLine> m_drawWallLine = nullptr;
    std::shared_ptr<MHDrawWallRectangle> m_drawWallRectangle = nullptr;

private:
    static MHWallPositionType m_drawWallPositionType;
    static double m_drawWallHeight;
    static double m_drawWallWidth;
};

}  // namespace MHHouse
