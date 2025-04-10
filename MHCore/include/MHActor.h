#pragma once

/**
 * @file MHActor.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#ifdef MH_CORE_EXPORTS
#define MH_CORE_API __declspec(dllexport)
#else
#define MH_CORE_API __declspec(dllimport)
#endif

#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkOpenGLActor.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>

#include <memory>

namespace MHCore {

class MHEntity;

class MH_CORE_API MHActor : public vtkOpenGLActor {
public:
    static MHActor* New() {
        return new MHActor();
    }
    vtkTypeMacro(MHActor, vtkOpenGLActor);
    MHActor(const MHActor& actor) = delete;
    MHActor(MHActor&& actor) = delete;
    MHActor& operator=(const MHActor& actor) = delete;
    MHActor& operator=(MHActor&& actor) = delete;

private:
    MHActor();
    ~MHActor();

public:
    void setEntity(std::shared_ptr<MHEntity> entity);
    std::shared_ptr<MHEntity> getEntity() const;

private:
    std::shared_ptr<MHEntity> m_Entity = nullptr;
};

}  // namespace MHCore
