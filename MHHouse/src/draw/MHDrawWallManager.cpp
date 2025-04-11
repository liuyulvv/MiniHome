/**
 * @file MHDrawWallManager.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHDrawWallManager.h"

#include "MHMainVTKInteractorStyle.h"

namespace MHHouse {

MHDrawWallManager::MHDrawWallManager() {
    m_drawWallRectangle.reset(new MHDrawWallRectangle());
    m_drawWallLine.reset(new MHDrawWallLine());
}

void MHDrawWallManager::beginDraw(MHDrawType drawType) {
    if (m_drawType == drawType) {
        return;
    }
    MHCore::MHMainVTKInteractorStyle::getInstance().switchTo(MHCore::MHInteractorType::Top2D);
    endDraw();
    m_drawType = drawType;
    switch (m_drawType) {
        case MHDrawType::WALL_RECTANGLE:
            m_drawWallRectangle->beginDraw();
            break;
        case MHDrawType::WALL_LINE:
            m_drawWallLine->beginDraw();
            break;
        default:
            m_drawType = MHDrawType::NONE;
            break;
    }
}

void MHDrawWallManager::endDraw() {
    switch (m_drawType) {
        case MHDrawType::WALL_RECTANGLE:
            m_drawWallRectangle->endDraw();
            break;
        case MHDrawType::WALL_LINE:
            m_drawWallLine->endDraw();
            break;
        default:
            break;
    }
    m_drawType = MHDrawType::NONE;
}

}  // namespace MHHouse
