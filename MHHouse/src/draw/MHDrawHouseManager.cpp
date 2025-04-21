/**
 * @file MHDrawHouseManager.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHDrawHouseManager.h"

#include "MHMainVTKInteractorStyle.h"

namespace MHHouse {

MHWallPositionType MHDrawHouseManager::m_drawWallPositionType = MHWallPositionType::RIGHT;
double MHDrawHouseManager::m_drawWallHeight = 2800.0;
double MHDrawHouseManager::m_drawWallWidth = 240.0;
double MHDrawHouseManager::m_drawPillarHeight = 2800.0;
double MHDrawHouseManager::m_drawPillarLength = 650.0;
double MHDrawHouseManager::m_drawPillarWidth = 650.0;
double MHDrawHouseManager::m_drawCylinderHeight = 2800.0;
double MHDrawHouseManager::m_drawCylinderRadius = 325.0;
double MHDrawHouseManager::m_drawHoleHeight = 2000.0;
double MHDrawHouseManager::m_drawHoleLength = 1200.0;
double MHDrawHouseManager::m_drawHoleWidth = 240.0;

MHDrawHouseManager::MHDrawHouseManager() {
    m_drawWallRectangle.reset(new MHDrawWallRectangle());
    m_drawWallLine.reset(new MHDrawWallLine());
    m_drawWallArc.reset(new MHDrawWallArc());
    m_drawPillar.reset(new MHDrawPillar());
    m_drawCylinder.reset(new MHDrawCylinder());
    m_drawHole.reset(new MHDrawHole());
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
        case MHDrawType::WALL_ARC:
            m_drawWallArc->beginDraw();
            break;
        case MHDrawType::PILLAR:
            m_drawPillar->beginDraw();
            break;
        case MHDrawType::CYLINDER:
            m_drawCylinder->beginDraw();
            break;
        case MHDrawType::HOLE:
            m_drawHole->beginDraw();
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
        case MHDrawType::WALL_ARC:
            m_drawWallArc->endDraw();
            break;
        case MHDrawType::PILLAR:
            m_drawPillar->endDraw();
            break;
        case MHDrawType::CYLINDER:
            m_drawCylinder->endDraw();
            break;
        case MHDrawType::HOLE:
            m_drawHole->endDraw();
            break;
        default:
            break;
    }
    m_drawType = MHDrawType::NONE;
    MHCore::MHMainVTKInteractorStyle::getInstance().render();
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

double MHDrawHouseManager::getDrawPillarHeight() {
    return m_drawPillarHeight;
}

double MHDrawHouseManager::getDrawPillarLength() {
    return m_drawPillarLength;
}

double MHDrawHouseManager::getDrawPillarWidth() {
    return m_drawPillarWidth;
}

double MHDrawHouseManager::getDrawCylinderHeight() {
    return m_drawCylinderHeight;
}

double MHDrawHouseManager::getDrawCylinderRadius() {
    return m_drawCylinderRadius;
}

double MHDrawHouseManager::getDrawHoleHeight() {
    return m_drawHoleHeight;
}

double MHDrawHouseManager::getDrawHoleLength() {
    return m_drawHoleLength;
}

double MHDrawHouseManager::getDrawHoleWidth() {
    return m_drawHoleWidth;
}

}  // namespace MHHouse
