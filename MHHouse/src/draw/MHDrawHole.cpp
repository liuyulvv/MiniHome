/**
 * @file MHDrawHole.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-20
 */

#include "MHDrawHole.h"

#include <vtkImageReader2.h>
#include <vtkImageReader2Factory.h>

#include "MHDrawHouseManager.h"
#include "MHEntityManager.h"
#include "MHHoleManager.h"
#include "MHMainVTKInteractorStyle.h"
#include "MHVertexToolKit.h"

namespace MHHouse {

MHDrawHole::MHDrawHole() {
    createTextures();
}

void MHDrawHole::beginDraw() {
    MHCore::MHMainVTKInteractorStyle::getInstance().insertFilter(shared_from_this());
    m_wallEntity = nullptr;
    m_holeEntity = std::make_shared<MHHoleEntity>();
    m_drawState = DrawState::FIRST;
}

void MHDrawHole::endDraw() {
    MHCore::MHMainVTKInteractorStyle::getInstance().removeFilter(shared_from_this());
    m_wallEntity = nullptr;
    if (m_holeEntity) {
        m_holeEntity->destroy();
    }
    m_holeEntity = nullptr;
    m_drawState = DrawState::END;
}

int MHDrawHole::getPriority() const {
    return 0;
}

bool MHDrawHole::onLeftButtonDown(const MHCore::MHInteractorInfo& interactorInfo) {
    if (m_drawState == DrawState::END || !m_wallEntity || !m_holeEntity) {
        return true;
    }
    pickWall();
    if (m_wallEntity) {
        auto midEdge = m_wallEntity->getMidEdge();
        if (!midEdge) {
            return true;
        }
        updateHole(interactorInfo);
        m_holeEntity->generateHole3D();
        m_holeEntity->show(false);
        MHCore::MHEntityManager::getInstance().addEntity(m_holeEntity);
        MHHoleManager::getInstance().addHole(m_holeEntity);
        m_holeEntity = std::make_shared<MHHoleEntity>();
        MHCore::MHMainVTKInteractorStyle::getInstance().render();
    }
    return true;
}

bool MHDrawHole::onLeftButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    return true;
}

bool MHDrawHole::onRightButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    MHDrawHouseManager::getInstance().endDraw();
    MHCore::MHMainVTKInteractorStyle::getInstance().render();
    return true;
}

bool MHDrawHole::onMouseMove(const MHCore::MHInteractorInfo& interactorInfo) {
    if (m_drawState == DrawState::END || !m_holeEntity) {
        return true;
    }
    updateHole(interactorInfo);
    return true;
}

void MHDrawHole::createTextures() {
    vtkSmartPointer<vtkImageReader2Factory> readerFactory2D = vtkSmartPointer<vtkImageReader2Factory>::New();
    vtkSmartPointer<vtkImageReader2> textureReader2D = readerFactory2D->CreateImageReader2("textures/default_pillar_2D.png");
    textureReader2D->SetFileName("textures/default_pillar_2D.png");
    textureReader2D->Update();
    m_validTexture = vtkSmartPointer<vtkTexture>::New();
    m_validTexture->SetInputConnection(textureReader2D->GetOutputPort());
    m_validTexture->InterpolateOn();

    vtkSmartPointer<vtkImageData> invalidImageData = vtkSmartPointer<vtkImageData>::New();
    invalidImageData->SetDimensions(1, 1, 1);
    invalidImageData->AllocateScalars(VTK_UNSIGNED_CHAR, 3);
    unsigned char* invalidPixel = static_cast<unsigned char*>(invalidImageData->GetScalarPointer(0, 0, 0));
    invalidPixel[0] = 255;
    invalidPixel[1] = 0;
    invalidPixel[2] = 0;
    m_invalidTexture = vtkSmartPointer<vtkTexture>::New();
    m_invalidTexture->SetInputData(invalidImageData);
    m_invalidTexture->InterpolateOn();
}

void MHDrawHole::pickWall() {
    auto entities = MHCore::MHMainVTKInteractorStyle::getInstance().pickEntities();
    for (const auto& entity : entities) {
        auto wallEntity = std::dynamic_pointer_cast<MHWallEntity>(entity);
        if (wallEntity) {
            m_wallEntity = wallEntity;
            return;
        }
    }
    m_wallEntity = nullptr;
}

void MHDrawHole::updateHole(const MHCore::MHInteractorInfo& interactorInfo) {
    pickWall();
    if (m_wallEntity) {
        auto midEdge = m_wallEntity->getMidEdge();
        if (!midEdge) {
            return;
        }
        MHGeometry::MHVertex midVertex = MHGeometry::MHVertex(interactorInfo.worldX, interactorInfo.worldY, 0);
        if (midEdge->getEdgeType() == MHGeometry::MHEdgeType::LINE_EDGE) {
            midVertex = MHGeometry::MHToolKit::projectToLine(midVertex, midEdge->getSourceVertex(), midEdge->getTargetVertex());
        } else {
            auto arcEdge = dynamic_cast<MHGeometry::MHArcEdge*>(midEdge.get());
            if (!arcEdge) {
                return;
            }
            auto direction = midVertex - arcEdge->getCenter();
            if (direction.length() < 1e-6) {
                return;
            }
            direction = direction.normalize();
            midVertex = arcEdge->getCenter() + direction * arcEdge->getRadius();
        }
        m_holeEntity->updateHole(m_wallEntity, midVertex, std::dynamic_pointer_cast<MHGeometry::MHEdge>(midEdge->clone()), MHDrawHouseManager::getDrawHoleHeight(), MHDrawHouseManager::getDrawHoleLength(), MHDrawHouseManager::getDrawHoleWidth());
        m_holeEntity->generateHole2D();
        m_holeEntity->getHole2D()->setTexture(m_validTexture);
        m_holeEntity->show();
    } else {
        MHGeometry::MHVertex source = {interactorInfo.worldX - MHDrawHouseManager::getDrawHoleLength() / 2, interactorInfo.worldY, 0};
        MHGeometry::MHVertex target = {interactorInfo.worldX + MHDrawHouseManager::getDrawHoleLength() / 2, interactorInfo.worldY, 0};
        auto midEdge = std::make_shared<MHGeometry::MHLineEdge>(source, target);
        m_holeEntity->updateHole(nullptr, MHGeometry::MHVertex(interactorInfo.worldX, interactorInfo.worldY, 0), std::dynamic_pointer_cast<MHGeometry::MHEdge>(midEdge->clone()), MHDrawHouseManager::getDrawHoleHeight(), MHDrawHouseManager::getDrawHoleLength(), MHDrawHouseManager::getDrawHoleWidth());
        m_holeEntity->generateHole2D();
        m_holeEntity->getHole2D()->setTexture(m_invalidTexture);
        m_holeEntity->show();
    }
}

}  // namespace MHHouse
