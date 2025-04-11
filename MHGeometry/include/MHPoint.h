#pragma once

/**
 * @file MHPoint.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#ifdef MH_GEOMETRY_EXPORTS
#define MH_GEOMETRY_API __declspec(dllexport)
#else
#define MH_GEOMETRY_API __declspec(dllimport)
#endif

namespace MHGeometry {

struct MH_GEOMETRY_API MHPoint {
    double x;
    double y;
    double z;
};

}  // namespace MHGeometry
