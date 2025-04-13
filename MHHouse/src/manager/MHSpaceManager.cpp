/**
 * @file MHSpaceManager.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include "MHSpaceManager.h"

#include "MHFaceToolKit.h"
#include "MHPillarManager.h"
#include "MHWallManager.h"

namespace MHHouse {

MHSpaceManager& MHSpaceManager::getInstance() {
    static MHSpaceManager instance;
    return instance;
}

void MHSpaceManager::generateSpaces() {
    std::vector<std::unique_ptr<MHGeometry::MHEdge>> edges;
    std::vector<MHGeometry::MHPlaneFace> excludeFaces;

    auto walls = MHWallManager::getInstance().getWalls();
    for (const auto& wall : walls) {
        if (wall) {
            auto wallEEdges = wall->getEdges();
            auto baseFace = wall->getBaseFace();
            if (baseFace) {
                excludeFaces.push_back(*baseFace);
            }
            for (auto& edge : wallEEdges) {
                if (edge) {
                    edges.push_back(std::move(edge));
                }
            }
        }
    }

    auto pillars = MHPillarManager::getInstance().getPillars();
    for (const auto& pillar : pillars) {
        if (pillar) {
            auto pillarEEdges = pillar->getEdges();
            auto baseFace = pillar->getBaseFace();
            if (baseFace) {
                excludeFaces.push_back(*baseFace);
            }
            for (auto& edge : pillarEEdges) {
                if (edge) {
                    edges.push_back(std::move(edge));
                }
            }
        }
    }

    auto faces = MHGeometry::MHToolKit::makeArrangement(edges);
    std::vector<MHGeometry::MHPlaneFace> spaceFaces;
    for (auto& face : faces) {
        bool isSpace = true;
        for (auto& excludeFace : excludeFaces) {
            if (MHGeometry::MHToolKit::isIntersect(face, excludeFace)) {
                isSpace = false;
                break;
            }
        }
        if (isSpace) {
            spaceFaces.push_back(face);
        }
    }
    for (auto& space : m_spaces) {
        if (space.second) {
            space.second->destroy();
        }
    }
    m_spaces.clear();
    for (auto& face : spaceFaces) {
        auto spaceEntity = std::make_shared<MHSpaceEntity>();
        spaceEntity->setTopo(std::make_unique<MHGeometry::MHPlaneFace>(face));
        spaceEntity->updateTopo();
        spaceEntity->show();
        addSpace(spaceEntity);
    }
}

void MHSpaceManager::addSpace(std::shared_ptr<MHSpaceEntity> space) {
    if (space) {
        m_spaces[space->getId()] = space;
    }
}

void MHSpaceManager::removeSpace(const std::string& spaceId) {
    auto it = m_spaces.find(spaceId);
    if (it != m_spaces.end()) {
        m_spaces.erase(it);
    }
}

void MHSpaceManager::removeSpace(std::shared_ptr<MHSpaceEntity> space) {
    if (space) {
        auto it = m_spaces.find(space->getId());
        if (it != m_spaces.end()) {
            m_spaces.erase(it);
        }
    }
}

}  // namespace MHHouse
