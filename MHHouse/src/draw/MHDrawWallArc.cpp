/**
 * @file MHDrawWallArc.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-16
 */

#include "MHDrawWallArc.h"

#include "MHDrawHouseManager.h"
#include "MHEntityManager.h"
#include "MHMainVTKInteractorStyle.h"
#include "MHSpaceManager.h"
#include "MHWallManager.h"

namespace MHHouse {

void MHDrawWallArc::beginDraw() {
    MHCore::MHMainVTKInteractorStyle::getInstance().insertFilter(shared_from_this());
    m_lineEdge = std::make_unique<MHGeometry::MHLineEdge>();
    m_midVertex = nullptr;
    m_arcEdge = std::make_unique<MHGeometry::MHArcEdge>();
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
        m_midVertex = std::make_unique<MHGeometry::MHVertex>((source + target) / 2.0);
        m_drawState = DrawState::THIRD;
    } else if (m_drawState == DrawState::THIRD) {
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
    if (m_drawState == DrawState::END || m_drawState == DrawState::FIRST || !m_lineEdge || !m_wallEntity || !m_midVertex) {
        return true;
    }
    if (m_drawState == DrawState::SECOND) {
        m_lineEdge->setTargetVertex({interactorInfo.worldX, interactorInfo.worldY, 0});
        m_wallEntity->updateWall(*m_lineEdge, MHDrawHouseManager::getDrawWallHeight(), MHDrawHouseManager::getDrawWallWidth(), MHDrawHouseManager::getDrawWallPositionType());
        m_wallEntity->generateWall2D();
        m_wallEntity->show();
    } else if (m_drawState == DrawState::THIRD && m_midVertex) {
        MHGeometry::MHVertex current = {interactorInfo.worldX, interactorInfo.worldY, 0};
        auto source = m_lineEdge->getSourceVertex();
        auto target = m_lineEdge->getTargetVertex();
        MHGeometry::MHVertex arcCenter, arcSource, arcTarget;
        double radius = (source - target).length() / 2.0;
        if (MHGeometry::MHToolKit::areCollinear(current, source, target)) {
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
            MHGeometry::MHVertex direction = *m_midVertex - current;
            if (direction.length() < 1e-6) {
                return true;
            }
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
                arcCenter = current - direction * radius;
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
        m_arcEdge = std::make_unique<MHGeometry::MHArcEdge>(arcCenter, normal, radius, sourceAangle, targetAngle);
    }
    return true;
}

}  // namespace MHHouse
