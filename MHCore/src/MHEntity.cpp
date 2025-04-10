/**
 * @file MHEntity.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2025-03-17
 */

#include "MHEntity.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "MHActor.h"

namespace MHCore {

MHEntity::MHEntity(vtkSmartPointer<MHRenderer> renderer) : m_renderer(renderer) {
    m_Actor = nullptr;
    boost::uuids::time_generator_v7 generator;
    auto uuid = generator();
    m_id = boost::uuids::to_string(uuid);
}

MHEntity::~MHEntity() {
    m_renderer->RemoveActor(m_Actor);
    m_Actor = nullptr;
    m_renderer = nullptr;
}

const std::string& MHEntity::getId() const {
    return m_id;
}

void MHEntity::show() {
    m_renderer->AddActor(m_Actor);
    m_renderer->render();
}

}  // namespace MHCore
