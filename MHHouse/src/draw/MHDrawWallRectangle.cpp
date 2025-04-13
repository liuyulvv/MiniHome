/**
 * @file MHDrawWallRectangle.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHDrawWallRectangle.h"

#include "MHDrawHouseManager.h"
#include "MHEntityManager.h"
#include "MHMainVTKInteractorStyle.h"
#include "MHSpaceManager.h"
#include "MHWallManager.h"

namespace MHHouse {

void MHDrawWallRectangle::beginDraw() {
    MHCore::MHMainVTKInteractorStyle::getInstance().insertFilter(shared_from_this());
    m_lineEdge = std::make_unique<MHGeometry::MHLineEdge>();
    m_wallEntities.clear();
    for (int i = 0; i < 4; ++i) {
        m_wallEntities.push_back(std::make_shared<MHWallEntity>());
    }
    m_drawState = DrawState::FIRST;
}

void MHDrawWallRectangle::endDraw() {
    MHCore::MHMainVTKInteractorStyle::getInstance().removeFilter(shared_from_this());
    m_lineEdge.reset();
    for (auto wallEntity : m_wallEntities) {
        if (wallEntity) {
            wallEntity->destroy();
        }
    }
    m_wallEntities.clear();
    m_drawState = DrawState::END;
}

int MHDrawWallRectangle::getPriority() const {
    return 0;
}

bool MHDrawWallRectangle::onLeftButtonDown(const MHCore::MHInteractorInfo& interactorInfo) {
    if (m_drawState == DrawState::FIRST) {
        m_lineEdge->setSourceVertex({interactorInfo.worldX, interactorInfo.worldY, 0});
        m_drawState = DrawState::SECOND;
    } else if (m_drawState == DrawState::SECOND) {
        m_lineEdge->setTargetVertex({interactorInfo.worldX, interactorInfo.worldY, 0});
        updateWallEntities(true);
        for (auto wallEntity : m_wallEntities) {
            wallEntity->show(false);
            MHWallManager::getInstance().addWall(wallEntity);
            MHCore::MHEntityManager::getInstance().addEntity(wallEntity);
        }
        MHSpaceManager::getInstance().generateSpaces();
        MHCore::MHMainVTKInteractorStyle::getInstance().render();
        m_wallEntities.clear();
        for (int i = 0; i < 4; ++i) {
            m_wallEntities.push_back(std::make_shared<MHWallEntity>());
        }
        m_drawState = DrawState::FIRST;
    }
    return true;
}

bool MHDrawWallRectangle::onLeftButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    return true;
}

bool MHDrawWallRectangle::onRightButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    MHDrawHouseManager::getInstance().endDraw();
    MHCore::MHMainVTKInteractorStyle::getInstance().render();
    return true;
}

bool MHDrawWallRectangle::onMouseMove(const MHCore::MHInteractorInfo& interactorInfo) {
    if (m_drawState == DrawState::END || m_drawState == DrawState::FIRST) {
        return true;
    }
    if (m_drawState == DrawState::SECOND) {
        m_lineEdge->setTargetVertex({interactorInfo.worldX, interactorInfo.worldY, 0});
        updateWallEntities(false);
        for (auto& wallEntity : m_wallEntities) {
            wallEntity->show(false);
        }
        MHCore::MHMainVTKInteractorStyle::getInstance().render();
    }
    return true;
}

void MHDrawWallRectangle::updateWallEntities(bool generate3D) {
    if (!m_lineEdge || m_drawState != DrawState::SECOND) {
        return;
    }
    auto sourceVertex = m_lineEdge->getSourceVertex();
    auto targetVertex = m_lineEdge->getTargetVertex();
    double xMin = std::min(sourceVertex.x, targetVertex.x);
    double xMax = std::max(sourceVertex.x, targetVertex.x);
    double yMin = std::min(sourceVertex.y, targetVertex.y);
    double yMax = std::max(sourceVertex.y, targetVertex.y);
    MHGeometry::MHVertex topLeft{xMin, yMax, 0};
    MHGeometry::MHVertex topRight{xMax, yMax, 0};
    MHGeometry::MHVertex bottomRight{xMax, yMin, 0};
    MHGeometry::MHVertex bottomLeft{xMin, yMin, 0};
    MHGeometry::MHLineEdge lineEdgeTop{topLeft, topRight};
    MHGeometry::MHLineEdge lineEdgeRight{topRight, bottomRight};
    MHGeometry::MHLineEdge lineEdgeBottom{bottomRight, bottomLeft};
    MHGeometry::MHLineEdge lineEdgeLeft{bottomLeft, topLeft};
    std::vector<MHGeometry::MHLineEdge> lineEdges{lineEdgeTop, lineEdgeRight, lineEdgeBottom, lineEdgeLeft};
    for (size_t i = 0; i < lineEdges.size(); i++) {
        m_wallEntities[i]->updateWall(lineEdges[i], MHDrawHouseManager::getDrawWallHeight(), MHDrawHouseManager::getDrawWallWidth(), MHDrawHouseManager::getDrawWallPositionType());
        m_wallEntities[i]->generateWall2D();
        if (generate3D) {
            m_wallEntities[i]->generateWall3D();
        }
    }
}

}  // namespace MHHouse
