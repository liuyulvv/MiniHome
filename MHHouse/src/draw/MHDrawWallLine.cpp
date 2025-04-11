/**
 * @file MHDrawWallLine.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHDrawWallLine.h"

#include "MHDrawWallManager.h"
#include "MHMainVTKInteractorStyle.h"
#include "MHRendererManager.h"

namespace MHHouse {

void MHDrawWallLine::beginDraw() {
    MHCore::MHMainVTKInteractorStyle::getInstance().insertFilter(shared_from_this());
    m_lineEntity = std::make_unique<MHCore::MHLineEntity>(MHCore::MHRendererManager::getInstance().getMainRenderer());
    m_lineEntityShow = false;
    m_lineEntities.clear();
    m_drawState = DrawState::FIRST;
}

void MHDrawWallLine::endDraw() {
    MHCore::MHMainVTKInteractorStyle::getInstance().removeFilter(shared_from_this());
    m_lineEntity.reset(nullptr);
    m_lineEntityShow = false;
    m_lineEntities.clear();
    m_drawState = DrawState::END;
}

int MHDrawWallLine::getPriority() const {
    return 0;
}

bool MHDrawWallLine::onLeftButtonDown(const MHCore::MHInteractorInfo& interactorInfo) {
    if (m_drawState == DrawState::FIRST) {
        m_lineEntity->setStartPoint({interactorInfo.worldX, interactorInfo.worldY, 0});
        m_drawState = DrawState::SECOND;
    } else if (m_drawState == DrawState::SECOND) {
        m_lineEntity->setEndPoint({interactorInfo.worldX, interactorInfo.worldY, 0});
        m_lineEntity->update();
        m_lineEntities.push_back(std::move(m_lineEntity));
        m_lineEntity = std::make_unique<MHCore::MHLineEntity>(MHCore::MHRendererManager::getInstance().getMainRenderer());
        m_lineEntityShow = false;
        m_lineEntity->setStartPoint({interactorInfo.worldX, interactorInfo.worldY, 0});
        m_lineEntity->update();
        MHCore::MHMainVTKInteractorStyle::getInstance().render();
    }
    return true;
}

bool MHDrawWallLine::onLeftButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    return true;
}

bool MHDrawWallLine::onRightButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    if (m_lineEntity) {
        m_lineEntity.reset(nullptr);
    }
    m_drawState = DrawState::END;
    m_lineEntityShow = false;
    MHDrawWallManager::getInstance().endDraw();
    MHCore::MHMainVTKInteractorStyle::getInstance().render();
    return true;
}

bool MHDrawWallLine::onMouseMove(const MHCore::MHInteractorInfo& interactorInfo) {
    if (m_drawState == DrawState::END || m_drawState == DrawState::FIRST) {
        return true;
    }
    if (m_drawState == DrawState::SECOND) {
        m_lineEntity->setEndPoint({interactorInfo.worldX, interactorInfo.worldY, 0});
        m_lineEntity->update();
        if (!m_lineEntityShow) {
            m_lineEntity->show();
            m_lineEntityShow = true;
        }
        MHCore::MHMainVTKInteractorStyle::getInstance().render();
    }
    return true;
}

}  // namespace MHHouse
