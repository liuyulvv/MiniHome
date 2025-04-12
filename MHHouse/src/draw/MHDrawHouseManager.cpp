/**
 * @file MHDrawHouseManager.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHDrawHouseManager.h"

#include "MHMainVTKInteractorStyle.h"

namespace MHHouse {

MHWallPositionType MHDrawHouseManager::m_drawWallPositionType = MHWallPositionType::MID;
double MHDrawHouseManager::m_drawWallHeight = 2800.0;
double MHDrawHouseManager::m_drawWallWidth = 240.0;

MHDrawHouseManager::MHDrawHouseManager() {
    m_drawWallRectangle.reset(new MHDrawWallRectangle());
    m_drawWallLine.reset(new MHDrawWallLine());
}

void MHDrawHouseManager::beginDraw(MHDrawType drawType) {
    if (m_drawType == drawType) {
        return;
    }
    MHCore::MHMainVTKInteractorStyle::getInstance().switchTo(MHCore::MHInteractorType::Top2D);
    endDraw();
    m_drawType = drawType;
    switch (m_drawType) {
        case MHDrawType::WALL_RECTANGLE:
            m_drawWallRectangle->beginDraw();
            break;
        case MHDrawType::WALL_LINE:
            m_drawWallLine->beginDraw();
            break;
        default:
            m_drawType = MHDrawType::NONE;
            break;
    }
}

void MHDrawHouseManager::endDraw() {
    switch (m_drawType) {
        case MHDrawType::WALL_RECTANGLE:
            m_drawWallRectangle->endDraw();
            break;
        case MHDrawType::WALL_LINE:
            m_drawWallLine->endDraw();
            break;
        default:
            break;
    }
    m_drawType = MHDrawType::NONE;
}

MHWallPositionType MHDrawHouseManager::getDrawWallPositionType() {
    return m_drawWallPositionType;
}

double MHDrawHouseManager::getDrawWallHeight() {
    return m_drawWallHeight;
}

double MHDrawHouseManager::getDrawWallWidth() {
    return m_drawWallWidth;
}

}  // namespace MHHouse
