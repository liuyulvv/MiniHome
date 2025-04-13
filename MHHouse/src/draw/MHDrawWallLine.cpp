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
    m_lineEdge = std::make_unique<MHGeometry::MHLineEdge>();
    m_wallEntity = std::make_unique<MHWallEntity>();
    m_wallEntityShow = false;
    m_drawState = DrawState::FIRST;
}

void MHDrawWallLine::endDraw() {
    MHCore::MHMainVTKInteractorStyle::getInstance().removeFilter(shared_from_this());
    m_wallEntity.reset();
    m_wallEntityShow = false;
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
        m_wallEntity->show();
        MHWallManager::getInstance().addWall(m_wallEntity);
        MHCore::MHEntityManager::getInstance().addEntity(m_wallEntity);
        MHSpaceManager::getInstance().generateSpaces();
        m_wallEntity = std::make_unique<MHWallEntity>();
        m_wallEntityShow = false;
        m_lineEdge = std::make_unique<MHGeometry::MHLineEdge>();
        m_lineEdge->setSourceVertex({interactorInfo.worldX, interactorInfo.worldY, 0});
        MHCore::MHMainVTKInteractorStyle::getInstance().render();
    }
    return true;
}

bool MHDrawWallLine::onLeftButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    return true;
}

bool MHDrawWallLine::onRightButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    if (m_wallEntity) {
        m_wallEntity.reset();
    }
    m_drawState = DrawState::END;
    m_wallEntityShow = false;
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
        if (!m_wallEntityShow) {
            m_wallEntity->show();
            m_wallEntityShow = true;
        }
        MHCore::MHMainVTKInteractorStyle::getInstance().render();
    }
    return true;
}

}  // namespace MHHouse
