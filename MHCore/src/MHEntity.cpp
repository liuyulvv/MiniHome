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
    boost::uuids::time_generator_v7 generator;
    auto uuid = generator();
    m_id = boost::uuids::to_string(uuid);
    m_actor = vtkSmartPointer<MHActor>::New();
    createDefaultTexture();
    m_actor->SetTexture(m_texture);
}

MHEntity::~MHEntity() {
    m_renderer->RemoveActor(m_actor);
    m_actor = nullptr;
    m_renderer = nullptr;
}

const std::string& MHEntity::getId() const {
    return m_id;
}

void MHEntity::show() {
    m_renderer->AddActor(m_actor);
    m_renderer->render();
}

void MHEntity::createDefaultTexture() {
    vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
    imageData->SetDimensions(1, 1, 1);
    imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 3);
    unsigned char* pixel = static_cast<unsigned char*>(imageData->GetScalarPointer(0, 0, 0));
    pixel[0] = 0;
    pixel[1] = 0;
    pixel[2] = 255;
    m_texture = vtkSmartPointer<vtkTexture>::New();
    m_texture->SetInputData(imageData);
    m_texture->InterpolateOn();
}

}  // namespace MHCore
