#pragma once

/**
 * @file MHDrawBase.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include <memory>

#include "MHInteractorFilter.h"

namespace MHHouse {

enum class MHDrawType {
    NONE,
    WALL_LINE,
    WALL_RECTANGLE,
    WALL_ARC,
    PILLAR,
};

class MHDrawBase : public MHCore::MHInteractorFilter, public std::enable_shared_from_this<MHDrawBase> {
public:
    MHDrawBase() = default;
    virtual ~MHDrawBase() = default;
    MHDrawBase(const MHDrawBase& base) = delete;
    MHDrawBase(MHDrawBase&& base) = delete;
    MHDrawBase& operator=(const MHDrawBase& base) = delete;
    MHDrawBase& operator=(MHDrawBase&& base) = delete;

public:
    virtual void beginDraw() = 0;
    virtual void endDraw() = 0;
};

}  // namespace MHHouse
