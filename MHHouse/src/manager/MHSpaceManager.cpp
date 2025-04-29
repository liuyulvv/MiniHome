/**
 * @file MHSpaceManager.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#include "MHSpaceManager.h"

#include "MHCylinderManager.h"
#include "MHFaceToolKit.h"
#include "MHPillarManager.h"
#include "MHWallManager.h"

namespace MHHouse {

MHSpaceManager& MHSpaceManager::getInstance() {
    static MHSpaceManager instance;
    return instance;
}

void MHSpaceManager::generateSpaces() {
    std::vector<std::shared_ptr<MHGeometry::MHEdge>> edges;
    std::vector<MHGeometry::MHPlaneFace> excludeFaces;

    auto walls = MHWallManager::getInstance().getWalls();
    for (const auto& wall : walls) {
        if (wall) {
            auto wallEdges = wall->getEdges();
            auto baseFace = wall->getBaseFace();
            if (baseFace) {
                excludeFaces.push_back(*baseFace);
            }
            for (auto& edge : wallEdges) {
                if (edge) {
                    edges.push_back(edge);
                }
            }
        }
    }

    auto pillars = MHPillarManager::getInstance().getPillars();
    for (const auto& pillar : pillars) {
        if (pillar) {
            auto pillarEdges = pillar->getEdges();
            auto baseFace = pillar->getBaseFace();
            if (baseFace) {
                excludeFaces.push_back(*baseFace);
            }
            for (auto& edge : pillarEdges) {
                if (edge) {
                    edges.push_back(edge);
                }
            }
        }
    }

    auto cylinders = MHCylinderManager::getInstance().getCylinders();
    for (const auto& cylinder : cylinders) {
        if (cylinder) {
            auto cylinderEdge = cylinder->getEdge();
            auto baseFace = cylinder->getBaseFace();
            if (baseFace) {
                excludeFaces.push_back(*baseFace);
            }
            if (cylinderEdge) {
                edges.push_back(cylinderEdge);
            }
        }
    }

    auto faces = MHGeometry::MHToolKit::makeArrangement(edges);
    std::vector<MHGeometry::MHPlaneFace> spaceFaces;
    for (auto& face : faces) {
        bool isSpace = true;
        for (auto& excludeFace : excludeFaces) {
            if (MHGeometry::MHToolKit::getFaceIntersectionType(face, excludeFace) == MHGeometry::MHToolKit::MHFaceIntersectionType::FACE) {
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
        spaceEntity->updateSpace(MHGeometry::MHToolKit::toTopoDSFace(face));
        spaceEntity->generateSpace2D();
        spaceEntity->generateSpace3D();
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

void MHSpaceManager::clear() {
    while (!m_spaces.empty()) {
        auto it = m_spaces.begin();
        if (it->second) {
            it->second->destroy();
        }
    }
    m_spaces.clear();
}

}  // namespace MHHouse
