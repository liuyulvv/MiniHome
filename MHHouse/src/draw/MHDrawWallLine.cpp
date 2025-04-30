/**
 * @file MHDrawWallLine.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHDrawWallLine.h"

#include "MHDrawHouseManager.h"
#include "MHEntityManager.h"
#include "MHMainVTKInteractorStyle.h"
#include "MHSpaceManager.h"
#include "MHWallManager.h"

namespace MHHouse {

void MHDrawWallLine::beginDraw() {
    MHCore::MHMainVTKInteractorStyle::getInstance().insertFilter(shared_from_this());
    m_lineEdge = std::make_shared<MHGeometry::MHLineEdge>();
    m_wallEntity = std::make_shared<MHWallEntity>();
    m_drawState = DrawState::FIRST;
}

void MHDrawWallLine::endDraw() {
    MHCore::MHMainVTKInteractorStyle::getInstance().removeFilter(shared_from_this());
    m_lineEdge = nullptr;
    if (m_wallEntity) {
        m_wallEntity->destroy();
    }
    m_wallEntity = nullptr;
    m_drawState = DrawState::END;
}

int MHDrawWallLine::getPriority() const {
    return 0;
}

bool MHDrawWallLine::onLeftButtonDown(const MHCore::MHInteractorInfo& interactorInfo) {
    if (m_drawState == DrawState::FIRST) {
        m_lineEdge->setSourceVertex({interactorInfo.worldX, interactorInfo.worldY, 0});
        m_drawState = DrawState::SECOND;
    } else if (m_drawState == DrawState::SECOND) {
        m_lineEdge->setTargetVertex({interactorInfo.worldX, interactorInfo.worldY, 0});
        m_wallEntity->updateWall(*m_lineEdge, MHDrawHouseManager::getDrawWallHeight(), MHDrawHouseManager::getDrawWallWidth(), MHDrawHouseManager::getDrawWallPositionType());
        m_wallEntity->generateWall2D();
        m_wallEntity->generateWall3D();
        m_wallEntity->show(false);
        MHWallManager::getInstance().addWall(m_wallEntity);
        MHCore::MHEntityManager::getInstance().addEntity(m_wallEntity);
        MHSpaceManager::getInstance().generateSpaces();
        m_wallEntity = std::make_shared<MHWallEntity>();
        m_lineEdge->setSourceVertex({interactorInfo.worldX, interactorInfo.worldY, 0});
        MHCore::MHMainVTKInteractorStyle::getInstance().render();
    }
    return true;
}

bool MHDrawWallLine::onLeftButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    return true;
}

bool MHDrawWallLine::onRightButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    MHDrawHouseManager::getInstance().endDraw();
    MHCore::MHMainVTKInteractorStyle::getInstance().render();
    return true;
}

bool MHDrawWallLine::onMouseMove(const MHCore::MHInteractorInfo& interactorInfo) {
    if (m_drawState == DrawState::END || m_drawState == DrawState::FIRST) {
        return true;
    }
    if (m_drawState == DrawState::SECOND) {
        m_lineEdge->setTargetVertex({interactorInfo.worldX, interactorInfo.worldY, 0});
        m_wallEntity->updateWall(*m_lineEdge, MHDrawHouseManager::getDrawWallHeight(), MHDrawHouseManager::getDrawWallWidth(), MHDrawHouseManager::getDrawWallPositionType());
        m_wallEntity->generateWall2D();
        m_wallEntity->show(false);
        MHCore::MHMainVTKInteractorStyle::getInstance().render();
    }
    return true;
}

bool MHDrawWallLine::onKeyPress(const MHCore::MHInteractorInfo& interactorInfo) {
    if (interactorInfo.pressedKeys.size() == 1 && interactorInfo.pressedKeys.contains("SPACE")) {
        auto currentType = static_cast<int>(MHDrawHouseManager::getInstance().getDrawWallPositionType());
        currentType = (currentType + 1) % 3;
        MHDrawHouseManager::getInstance().setDrawWallPositionType(static_cast<MHWallPositionType>(currentType));
    }
    return true;
}

bool MHDrawWallLine::onKeyRelease(const MHCore::MHInteractorInfo& interactorInfo) {
    return true;
}

}  // namespace MHHouse
