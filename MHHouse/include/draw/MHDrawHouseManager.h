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

#include <functional>
#include <memory>

#include "MHDrawBase.h"
#include "MHDrawCylinder.h"
#include "MHDrawHole.h"
#include "MHDrawPillar.h"
#include "MHDrawWallArc.h"
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
    static double getDrawPillarHeight();
    static double getDrawPillarLength();
    static double getDrawPillarWidth();
    static double getDrawCylinderHeight();
    static double getDrawCylinderRadius();
    static double getDrawHoleHeight();
    static double getDrawHoleLength();
    static double getDrawHoleWidth();

public:
    void setShowRightPanelCallback(std::function<void(bool, MHDrawType)> callback);

private:
    MHDrawType m_drawType = MHDrawType::NONE;
    std::shared_ptr<MHDrawWallLine> m_drawWallLine = nullptr;
    std::shared_ptr<MHDrawWallRectangle> m_drawWallRectangle = nullptr;
    std::shared_ptr<MHDrawWallArc> m_drawWallArc = nullptr;
    std::shared_ptr<MHDrawPillar> m_drawPillar = nullptr;
    std::shared_ptr<MHDrawCylinder> m_drawCylinder = nullptr;
    std::shared_ptr<MHDrawHole> m_drawHole = nullptr;

private:
    static MHWallPositionType m_drawWallPositionType;
    static double m_drawWallHeight;
    static double m_drawWallWidth;
    static double m_drawPillarHeight;
    static double m_drawPillarLength;
    static double m_drawPillarWidth;
    static double m_drawCylinderHeight;
    static double m_drawCylinderRadius;
    static double m_drawHoleHeight;
    static double m_drawHoleLength;
    static double m_drawHoleWidth;

private:
    std::function<void(bool, MHDrawType)> m_showRightPanelCallback = nullptr;
};

}  // namespace MHHouse
