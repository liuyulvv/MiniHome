/**
 * @file MHEntity.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include "MHEntity.h"

#include "MHActor.h"

namespace MHCore {

MHEntity::MHEntity() {
    m_Actor = nullptr;
}

MHEntity::~MHEntity() {
    m_Actor = nullptr;
}

}  // namespace MHCore
