/**
 * @file MHActor.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include "MHActor.h"

namespace MHCore {

MHActor::MHActor() {
    GetProperty()->BackfaceCullingOn();
}

MHActor::~MHActor() {
    m_Entity = nullptr;
}

void MHActor::Render(vtkRenderer* ren, vtkMapper* mapper) {
    if (m_renderCallback) {
        m_renderCallback();
    }
    vtkOpenGLActor::Render(ren, mapper);
}

void MHActor::setEntity(std::shared_ptr<MHEntity> entity) {
    m_Entity = entity;
}

std::shared_ptr<MHEntity> MHActor::getEntity() const {
    return m_Entity;
}

void MHActor::setRenderCallback(std::function<void()> callback) {
    m_renderCallback = callback;
}

}  // namespace MHCore
