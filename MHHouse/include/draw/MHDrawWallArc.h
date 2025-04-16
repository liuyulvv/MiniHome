#pragma once

/**
 * @file MHDrawWallArc.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-16
 */

#include "MHDrawBase.h"
#include "MHWallEntity.h"

namespace MHHouse {

class MHDrawHouseManager;

class MHDrawWallArc final : public MHDrawBase {
public:
    friend class MHDrawHouseManager;
    ~MHDrawWallArc() = default;
    MHDrawWallArc(const MHDrawWallArc& arc) = delete;
    MHDrawWallArc(MHDrawWallArc&& arc) = delete;
    MHDrawWallArc& operator=(const MHDrawWallArc& arc) = delete;
    MHDrawWallArc& operator=(MHDrawWallArc&& arc) = delete;

private:
    MHDrawWallArc() = default;

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
        SECOND,
        THIRD,
    };

private:
    DrawState m_drawState = DrawState::END;
    std::unique_ptr<MHGeometry::MHLineEdge> m_lineEdge = nullptr;
    std::unique_ptr<MHGeometry::MHVertex> m_midVertex = nullptr;
    std::unique_ptr<MHGeometry::MHArcEdge> m_arcEdge = nullptr;
    std::shared_ptr<MHWallEntity> m_wallEntity = nullptr;
};

}  // namespace MHHouse
