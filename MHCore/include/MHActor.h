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

#include <vtkOpenGLActor.h>

namespace MHCore {

class MHEntity;

class MH_CORE_API MHActor : public vtkOpenGLActor {
public:
    static MHActor* New();
    vtkTypeMacro(MHActor, vtkOpenGLActor);
    MHActor(const MHActor& actor) = delete;
    MHActor(MHActor&& actor) = delete;
    MHActor& operator=(const MHActor& actor) = delete;
    MHActor& operator=(MHActor&& actor) = delete;

protected:
    MHActor();
    ~MHActor();

private:
    MHEntity* m_Entity;
};

}  // namespace MHCore
