#pragma once

/**
 * @file MHDrawPillar.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-13
 */

#include "MHDrawBase.h"
#include "MHPillarEntity.h"

namespace MHHouse {

class MHDrawHouseManager;

class MHDrawPillar : public MHDrawBase {
public:
    friend class MHDrawHouseManager;
    ~MHDrawPillar() = default;
    MHDrawPillar(const MHDrawPillar& pillar) = delete;
    MHDrawPillar(MHDrawPillar&& pillar) = delete;
    MHDrawPillar& operator=(const MHDrawPillar& pillar) = delete;
    MHDrawPillar& operator=(MHDrawPillar&& pillar) = delete;

private:
    MHDrawPillar() = default;

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
    std::shared_ptr<MHGeometry::MHLineEdge> m_midEdge = nullptr;
    std::shared_ptr<MHPillarEntity> m_pillarEntity = nullptr;
};

}  // namespace MHHouse
