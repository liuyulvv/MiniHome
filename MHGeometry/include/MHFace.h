#pragma once

/**
 * @file MHFace.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#include <vector>

#include "MHWire.h"

namespace MHGeometry {

class MHFace {
public:
    MHFace() = default;
    ~MHFace() = default;
    MHFace(const MHFace& face) = default;
    MHFace(MHFace&& face) = default;
    MHFace& operator=(const MHFace& face) = default;
    MHFace& operator=(MHFace&& face) = default;

private:
    std::vector<MHWire> m_wires;
};

}  // namespace MHGeometry
