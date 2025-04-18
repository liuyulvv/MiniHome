#pragma once

/**
 * @file MHHouseEntity.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-04-12
 */

#ifdef MH_HOUSE_EXPORTS
#define MH_HOUSE_API __declspec(dllexport)
#else
#define MH_HOUSE_API __declspec(dllimport)
#endif

#include <TopoDS_Shape.hxx>

#include "MHEntity.h"

namespace MHHouse {

class MH_HOUSE_API MHHouseEntity : public MHCore::MHEntity {
public:
    explicit MHHouseEntity(vtkSmartPointer<MHCore::MHRenderer> renderer = nullptr);
    virtual ~MHHouseEntity();
    MHHouseEntity(const MHHouseEntity& entity) = delete;
    MHHouseEntity(MHHouseEntity&& entity) = delete;
    MHHouseEntity& operator=(const MHHouseEntity& entity) = delete;
    MHHouseEntity& operator=(MHHouseEntity&& entity) = delete;

public:
    virtual void setTopo(const TopoDS_Shape& topo);
    virtual void updateTopo();
    virtual void onEnter() override;
    virtual void onLeave() override;

protected:
    TopoDS_Shape m_topo;
    vtkSmartPointer<vtkActor> m_outlineActor;
};

}  // namespace MHHouse
