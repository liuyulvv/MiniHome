#pragma once

/**
 * @file MHLineEntity.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-11
 */

#ifdef MH_CORE_EXPORTS
#define MH_CORE_API __declspec(dllexport)
#else
#define MH_CORE_API __declspec(dllimport)
#endif

#include "MHEntity.h"
#include "MHPoint.h"

namespace MHCore {

class MH_CORE_API MHLineEntity : public MHEntity {
public:
    MHLineEntity(vtkSmartPointer<MHRenderer> renderer);
    ~MHLineEntity() = default;
    MHLineEntity(const MHLineEntity& entity) = delete;
    MHLineEntity(MHLineEntity&& entity) = delete;
    MHLineEntity& operator=(const MHLineEntity& entity) = delete;
    MHLineEntity& operator=(MHLineEntity&& entity) = delete;

public:
    void setStartPoint(const MHGeometry::MHPoint& point);
    void setEndPoint(const MHGeometry::MHPoint& point);
    void update();

private:
    MHGeometry::MHPoint m_startPoint;
    MHGeometry::MHPoint m_endPoint;
};

}  // namespace MHCore
