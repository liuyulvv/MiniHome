#pragma once

/**
 * @file MHFace.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include "MHWire.h"

namespace MHGeometry {

enum class MHFaceType {
    PLANE_FACE,
    CYLINDRICAL_FACE,
};

class MH_GEOMETRY_API MHFace : public MHTopoBase {
public:
    MHFace() = default;
    ~MHFace() = default;
    MHFace(const MHFace& face) = default;
    MHFace(MHFace&& face) = default;
    MHFace& operator=(const MHFace& face) = default;
    MHFace& operator=(MHFace&& face) = default;

public:
    virtual MHTopoType getType() const override {
        return MHTopoType::FACE;
    }
    virtual MHFaceType getFaceType() const = 0;
};

}  // namespace MHGeometry
