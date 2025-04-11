#pragma once

/**
 * @file MHDrawWallLine.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHDrawBase.h"
#include "MHLineEntity.h"

namespace MHHouse {

class MHDrawWallManager;

class MHDrawWallLine final : public MHDrawBase {
public:
    friend class MHDrawWallManager;
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
    std::unique_ptr<MHCore::MHLineEntity> m_lineEntity = nullptr;
    bool m_lineEntityShow = false;
    std::vector<std::unique_ptr<MHCore::MHLineEntity>> m_lineEntities = {};
};

}  // namespace MHHouse
