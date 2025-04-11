/**
 * @file MHDrawWallLine.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHDrawWallLine.h"

#include "MHDrawManager.h"

namespace MHHouse {

void MHDrawWallLine::beginDraw() {
}

void MHDrawWallLine::endDraw() {
}

int MHDrawWallLine::getPriority() const {
    return 0;
}

bool MHDrawWallLine::onLeftButtonDown(const MHCore::MHInteractorInfo& interactorInfo) {
    return false;
}

bool MHDrawWallLine::onLeftButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    return false;
}

bool MHDrawWallLine::onRightButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    return false;
}

bool MHDrawWallLine::onMouseMove(const MHCore::MHInteractorInfo& interactorInfo) {
    return false;
}

}  // namespace MHHouse
