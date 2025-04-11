#pragma once

/**
 * @file MHDrawBase.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHInteractorFilter.h"

namespace MHHouse {

enum class MHDrawType {
    NONE,
    WALL_RECTANGLE,
    WALL_LINE,
    WALL_ARC
};

class MHDrawBase : public MHCore::MHInteractorFilter {
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
