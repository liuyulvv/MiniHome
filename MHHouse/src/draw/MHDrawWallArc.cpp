/**
 * @file MHDrawWallArc.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-16
 */

#include "MHDrawWallArc.h"

#include "MHDrawHouseManager.h"
#include "MHEdgeToolKit.h"
#include "MHEntityManager.h"
#include "MHMainVTKInteractorStyle.h"
#include "MHSpaceManager.h"
#include "MHWallManager.h"

namespace MHHouse {

void MHDrawWallArc::beginDraw() {
    MHCore::MHMainVTKInteractorStyle::getInstance().insertFilter(shared_from_this());
    m_lineEdge = std::make_shared<MHGeometry::MHLineEdge>();
    m_midVertex = nullptr;
    m_arcEdge = std::make_shared<MHGeometry::MHArcEdge>();
    m_wallEntity = std::make_shared<MHWallEntity>();
    m_drawState = DrawState::FIRST;
}

void MHDrawWallArc::endDraw() {
    MHCore::MHMainVTKInteractorStyle::getInstance().removeFilter(shared_from_this());
    m_lineEdge = nullptr;
    m_midVertex = nullptr;
    m_arcEdge = nullptr;
    if (m_wallEntity) {
        m_wallEntity->destroy();
    }
    m_wallEntity = nullptr;
    m_drawState = DrawState::END;
}

int MHDrawWallArc::getPriority() const {
    return 0;
}

bool MHDrawWallArc::onLeftButtonDown(const MHCore::MHInteractorInfo& interactorInfo) {
    if (m_drawState == DrawState::FIRST) {
        m_lineEdge->setSourceVertex({interactorInfo.worldX, interactorInfo.worldY, 0});
        m_drawState = DrawState::SECOND;
    } else if (m_drawState == DrawState::SECOND) {
        m_lineEdge->setTargetVertex({interactorInfo.worldX, interactorInfo.worldY, 0});
        auto source = m_lineEdge->getSourceVertex();
        auto target = m_lineEdge->getTargetVertex();
        m_midVertex = std::make_shared<MHGeometry::MHVertex>((source + target) / 2.0);
        m_drawState = DrawState::THIRD;
    } else if (m_drawState == DrawState::THIRD) {
        computeArcEdge(interactorInfo);
        m_wallEntity->generateWall2D();
        m_wallEntity->generateWall3D();
        m_wallEntity->show();
        MHWallManager::getInstance().addWall(m_wallEntity);
        MHCore::MHEntityManager::getInstance().addEntity(m_wallEntity);
        MHSpaceManager::getInstance().generateSpaces();
        m_wallEntity = std::make_shared<MHWallEntity>();
        m_drawState = DrawState::FIRST;
    }
    return true;
}

bool MHDrawWallArc::onLeftButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    return true;
}

bool MHDrawWallArc::onRightButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    MHDrawHouseManager::getInstance().endDraw();
    MHCore::MHMainVTKInteractorStyle::getInstance().render();
    return true;
}

bool MHDrawWallArc::onMouseMove(const MHCore::MHInteractorInfo& interactorInfo) {
    if (m_drawState == DrawState::END || m_drawState == DrawState::FIRST || !m_lineEdge || !m_wallEntity) {
        return true;
    }
    if (m_drawState == DrawState::SECOND) {
        m_lineEdge->setTargetVertex({interactorInfo.worldX, interactorInfo.worldY, 0});
        m_wallEntity->updateWall(*m_lineEdge, MHDrawHouseManager::getDrawWallHeight(), MHDrawHouseManager::getDrawWallWidth(), MHDrawHouseManager::getDrawWallPositionType());
        m_wallEntity->generateWall2D();
        m_wallEntity->show();
    } else if (m_drawState == DrawState::THIRD && m_midVertex) {
        if (m_lineEdge->length() < 1e-6) {
            return true;
        }
        computeArcEdge(interactorInfo);
        m_wallEntity->updateWall(*m_arcEdge, MHDrawHouseManager::getDrawWallHeight(), MHDrawHouseManager::getDrawWallWidth(), MHDrawHouseManager::getDrawWallPositionType());
        m_wallEntity->generateWall2D();
        m_wallEntity->show();
    }
    return true;
}

bool MHDrawWallArc::onKeyPress(const MHCore::MHInteractorInfo& interactorInfo) {
    if (interactorInfo.pressedKeys.size() == 1 && interactorInfo.pressedKeys.contains("SPACE")) {
        auto currentType = static_cast<int>(MHDrawHouseManager::getInstance().getDrawWallPositionType());
        currentType = (currentType + 1) % 3;
        MHDrawHouseManager::getInstance().setDrawWallPositionType(static_cast<MHWallPositionType>(currentType));
    }
    return true;
}

bool MHDrawWallArc::onKeyRelease(const MHCore::MHInteractorInfo& interactorInfo) {
    return true;
}

void MHDrawWallArc::computeArcEdge(const MHCore::MHInteractorInfo& interactorInfo) {
    MHGeometry::MHVertex current = {interactorInfo.worldX, interactorInfo.worldY, 0};
    auto source = m_lineEdge->getSourceVertex();
    auto target = m_lineEdge->getTargetVertex();
    auto lineEdgeNormal = MHGeometry::MHToolKit::getLineNormal(source, target);
    auto normalEndVertex = *m_midVertex + lineEdgeNormal * 100;
    current = MHGeometry::MHToolKit::projectToLine(current, *m_midVertex, normalEndVertex);
    MHGeometry::MHVertex arcCenter, arcSource, arcTarget;
    double radius = (source - target).length() / 2.0;
    MHGeometry::MHVertex direction = *m_midVertex - current;
    if (direction.length() < 1e-6) {
        return;
    }
    if (MHGeometry::MHToolKit::areCollinear(current, source, target, 1e-3)) {
        if (MHGeometry::MHToolKit::isParallelToYAxis(source, target)) {
            arcCenter = *m_midVertex;
            arcSource = source.y > target.y ? source : target;
            arcTarget = source.y > target.y ? target : source;
        } else {
            arcCenter = *m_midVertex;
            arcSource = source.x > target.x ? source : target;
            arcTarget = source.x > target.x ? target : source;
        }
    } else {
        direction = direction.normalize();
        auto distanceToCurrent = (*m_midVertex - current).length();
        auto distanceToSource = (*m_midVertex - source).length();
        if (std::abs(distanceToCurrent - distanceToSource) > 1e-6) {
            radius = (distanceToCurrent * distanceToCurrent + distanceToSource * distanceToSource) / (2 * distanceToCurrent);
        }
        bool isLeft = MHGeometry::MHToolKit::isLeftTurn(*m_midVertex, current, source);
        if (std::abs(radius - distanceToCurrent) < 1e-6) {
            arcCenter = *m_midVertex;
        } else {
            arcCenter = current + direction * radius;
        }
        if (isLeft) {
            arcSource = target;
            arcTarget = source;
        } else {
            arcSource = source;
            arcTarget = target;
        }
    }
    auto sourceAangle = MHGeometry::MHToolKit::angleToXAxis(arcCenter, arcSource);
    auto targetAngle = MHGeometry::MHToolKit::angleToXAxis(arcCenter, arcTarget);
    MHGeometry::MHVertex normal = {0, 0, 1};
    m_arcEdge = std::make_shared<MHGeometry::MHArcEdge>(arcCenter, normal, radius, sourceAangle, targetAngle);
}

}  // namespace MHHouse
