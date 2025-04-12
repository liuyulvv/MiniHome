#pragma once

/**
 * @file MHDrawWallLine.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHDrawBase.h"
#include "MHWallEntity.h"

namespace MHHouse {

class MHDrawHouseManager;

class MHDrawWallLine final : public MHDrawBase {
public:
    friend class MHDrawHouseManager;
    ~MHDrawWallLine() = default;
    MHDrawWallLine(const MHDrawWallLine& line) = delete;
    MHDrawWallLine(MHDrawWallLine&& line) = delete;
    MHDrawWallLine& operator=(const MHDrawWallLine& line) = delete;
    MHDrawWallLine& operator=(MHDrawWallLine&& line) = delete;

private:
    MHDrawWallLine() = default;

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
    };

private:
    DrawState m_drawState = DrawState::END;
    std::unique_ptr<MHGeometry::MHLineEdge> m_lineEdge = nullptr;
    std::shared_ptr<MHWallEntity> m_wallEntity = nullptr;
    bool m_wallEntityShow = false;
    std::vector<std::shared_ptr<MHWallEntity>> m_wallEntities = {};
};

}  // namespace MHHouse
