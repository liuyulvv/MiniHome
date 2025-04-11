/**
 * @file MHDrawWallRectangle.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHDrawWallRectangle.h"

#include "MHDrawWallManager.h"

namespace MHHouse {

MHDrawWallRectangle::MHDrawWallRectangle() {
}

void MHDrawWallRectangle::beginDraw() {
}

void MHDrawWallRectangle::endDraw() {
}

int MHDrawWallRectangle::getPriority() const {
    return 0;
}

bool MHDrawWallRectangle::onLeftButtonDown(const MHCore::MHInteractorInfo& interactorInfo) {
    return false;
}

bool MHDrawWallRectangle::onLeftButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    return false;
}

bool MHDrawWallRectangle::onRightButtonUp(const MHCore::MHInteractorInfo& interactorInfo) {
    return false;
}

bool MHDrawWallRectangle::onMouseMove(const MHCore::MHInteractorInfo& interactorInfo) {
    return false;
}

}  // namespace MHHouse
