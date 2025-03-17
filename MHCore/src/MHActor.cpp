/**
 * @file MHActor.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include "MHActor.h"

#include <vtkObjectFactory.h>

namespace MHCore {

vtkStandardNewMacro(MHActor);

MHActor::MHActor() {
    m_Entity = nullptr;
}

MHActor::~MHActor() {
    m_Entity = nullptr;
}

}  // namespace MHCore
