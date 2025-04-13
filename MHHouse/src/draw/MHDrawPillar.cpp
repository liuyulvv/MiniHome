/**
 * @file MHDrawPillar.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-13
 */

#include "MHDrawPillar.h"

#include "MHDrawHouseManager.h"
#include "MHEntityManager.h"
#include "MHMainVTKInteractorStyle.h"
#include "MHPillarManager.h"
#include "MHSpaceManager.h"

namespace MHHouse {

void MHDrawPillar::beginDraw() {
    MHCore::MHMainVTKInteractorStyle::getInstance().insertFilter(shared_from_this());
    m_midEdge = std::make_unique<MHGeometry::MHLineEdge>();
    m_pillarEntity = std::make_shared<MHPillarEntity>();
    m_drawState = DrawState::FIRST;
}

void MHDrawPillar::endDraw() {
    MHCore::MHMainVTKInteractorStyle::getInstance().removeFilter(shared_from_this());
    m_midEdge.reset(nullptr);
    m_pillarEntity.reset();
    m_drawState = DrawState::END;
}

int MHDrawPillar::getPriority() const {
    return 0;
}

bool MHDrawPillar::onLeftButtonDown(const MHCore::MHInteractorInfo& interactorInfo) {
    if (m_drawState == DrawState::END || !m_midEdge || !m_pillarEntity) {
        return true;
    }
    m_midEdge->setSourceVertex({interactorInfo.worldX - MHDrawHouseManager::getDrawPillarLength() / 2, interactorInfo.worldY, 0});
    m_midEdge->setTargetVertex({interactorInfo.worldX + MHDrawHouseManager::getDrawPillarLength() / 2, interactorInfo.worldY, 0});
    m_pillarEntity->updatePillar(*m_midEdge, MHDrawHouseManager::getDrawPillarHeight(), MHDrawHouseManager::getDrawPillarLength(), MHDrawHouseManager::getDrawPillarWidth());
    m_pillarEntity->generatePillar2D();
    m_pillarEntity->generatePillar3D();
    m_pillarEntity->show(false);
    MHPillarManager::getInstance().addPillar(m_pillarEntity);
    MHCore::MHEntityManager::getInstance().addEntity(m_pillarEntity);
    MHSpaceManager::getInstance().generateSpaces();
    m_pillarEntity = std::make_shared<MHPillarEntity>();
    MHCore::MHMainVTKInteractorStyle::getInstance().render();
    return true;
}

bool MHDrawPillar::onLeftButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    return true;
}

bool MHDrawPillar::onRightButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    m_drawState = DrawState::END;
    MHDrawHouseManager::getInstance().endDraw();
    MHCore::MHMainVTKInteractorStyle::getInstance().render();
    return true;
}

bool MHDrawPillar::onMouseMove(const MHCore::MHInteractorInfo& interactorInfo) {
    if (m_drawState == DrawState::END || !m_midEdge || !m_pillarEntity) {
        return true;
    }
    m_midEdge->setSourceVertex({interactorInfo.worldX - MHDrawHouseManager::getDrawPillarLength() / 2, interactorInfo.worldY, 0});
    m_midEdge->setTargetVertex({interactorInfo.worldX + MHDrawHouseManager::getDrawPillarLength() / 2, interactorInfo.worldY, 0});
    m_pillarEntity->updatePillar(*m_midEdge, MHDrawHouseManager::getDrawPillarHeight(), MHDrawHouseManager::getDrawPillarLength(), MHDrawHouseManager::getDrawPillarWidth());
    m_pillarEntity->generatePillar2D();
    m_pillarEntity->show();
    return true;
}

}  // namespace MHHouse
