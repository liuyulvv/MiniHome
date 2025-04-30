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
#include "MHLineEdge.h"

namespace MHCore {

class MH_CORE_API MHLineEntity : public MHEntity {
public:
    explicit MHLineEntity(vtkSmartPointer<MHRenderer> renderer = nullptr);
    ~MHLineEntity() = default;
    MHLineEntity(const MHLineEntity& entity) = delete;
    MHLineEntity(MHLineEntity&& entity) = delete;
    MHLineEntity& operator=(const MHLineEntity& entity) = delete;
    MHLineEntity& operator=(MHLineEntity&& entity) = delete;

public:
    void update(const MHGeometry::MHVertex& sourceVertex, const MHGeometry::MHVertex& targetVertex);
    void update(const MHGeometry::MHVertex& sourceVertex, const MHGeometry::MHVertex& targetVertex, double length);
};

}  // namespace MHCore
