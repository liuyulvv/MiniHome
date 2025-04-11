/**
 * @file MHDrawManager.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHDrawManager.h"

namespace MHHouse {

MHDrawManager::MHDrawManager() {
    m_drawWallRectangle.reset(new MHDrawWallRectangle());
    m_drawWallLine.reset(new MHDrawWallLine());
}

}  // namespace MHHouse
