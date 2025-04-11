#pragma once

/**
 * @file MHEntity.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#ifdef MH_CORE_EXPORTS
#define MH_CORE_API __declspec(dllexport)
#else
#define MH_CORE_API __declspec(dllimport)
#endif

#include <string>

#include "MHActor.h"
#include "MHRenderer.h"

namespace MHCore {

class MH_CORE_API MHEntity {
public:
    MHEntity(vtkSmartPointer<MHRenderer> renderer);
    ~MHEntity();
    MHEntity(const MHEntity& entity) = delete;
    MHEntity(MHEntity&& entity) = delete;
    MHEntity& operator=(const MHEntity& entity) = delete;
    MHEntity& operator=(MHEntity&& entity) = delete;

public:
    const std::string& getId() const;
    virtual void show();

protected:
    vtkSmartPointer<MHRenderer> m_renderer = nullptr;
    std::string m_id;
    vtkSmartPointer<vtkActor> m_actor = nullptr;
};

}  // namespace MHCore
