#pragma once

/**
 * @file MHDrawWallRectangle.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHDrawBase.h"

namespace MHHouse {

class MHDrawHouseManager;

class MHDrawWallRectangle final : public MHDrawBase {
public:
    friend class MHDrawHouseManager;
    ~MHDrawWallRectangle() = default;
    MHDrawWallRectangle(const MHDrawWallRectangle& rectangle) = delete;
    MHDrawWallRectangle(MHDrawWallRectangle&& rectangle) = delete;
    MHDrawWallRectangle& operator=(const MHDrawWallRectangle& rectangle) = delete;
    MHDrawWallRectangle& operator=(MHDrawWallRectangle&& rectangle) = delete;

private:
    MHDrawWallRectangle();

public:
    virtual void beginDraw() override;
    virtual void endDraw() override;
    virtual int getPriority() const override;
    virtual bool onLeftButtonDown(const MHCore::MHInteractorInfo& interactorInfo) override;
    virtual bool onLeftButtonUp(const MHCore::MHInteractorInfo& interactorInfo) override;
    virtual bool onRightButtonUp(const MHCore::MHInteractorInfo& interactorInfo) override;
    virtual bool onMouseMove(const MHCore::MHInteractorInfo& interactorInfo) override;

private:
};

}  // namespace MHHouse
