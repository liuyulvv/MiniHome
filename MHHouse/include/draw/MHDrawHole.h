#pragma once

/**
 * @file MHDrawHole.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-20
 */

#include "MHDrawBase.h"
#include "MHHoleEntity.h"
#include "MHWallEntity.h"

namespace MHHouse {

class MHDrawHouseManager;

class MHDrawHole : public MHDrawBase {
public:
    friend class MHDrawHouseManager;
    ~MHDrawHole() = default;
    MHDrawHole(const MHDrawHole& hole) = delete;
    MHDrawHole(MHDrawHole&& hole) = delete;
    MHDrawHole& operator=(const MHDrawHole& hole) = delete;
    MHDrawHole& operator=(MHDrawHole&& hole) = delete;

private:
    MHDrawHole();

public:
    virtual void beginDraw() override;
    virtual void endDraw() override;
    virtual int getPriority() const override;
    virtual bool onLeftButtonDown(const MHCore::MHInteractorInfo& interactorInfo) override;
    virtual bool onLeftButtonUp(const MHCore::MHInteractorInfo& interactorInfo) override;
    virtual bool onRightButtonUp(const MHCore::MHInteractorInfo& interactorInfo) override;
    virtual bool onMouseMove(const MHCore::MHInteractorInfo& interactorInfo) override;

private:
    void createTextures();
    void pickWall();
    void updateHole(const MHCore::MHInteractorInfo& interactorInfo);

private:
    enum class DrawState {
        END,
        FIRST,
    };

private:
    DrawState m_drawState = DrawState::END;
    std::shared_ptr<MHWallEntity> m_wallEntity = nullptr;
    std::shared_ptr<MHHoleEntity> m_holeEntity = nullptr;
    vtkSmartPointer<vtkTexture> m_invalidTexture = nullptr;
    vtkSmartPointer<vtkTexture> m_validTexture = nullptr;
};

}  // namespace MHHouse
