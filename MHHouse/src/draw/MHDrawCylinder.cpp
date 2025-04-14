/**
 * @file MHDrawCylinder.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-14
 */

#include "MHDrawCylinder.h"

#include "MHCylinderManager.h"
#include "MHDrawHouseManager.h"
#include "MHEntityManager.h"
#include "MHMainVTKInteractorStyle.h"
#include "MHSpaceManager.h"

namespace MHHouse {

void MHDrawCylinder::beginDraw() {
    MHCore::MHMainVTKInteractorStyle::getInstance().insertFilter(shared_from_this());
    m_centerVertex = std::make_unique<MHGeometry::MHVertex>();
    m_cylinderEntity = std::make_shared<MHCylinderEntity>();
    m_drawState = DrawState::FIRST;
}

void MHDrawCylinder::endDraw() {
    MHCore::MHMainVTKInteractorStyle::getInstance().removeFilter(shared_from_this());
    m_centerVertex.reset(nullptr);
    if (m_cylinderEntity) {
        m_cylinderEntity->destroy();
    }
    m_cylinderEntity.reset();
    m_drawState = DrawState::END;
}

int MHDrawCylinder::getPriority() const {
    return 0;
}

bool MHDrawCylinder::onLeftButtonDown(const MHCore::MHInteractorInfo& interactorInfo) {
    if (m_drawState == DrawState::END || !m_centerVertex || !m_cylinderEntity) {
        return true;
    }
    m_centerVertex->x = interactorInfo.worldX;
    m_centerVertex->y = interactorInfo.worldY;
    m_centerVertex->z = 0;
    m_cylinderEntity->updateCylinder(*m_centerVertex, MHDrawHouseManager::getDrawCylinderRadius(), MHDrawHouseManager::getDrawCylinderHeight());
    m_cylinderEntity->generateCylinder2D();
    m_cylinderEntity->generateCylinder3D();
    m_cylinderEntity->show(false);
    MHCylinderManager::getInstance().addCylinder(m_cylinderEntity);
    MHCore::MHEntityManager::getInstance().addEntity(m_cylinderEntity);
    MHSpaceManager::getInstance().generateSpaces();
    m_cylinderEntity = std::make_shared<MHCylinderEntity>();
    MHCore::MHMainVTKInteractorStyle::getInstance().render();
    return true;
}

bool MHDrawCylinder::onLeftButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    return true;
}

bool MHDrawCylinder::onRightButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    MHDrawHouseManager::getInstance().endDraw();
    MHCore::MHMainVTKInteractorStyle::getInstance().render();
    return true;
}

bool MHDrawCylinder::onMouseMove(const MHCore::MHInteractorInfo& interactorInfo) {
    if (m_drawState == DrawState::END || !m_centerVertex || !m_cylinderEntity) {
        return true;
    }
    m_centerVertex->x = interactorInfo.worldX;
    m_centerVertex->y = interactorInfo.worldY;
    m_centerVertex->z = 0;
    m_cylinderEntity->updateCylinder(*m_centerVertex, MHDrawHouseManager::getDrawCylinderRadius(), MHDrawHouseManager::getDrawCylinderHeight());
    m_cylinderEntity->generateCylinder2D();
    m_cylinderEntity->show();
    return true;
}

}  // namespace MHHouse
