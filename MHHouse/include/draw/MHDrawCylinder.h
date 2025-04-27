#pragma once
/**
 * @file MHDrawCylinder.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-14
 */

#include "MHCylinderEntity.h"
#include "MHDrawBase.h"

namespace MHHouse {

class MHDrawHouseManager;

class MHDrawCylinder : public MHDrawBase {
public:
    friend class MHDrawHouseManager;
    ~MHDrawCylinder() = default;
    MHDrawCylinder(const MHDrawCylinder& cylinder) = delete;
    MHDrawCylinder(MHDrawCylinder&& cylinder) = delete;
    MHDrawCylinder& operator=(const MHDrawCylinder& cylinder) = delete;
    MHDrawCylinder& operator=(MHDrawCylinder&& cylinder) = delete;

private:
    MHDrawCylinder() = default;

public:
    virtual void beginDraw() override;
    virtual void endDraw() override;
    virtual int getPriority() const override;
    virtual bool onLeftButtonDown(const MHCore::MHInteractorInfo& interactorInfo) override;
    virtual bool onLeftButtonUp(const MHCore::MHInteractorInfo& interactorInfo) override;
    virtual bool onRightButtonUp(const MHCore::MHInteractorInfo& interactorInfo) override;
    virtual bool onMouseMove(const MHCore::MHInteractorInfo& interactorInfo) override;

private:
    enum class DrawState {
        END,
        FIRST,
    };

private:
    DrawState m_drawState = DrawState::END;
    std::shared_ptr<MHGeometry::MHVertex> m_centerVertex = nullptr;
    std::shared_ptr<MHCylinderEntity> m_cylinderEntity = nullptr;
};

}  // namespace MHHouse
